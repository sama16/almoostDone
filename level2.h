#ifndef LEVEL2_H
#define LEVEL2_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QKeyEvent>
#include "Player.h"
#include "Coin.h"
#include "Enemy.h"
#include "Heart.h"

class Level2 : public QGraphicsScene {
    Q_OBJECT

public:
    Level2(int score, int health,QObject *parent = nullptr);
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
    void triggerGameOver();
    void resetAndUpdateScene();
     void addHearts();

public slots:
    void updateScene();
    // void loadLevel2();
    // void emitGoToLevel3Signal();
signals:
    void goToLevel3Triggered();
     void restartRequested();
private:
    bool newScreenTriggered;
    Player *player;
    QList<Coin *> coins;
    QList<Enemy *> enemies;
    QGraphicsPixmapItem *background;
    QGraphicsTextItem *health;
    QList<QGraphicsPixmapItem*> hearts; // List to store the heart items
    QGraphicsProxyWidget *restartButtonProxy;
    QPushButton *restartButton;


 void updateHealthDisplay();
    int score;
int healthCount;     // Variable to track the score
    QGraphicsTextItem *scoreText;  // Text item to display the score
    QTimer *timer;

    void updateScore(int change); // Function to update the score
};

#endif // LEVEL2_H
