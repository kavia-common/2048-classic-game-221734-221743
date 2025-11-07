#include "GameController.h"
#include "GameModel.h"
#include <QKeyEvent>

GameController::GameController(GameModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
    QObject::connect(m_model, &GameModel::boardChanged, this, &GameController::boardUpdated);
    QObject::connect(m_model, &GameModel::scoreChanged, this, &GameController::scoresUpdated);
    QObject::connect(m_model, &GameModel::gameOver, this, &GameController::showGameOver);
    QObject::connect(m_model, &GameModel::gameWon, this, &GameController::showGameWon);
}

void GameController::handleKey(int key) {
    switch (key) {
        case Qt::Key_Left:
        case Qt::Key_A:
            m_model->moveLeft(); break;
        case Qt::Key_Right:
        case Qt::Key_D:
            m_model->moveRight(); break;
        case Qt::Key_Up:
        case Qt::Key_W:
            m_model->moveUp(); break;
        case Qt::Key_Down:
        case Qt::Key_S:
            m_model->moveDown(); break;
        default: break;
    }
}

void GameController::newGame() {
    m_model->newGame();
}

void GameController::undo() {
    m_model->undo();
}
