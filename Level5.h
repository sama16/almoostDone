#ifndef LEVEL5_H
#define LEVEL5_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "FlyingEnemy.h"
#include "fallingRock.h"
#include <QPushButton>

class Level5 : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Level5(int socre,int healthCount, QObject *parent = nullptr);

signals:
    void levelComplete(int finalScore);

private:
    Player *player;
    QGraphicsPixmapItem *background;
    QGraphicsTextItem *scoreText;
    QList<Coin *> coins;
    QList<Enemy *> enemies;
    QList<FlyingEnemy *> flyingEnemies;
    QList<fallingRock *> rocks;
    int score;
    int healthCount;
    bool newScreenTriggered;
    QPushButton *restartButton;
    QGraphicsProxyWidget *restartButtonProxy;


    void generateCoins();
    void generateEnemies();
    void generateFlyingEnemies();
    void generateRocks();
    void clearScene();
    void updateScore(int change);

private slots:
    void updateScene();
    void triggerNewScreen();
    void checkLevelEnd();
};

#endif // LEVEL5_H
