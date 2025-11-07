#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QKeyEvent>

class GameModel;

/**
 * PUBLIC_INTERFACE
 * GameController wires user input to GameModel and re-emits signals for the UI.
 */
class GameController : public QObject {
    Q_OBJECT
public:
    explicit GameController(GameModel* model, QObject* parent = nullptr);

signals:
    void boardUpdated(const QVector<QVector<int>>& board);
    void scoresUpdated(int score, int best);
    void showGameOver();
    void showGameWon();

public slots:
    // PUBLIC_INTERFACE
    void handleKey(int key);
    // PUBLIC_INTERFACE
    void newGame();
    // PUBLIC_INTERFACE
    void undo();

private:
    GameModel* m_model;
};

#endif // GAMECONTROLLER_H
