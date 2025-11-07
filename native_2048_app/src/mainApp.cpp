#include "mainApp.h"
#include "GameView.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GameView window;
    window.show();

    return app.exec();
}
