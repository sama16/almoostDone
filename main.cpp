#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "WelcomeWindow.h"
#include "GameWindow.h"  // Include your game window header here

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the welcome window
    WelcomeWindow *welcomeWindow = new WelcomeWindow();

    // Connect the signal for starting the game
    QObject::connect(welcomeWindow, &WelcomeWindow::startGameClicked, [&]() {
        // Hide the welcome window and show the game window
        welcomeWindow->hide();

        // Create and show the game window
        GameWindow *gameWindow = new GameWindow();
        gameWindow->show();
    });

    // Show the welcome window
    welcomeWindow->show();

    return app.exec();
}
