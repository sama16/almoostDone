#include "Level5.h"
#include "fallingRock.h"
#include <QRandomGenerator>
#include <QFont>
#include <QDebug>

// Constructor
Level5::Level5(int score,int health, QObject *parent)
    : QGraphicsScene(parent), newScreenTriggered(false) {
    // Set the background
    this->score=score;
    this->healthCount=health;
    QPixmap bgPixmap(":/images/bg_combined.png"); // Use a suitable background image
    setSceneRect(0, 0, bgPixmap.width(), bgPixmap.height());
    background = new QGraphicsPixmapItem(bgPixmap);
    addItem(background);

    // Add the player
    player = new Player();
    addItem(player);
    player->setPosition(100, 380);

    // Add score display
    scoreText = new QGraphicsTextItem(QString("Score: %1").arg(score));
    scoreText->setDefaultTextColor(Qt::black);
    scoreText->setFont(QFont("Arial", 16));
    scoreText->setPos(30, 30);
    addItem(scoreText);

    // Generate all features for the level
    generateCoins();
    generateEnemies();
    generateFlyingEnemies();
    generateRocks();

    // Timer to update the scene
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level5::updateScene);
    timer->start(16); // ~60 FPS
}

void Level5::generateCoins() {
    for (int i = 0; i < 15; ++i) {
        int x = QRandomGenerator::global()->bounded(100, 800);
        int y = QRandomGenerator::global()->bounded(100, 300);
        Coin *coin = new Coin(x, y);
        coins.append(coin);
        addItem(coin);
    }
}

void Level5::generateEnemies() {
    for (int i = 0; i < 5; ++i) {
        int x = QRandomGenerator::global()->bounded(100, 800);
        Enemy *enemy = new Enemy(x, 380);
        enemies.append(enemy);
        addItem(enemy);
    }
}

void Level5::generateFlyingEnemies() {
    for (int i = 0; i < 3; ++i) {
        int x = QRandomGenerator::global()->bounded(100, 800);
        int y = QRandomGenerator::global()->bounded(100, 200);
        FlyingEnemy *flyingEnemy = new FlyingEnemy(x, y);
        flyingEnemies.append(flyingEnemy);
        addItem(flyingEnemy);
    }
}

void Level5::generateRocks() {
    for (int i = 0; i < 5; ++i) {
        int y = QRandomGenerator::global()->bounded(100, 800);
        fallingRock *rock = new fallingRock();
        rocks.append(rock);
        addItem(rock);
    }
}

void Level5::updateScene() {
    player->update();

    // Update coins
    for (Coin *coin : coins) {
        coin->setPos(coin->x() - player->getVelocityX(), coin->y());
        if (player->collidesWithItem(coin)) {
            updateScore(5);
            removeItem(coin);
            coins.removeOne(coin);
            delete coin;
            break;
        }
    }

    // Update ground enemies
    for (Enemy *enemy : enemies) {
        enemy->update();
        if (player->collidesWithItem(enemy) && player->getVelocityY() > 0) {
            updateScore(10);
            removeItem(enemy);
            enemies.removeOne(enemy);
            delete enemy;
            break;
        } else if (player->collidesWithItem(enemy)) {
            updateScore(-10);
            break;
        }
    }

    // Update flying enemies
    for (FlyingEnemy *flyingEnemy : flyingEnemies) {
        flyingEnemy->update();
        if (player->collidesWithItem(flyingEnemy)) {
            updateScore(-10);
            break;
        }
    }

    // Update falling rocks
    for (fallingRock *rock : rocks) {
        rock->update();
        if (player->collidesWithItem(rock)) {
            updateScore(-10);
            break;
        }
    }

    // Trigger new screen logic
    if (!newScreenTriggered && player->x() >= sceneRect().width()) {
        newScreenTriggered = true;
        triggerNewScreen();
    }

    // Check if level is complete
    checkLevelEnd();
}

void Level5::triggerNewScreen() {
    clearScene();
    generateCoins();
    generateEnemies();
    generateFlyingEnemies();
    generateRocks();
}

void Level5::checkLevelEnd() {
    if (player->x() >= sceneRect().width() * 2) { // Adjust for level length
        emit levelComplete(score); // Emit signal when level is complete
    }
}

void Level5::clearScene() {
    for (Coin *coin : coins) {
        removeItem(coin);
        delete coin;
    }
    coins.clear();

    for (Enemy *enemy : enemies) {
        removeItem(enemy);
        delete enemy;
    }
    enemies.clear();

    for (FlyingEnemy *flyingEnemy : flyingEnemies) {
        removeItem(flyingEnemy);
        delete flyingEnemy;
    }
    flyingEnemies.clear();

    for (fallingRock *rock : rocks) {
        removeItem(rock);
        delete rock;
    }
    rocks.clear();
}

void Level5::updateScore(int change) {
    score += change;
    if (score < 0) score = 0;
    scoreText->setPlainText(QString("Score: %1").arg(score));
}
