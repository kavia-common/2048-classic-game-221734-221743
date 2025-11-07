#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QVector>
#include <QPair>
#include <QSettings>

/**
 * PUBLIC_INTERFACE
 * GameModel implements 2048 rules, board state, score, best score persistence,
 * and provides move operations with merge-once rule and random spawns.
 */
class GameModel : public QObject {
    Q_OBJECT
public:
    explicit GameModel(QObject* parent = nullptr);

    // PUBLIC_INTERFACE
    Q_INVOKABLE QVector<QVector<int>> board() const;
    // PUBLIC_INTERFACE
    Q_INVOKABLE int score() const;
    // PUBLIC_INTERFACE
    Q_INVOKABLE int bestScore() const;
    // PUBLIC_INTERFACE
    Q_INVOKABLE bool canMove() const;
    // PUBLIC_INTERFACE
    Q_INVOKABLE bool isGameOver() const;
    // PUBLIC_INTERFACE
    Q_INVOKABLE bool isGameWon() const;

signals:
    // Emitted when the board changes; includes list of moves (from->to) and merges for animations.
    void boardChanged(const QVector<QVector<int>>& newBoard);
    void scoreChanged(int score, int best);
    void gameOver();
    void gameWon();

public slots:
    // PUBLIC_INTERFACE
    void newGame();
    // PUBLIC_INTERFACE
    bool moveUp();
    // PUBLIC_INTERFACE
    bool moveDown();
    // PUBLIC_INTERFACE
    bool moveLeft();
    // PUBLIC_INTERFACE
    bool moveRight();
    // PUBLIC_INTERFACE
    bool undo();

private:
    QVector<QVector<int>> m_board;
    QVector<QVector<int>> m_prevBoard;
    int m_score = 0;
    int m_prevScore = 0;
    int m_bestScore = 0;
    bool m_gameOver = false;
    bool m_gameWon = false;

    QSettings m_settings;

    void resetBoard();
    void spawnRandomTile();
    bool collapseLeft(QVector<int>& row, int& rowScore);
    bool slideLeft(); // returns true if any tile moved or merged
    void updateBestScore();
    void checkGameState();
    bool anyMovesAvailable() const;
    bool hasEmptyCell() const;
};

#endif // GAMEMODEL_H
