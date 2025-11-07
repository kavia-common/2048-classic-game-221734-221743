#include "GameView.h"
#include "GameController.h"
#include "GameModel.h"
#include "Theme.h"
#include "GridWidget.h"
#include "ScorePanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>

GameView::GameView(QWidget* parent)
    : QMainWindow(parent)
{
    m_model = new GameModel(this);
    m_controller = new GameController(m_model, this);
    buildUi();
    applyWindowStyle();

    // initial state
    updateBoard(m_model->board());
    updateScores(m_model->score(), m_model->bestScore());

    // wiring
    connect(m_controller, &GameController::boardUpdated, this, &GameView::updateBoard);
    connect(m_controller, &GameController::scoresUpdated, this, &GameView::updateScores);
    connect(m_controller, &GameController::showGameOver, this, &GameView::onGameOver);
    connect(m_controller, &GameController::showGameWon, this, &GameView::onGameWon);
}

GameView::~GameView() {}

void GameView::applyWindowStyle() {
    setWindowTitle("2048");
    setMinimumSize(480, 640);
    auto pal = palette();
    pal.setColor(QPalette::Window, Theme::background());
    pal.setColor(QPalette::WindowText, Theme::text());
    setPalette(pal);
}

void GameView::buildUi() {
    m_central = new QWidget(this);
    setCentralWidget(m_central);

    auto* root = new QVBoxLayout(m_central);
    root->setContentsMargins(16,16,16,16);
    root->setSpacing(16);

    // Header
    auto* header = new QWidget(m_central);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0,0,0,0);
    headerLayout->setSpacing(12);

    auto* title = new QLabel("2048", header);
    title->setFont(Theme::titleFont());
    title->setStyleSheet(QString("QLabel{color:%1;}").arg(Theme::primary().name()));

    m_scores = new ScorePanel(header);

    m_newGameBtn = new QPushButton("New Game", header);
    m_newGameBtn->setCursor(Qt::PointingHandCursor);
    m_newGameBtn->setStyleSheet(QString(
        "QPushButton{background:%1;color:#fff;border:none;border-radius:%2px;padding:10px 16px;font-weight:bold;}"
        "QPushButton:hover{background:%3;}"
        ).arg(Theme::secondary().name())
         .arg(Theme::cornerRadius())
         .arg(Theme::primary().name()));
    connect(m_newGameBtn, &QPushButton::clicked, m_controller, &GameController::newGame);

    headerLayout->addWidget(title);
    headerLayout->addStretch(1);
    headerLayout->addWidget(m_scores);
    headerLayout->addWidget(m_newGameBtn);
    header->setLayout(headerLayout);

    // Grid
    m_grid = new GridWidget(m_central);

    // Footer hint
    auto* footer = new QLabel("Use Arrow Keys / WASD to play. Undo: Ctrl+Z. New Game resets.", m_central);
    footer->setStyleSheet(QString("QLabel{color:%1;}").arg(Theme::text().name()));

    root->addWidget(header);
    root->addWidget(m_grid, 1);
    root->addWidget(footer);
}

void GameView::keyPressEvent(QKeyEvent* event) {
    if (event->matches(QKeySequence::Undo) || (event->modifiers() & Qt::ControlModifier && event->key()==Qt::Key_Z)) {
        m_controller->undo();
        return;
    }
    m_controller->handleKey(event->key());
    QMainWindow::keyPressEvent(event);
}

void GameView::mousePressEvent(QMouseEvent* event) {
    m_swipeStart = event->pos();
    QMainWindow::mousePressEvent(event);
}

void GameView::mouseReleaseEvent(QMouseEvent* event) {
    QPoint d = event->pos() - m_swipeStart;
    int threshold = 30;
    if (d.manhattanLength() > threshold) {
        if (abs(d.x()) > abs(d.y())) {
            m_controller->handleKey(d.x() > 0 ? Qt::Key_Right : Qt::Key_Left);
        } else {
            m_controller->handleKey(d.y() > 0 ? Qt::Key_Down : Qt::Key_Up);
        }
    }
    QMainWindow::mouseReleaseEvent(event);
}

void GameView::updateBoard(const QVector<QVector<int>>& board) {
    m_grid->setBoard(board, true);
}

void GameView::updateScores(int score, int best) {
    m_scores->setScores(score, best);
}

void GameView::onGameOver() {
    QMessageBox::information(this, "Game Over", "No more valid moves. Try a new game!");
}

void GameView::onGameWon() {
    // Show once; allow continue playing
    static bool shown = false;
    if (!shown) {
        shown = true;
        QMessageBox::information(this, "You Win!", "You've reached 2048! You can continue to play.");
    }
}
