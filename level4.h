#ifndef LEVEL4_H
#define LEVEL4_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "Heart.h"
#include <QPushButton>

#include "fallingRock.h"

class Level4 : public QGraphicsScene {
    Q_OBJECT

public:
    Level4(int score,int health,QObject *parent = nullptr);
    void paintEvent(QPaintEvent *event) ;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void generateRandomCoins(int numberOfCoins);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void triggerNewScreen();
    void  triggerEndScreen();
    void showGameOverMessage(const QString &message);
    void triggerGameOver();

 void addHearts();

    void clearEnemiesAndCoins();
    void generateEnemies();
    void generateRocks();
    void generateCoins();

    void moveItemsWithPlayer();


    // Check for collisions with coins
    void handleCoinCollisions();

    // Check for collisions with enemies (mushrooms)
    void handleEnemyCollisions();
signals:
       void goToLevel5Triggered();
             void restartRequested();

public slots:
    void updateScene();

private:
    bool newScreenTriggered;
    Player *player;
    QList<Coin *> coins;
    QList<Enemy *> enemies;
    QList<fallingRock*> rocks;
    QGraphicsPixmapItem *background;
    QList<QGraphicsPixmapItem*> hearts; // List to store the heart items
    int healthCount;
    QPushButton *restartButton;
     QGraphicsProxyWidget *restartButtonProxy;


 void updateHealthDisplay();
    int score;                  // Variable to track the score
    QGraphicsTextItem *scoreText;  // Text item to display the score

    void updateScore(int change); // Function to update the score
};

#endif // GAMESCENE_H

