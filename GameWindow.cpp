#include "GameWindow.h"
#include <QGraphicsView>
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Create the game scene
    scene = new GameScene(this);
    level2Scene=new Level2(scene->getScore(),scene->getHealth(),this);
    // Create the view to display the scene
    view = new QGraphicsView(scene, this);
    // Set the fixed size of the view to match the scene (background size)
    view->setFixedSize(scene->sceneRect().width(), scene->sceneRect().height());
    // Set the view as the central widget
    setCentralWidget(view);
    // connect(scene, &GameScene::level2Requested, this, &GameWindow::loadLevel2);
    // connect(level2Scene, &Level2::level3Requested, this, &GameWindow::loadLevel3);
    welcome = new WelcomeWindow();
    connect(scene, &GameScene::restartRequested, this, &GameWindow::restartAgain);
    connect(welcome, &WelcomeWindow::startGameClicked, this,&GameWindow::startAgain);

}
// void GameWindow::loadLevel2() {
//     qDebug() << "Loading Level 2...";
//         int level1Score = scene->getScore();
//     int healthCount=scene->getHealth();
//         level2Scene = new Level2(level1Score,healthCount,this);
//     // Set Level 2 scene to the view
//     view->setScene(level2Scene);
// }
// void GameWindow::loadLevel3() {
//     int level3Score = scene->getScore(); // Use level2Scene to fetch the updated score
//     level3Scene = new Level3(level3Score, this); // Create Level 3 scene
//     view->setScene(level3Scene); // Set Level 3 as the active scene
//     qDebug() << "Level 3 set as the active scene.";
// }

void GameWindow::restartAgain(){
    this->hide();
    welcome->show();
}
void GameWindow::startAgain(){
    welcome->hide();         // Hide the WelcomeWindow
      // Update the game scene for a fresh start
    this->show();
}

