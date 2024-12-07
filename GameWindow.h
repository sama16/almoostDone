#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include "GameScene.h"
#include"WelcomeWindow.h"
#include"level2.h"
#include"level3.h"
class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    // void loadLevel2();
      // void loadLevel3();
    void restartAgain();
    void startGame();
    void startAgain();

private:


    GameScene *scene;
    WelcomeWindow *welcome;
    Level2 *level2Scene;
    Level3 *level3Scene;
    QGraphicsView *view;
    QTimer *timer;
     QTimer *level2Timer;
};

#endif // GAMEWINDOW_H
