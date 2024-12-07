#ifndef GAMESCENE_H
#define GAMESCENE_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QPushButton>
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "Heart.h"
class GameScene : public QGraphicsScene{
    Q_OBJECT
public:
    GameScene(QObject *parent = nullptr);
    int getScore() const { return score; }
    int getHealth() const { return health; }
    void paintEvent(QPaintEvent *event) ;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void generateRandomCoins(int numberOfCoins);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void triggerNewScreen();
    void  triggerEndScreen();
    void showGameOverMessage(const QString &message);
   void clearEnemiesAndCoins();
    void generateEnemies();
    void generateCoins();
    void addHearts();
    void moveItemsWithPlayer();
    // Check for collisions with coins
    void handleCoinCollisions();
    // Check for collisions with enemies (mushrooms)
    void handleEnemyCollisions();
    void triggerGameOver();
    void resetAndUpdateScene();
signals:
    void goToLevel2Triggered();
    void restartRequested();
public slots:
    void updateScene();
private:

    bool newScreenTriggered;
    Player *player;
    QList<Coin *> coins;
    QList<Enemy *> enemies;

    QList<QGraphicsPixmapItem*> hearts; // List to store the heart items
    QGraphicsPixmapItem *background;
    int score;
    int health;
    QGraphicsTextItem *scoreText;  // Text item to display the score
    QGraphicsTextItem *healthText;

    void updateScore(int change); // Function to update the score
    void updateHealthDisplay();
    QGraphicsRectItem *gameOverOverlay;
    QGraphicsTextItem *gameOverText;
    QGraphicsProxyWidget *restartButtonProxy;
    QPushButton *restartButton;

};

#endif // GAMESCENE_H
