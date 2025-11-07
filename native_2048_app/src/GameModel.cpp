#include "GameModel.h"
#include <QRandomGenerator>
#include <QDebug>

static constexpr int SIZE = 4;

GameModel::GameModel(QObject* parent)
    : QObject(parent)
    , m_settings("Kavia", "Native2048")
{
    m_bestScore = m_settings.value("bestScore", 0).toInt();

    // Optionally restore last session
    QVariant saved = m_settings.value("lastBoard");
    if (saved.isValid()) {
        QVariantList rows = saved.toList();
        if (rows.size() == SIZE) {
            m_board = QVector<QVector<int>>(SIZE, QVector<int>(SIZE, 0));
            for (int r = 0; r < SIZE; ++r) {
                QVariantList cols = rows[r].toList();
                if (cols.size() == SIZE) {
                    for (int c = 0; c < SIZE; ++c) m_board[r][c] = cols[c].toInt();
                }
            }
            m_score = m_settings.value("lastScore", 0).toInt();
            checkGameState();
            emit boardChanged(m_board);
            emit scoreChanged(m_score, m_bestScore);
            return;
        }
    }

    newGame();
}

QVector<QVector<int>> GameModel::board() const { return m_board; }
int GameModel::score() const { return m_score; }
int GameModel::bestScore() const { return m_bestScore; }
bool GameModel::canMove() const { return anyMovesAvailable(); }
bool GameModel::isGameOver() const { return m_gameOver; }
bool GameModel::isGameWon() const { return m_gameWon; }

void GameModel::resetBoard() {
    m_board = QVector<QVector<int>>(SIZE, QVector<int>(SIZE, 0));
}

void GameModel::spawnRandomTile() {
    QVector<QPair<int,int>> empty;
    for (int r=0;r<SIZE;++r)
        for (int c=0;c<SIZE;++c)
            if (m_board[r][c] == 0) empty.append({r,c});
    if (empty.isEmpty()) return;
    int idx = QRandomGenerator::global()->bounded(empty.size());
    auto [r,c] = empty[idx];
    int val = (QRandomGenerator::global()->bounded(100) < 90) ? 2 : 4;
    m_board[r][c] = val;
}

void GameModel::updateBestScore() {
    if (m_score > m_bestScore) {
        m_bestScore = m_score;
        m_settings.setValue("bestScore", m_bestScore);
    }
}

void GameModel::checkGameState() {
    m_gameOver = !anyMovesAvailable();
    // game won when 2048 exists
    m_gameWon = false;
    for (int r=0;r<SIZE;++r)
        for (int c=0;c<SIZE;++c)
            if (m_board[r][c] >= 2048) m_gameWon = true;

    if (m_gameWon) emit gameWon();
    if (m_gameOver) emit gameOver();
}

bool GameModel::hasEmptyCell() const {
    for (int r=0;r<SIZE;++r)
        for (int c=0;c<SIZE;++c)
            if (m_board[r][c] == 0) return true;
    return false;
}

bool GameModel::anyMovesAvailable() const {
    if (hasEmptyCell()) return true;
    // Check horizontal/vertical neighbors
    for (int r=0;r<SIZE;++r) {
        for (int c=0;c<SIZE-1;++c) {
            if (m_board[r][c] == m_board[r][c+1]) return true;
        }
    }
    for (int c=0;c<SIZE;++c) {
        for (int r=0;r<SIZE-1;++r) {
            if (m_board[r][c] == m_board[r+1][c]) return true;
        }
    }
    return false;
}

bool GameModel::collapseLeft(QVector<int>& row, int& rowScore) {
    // Move non-zero to left
    QVector<int> vals;
    for (int v : row) if (v != 0) vals.append(v);

    bool changed = (vals.size() != row.size());
    // Merge with merge-once rule
    QVector<int> merged;
    for (int i = 0; i < vals.size();) {
        if (i+1 < vals.size() && vals[i] == vals[i+1]) {
            int sum = vals[i] * 2;
            merged.append(sum);
            rowScore += sum;
            i += 2;
            changed = true;
        } else {
            merged.append(vals[i]);
            i += 1;
        }
    }
    while (merged.size() < row.size()) merged.append(0);
    if (row != merged) changed = true;
    row = merged;
    return changed;
}

bool GameModel::slideLeft() {
    bool changed = false;
    int gained = 0;
    for (int r=0;r<SIZE;++r) {
        QVector<int> row = m_board[r];
        if (collapseLeft(row, gained)) changed = true;
        m_board[r] = row;
    }
    if (changed) {
        m_prevScore = m_score;
        m_score += gained;
        updateBestScore();
    }
    return changed;
}

bool GameModel::moveLeft() {
    if (m_gameOver) return false;
    m_prevBoard = m_board;
    bool moved = slideLeft();
    if (moved) {
        spawnRandomTile();
        checkGameState();
        emit boardChanged(m_board);
        emit scoreChanged(m_score, m_bestScore);
        m_settings.setValue("lastBoard", QVariant::fromValue(QVariantList{
            QVariantList{m_board[0][0],m_board[0][1],m_board[0][2],m_board[0][3]},
            QVariantList{m_board[1][0],m_board[1][1],m_board[1][2],m_board[1][3]},
            QVariantList{m_board[2][0],m_board[2][1],m_board[2][2],m_board[2][3]},
            QVariantList{m_board[3][0],m_board[3][1],m_board[3][2],m_board[3][3]}
        }));
        m_settings.setValue("lastScore", m_score);
    }
    return moved;
}

bool GameModel::moveRight() {
    if (m_gameOver) return false;
    m_prevBoard = m_board;
    // reverse rows, slideLeft, reverse back
    for (int r=0;r<SIZE;++r) std::reverse(m_board[r].begin(), m_board[r].end());
    bool moved = slideLeft();
    for (int r=0;r<SIZE;++r) std::reverse(m_board[r].begin(), m_board[r].end());
    if (moved) {
        spawnRandomTile();
        checkGameState();
        emit boardChanged(m_board);
        emit scoreChanged(m_score, m_bestScore);
        m_settings.setValue("lastBoard", QVariant::fromValue(QVariantList{
            QVariantList{m_board[0][0],m_board[0][1],m_board[0][2],m_board[0][3]},
            QVariantList{m_board[1][0],m_board[1][1],m_board[1][2],m_board[1][3]},
            QVariantList{m_board[2][0],m_board[2][1],m_board[2][2],m_board[2][3]},
            QVariantList{m_board[3][0],m_board[3][1],m_board[3][2],m_board[3][3]}
        }));
        m_settings.setValue("lastScore", m_score);
    }
    return moved;
}

bool GameModel::moveUp() {
    if (m_gameOver) return false;
    m_prevBoard = m_board;
    // transpose to use slideLeft
    QVector<QVector<int>> t(SIZE, QVector<int>(SIZE, 0));
    for (int r=0;r<SIZE;++r) for (int c=0;c<SIZE;++c) t[c][r] = m_board[r][c];
    m_board = t;
    bool moved = slideLeft();
    // transpose back
    QVector<QVector<int>> b(SIZE, QVector<int>(SIZE, 0));
    for (int r=0;r<SIZE;++r) for (int c=0;c<SIZE;++c) b[c][r] = m_board[r][c];
    m_board = b;

    if (moved) {
        spawnRandomTile();
        checkGameState();
        emit boardChanged(m_board);
        emit scoreChanged(m_score, m_bestScore);
        m_settings.setValue("lastBoard", QVariant::fromValue(QVariantList{
            QVariantList{m_board[0][0],m_board[0][1],m_board[0][2],m_board[0][3]},
            QVariantList{m_board[1][0],m_board[1][1],m_board[1][2],m_board[1][3]},
            QVariantList{m_board[2][0],m_board[2][1],m_board[2][2],m_board[2][3]},
            QVariantList{m_board[3][0],m_board[3][1],m_board[3][2],m_board[3][3]}
        }));
        m_settings.setValue("lastScore", m_score);
    }
    return moved;
}

bool GameModel::moveDown() {
    if (m_gameOver) return false;
    m_prevBoard = m_board;
    // transpose, reverse rows, slideLeft, reverse, transpose back
    QVector<QVector<int>> t(SIZE, QVector<int>(SIZE, 0));
    for (int r=0;r<SIZE;++r) for (int c=0;c<SIZE;++c) t[c][r] = m_board[r][c];
    m_board = t;
    for (int r=0;r<SIZE;++r) std::reverse(m_board[r].begin(), m_board[r].end());
    bool moved = slideLeft();
    for (int r=0;r<SIZE;++r) std::reverse(m_board[r].begin(), m_board[r].end());
    QVector<QVector<int>> b(SIZE, QVector<int>(SIZE, 0));
    for (int r=0;r<SIZE;++r) for (int c=0;c<SIZE;++c) b[c][r] = m_board[r][c];
    m_board = b;

    if (moved) {
        spawnRandomTile();
        checkGameState();
        emit boardChanged(m_board);
        emit scoreChanged(m_score, m_bestScore);
        m_settings.setValue("lastBoard", QVariant::fromValue(QVariantList{
            QVariantList{m_board[0][0],m_board[0][1],m_board[0][2],m_board[0][3]},
            QVariantList{m_board[1][0],m_board[1][1],m_board[1][2],m_board[1][3]},
            QVariantList{m_board[2][0],m_board[2][1],m_board[2][2],m_board[2][3]},
            QVariantList{m_board[3][0],m_board[3][1],m_board[3][2],m_board[3][3]}
        }));
        m_settings.setValue("lastScore", m_score);
    }
    return moved;
}

void GameModel::newGame() {
    m_prevBoard = QVector<QVector<int>>();
    m_prevScore = 0;
    m_score = 0;
    resetBoard();
    spawnRandomTile();
    spawnRandomTile();
    m_gameOver = false;
    m_gameWon = false;
    emit boardChanged(m_board);
    emit scoreChanged(m_score, m_bestScore);
    m_settings.setValue("lastBoard", QVariant()); // clear
    m_settings.setValue("lastScore", 0);
}

bool GameModel::undo() {
    if (m_prevBoard.isEmpty()) return false;
    m_board = m_prevBoard;
    m_score = m_prevScore;
    m_gameOver = false;
    emit boardChanged(m_board);
    emit scoreChanged(m_score, m_bestScore);
    return true;
}
