#ifndef LEVEL3_H
#define LEVEL3_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "Heart.h"
#include <QPushButton>

#include<flyingenemy.h>
class Level3: public QGraphicsScene
{
public:
    Level3(int score,int health,QObject *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void generateRandomCoins(int numberOfCoins);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void triggerNewScreen();
    void triggerEndScreen();
    void showGameOverMessage(const QString &message);
    void clearEnemiesAndCoins();
    void generateEnemies();
    void generateCoins();
    void generatePipes();
    void moveItemsWithPlayer();
    // Check for collisions with coins
    void handleCoinCollisions();
    // Check for collisions with enemies (mushrooms)
    void handleEnemyCollisions();
     void addHearts();
     void triggerGameOver();
    QList<FlyingEnemy *> flyingEnemies;
      QGraphicsProxyWidget *restartButtonProxy;
     QPushButton *restartButton;


public slots:
    void updateScene();
    void loadLevel3();
signals:
    void goToLevel4Triggered();
      void restartRequested();
private:
    bool newScreenTriggered;
    Player *player;
    QList<Coin *> coins;
    QList<Enemy *> enemies;
    QList<QGraphicsPixmapItem*> hearts; // List to store the heart items
int healthCount;
    QGraphicsPixmapItem *background;
 void updateHealthDisplay();
    int score;                  // Variable to track the score
    QGraphicsTextItem *scoreText;  // Text item to display the score
    QTimer *timer;

    void updateScore(int change); // Function to update the score
};

#endif // LEVEL3_H
