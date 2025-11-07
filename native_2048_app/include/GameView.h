#ifndef GAMEVIEW_H
#define GAMEVIEW_H

/*
 Core Qt includes for this class; keep explicit to prevent MOC/type errors.
 Avoid forward declarations for QObject-derived members to ensure MOC and
 compiler see complete types in the header.
*/
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTimer>
#include <QPoint>
#include <QVector>
#include <QKeyEvent>
#include <QMouseEvent>

class GridWidget;
class ScorePanel;
class GameController;
class GameModel;

/**
 * PUBLIC_INTERFACE
 * GameView is the main window that composes header (title, scores, New Game),
 * the grid, and a footer hint. It captures keyboard and basic swipe input.
 */
class GameView : public QMainWindow {
    Q_OBJECT
public:
    explicit GameView(QWidget* parent = nullptr);
    ~GameView();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private slots:
    void updateBoard(const QVector<QVector<int>>& board);
    void updateScores(int score, int best);
    void onGameOver();
    void onGameWon();

private:
    QWidget* m_central = nullptr;
    GridWidget* m_grid = nullptr;
    ScorePanel* m_scores = nullptr;
    QPushButton* m_newGameBtn = nullptr;

    GameModel* m_model = nullptr;
    GameController* m_controller = nullptr;

    QPoint m_swipeStart;

    void buildUi();
    void applyWindowStyle();
};

#endif // GAMEVIEW_H
