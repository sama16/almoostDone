#include "level4.h"
#include "Level5.h"

#include "GameScene.h"

#include "Coin.h"
#include "Enemy.h"
#include "fallingRock.h"
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QGraphicsProxyWidget>
#include"QPushButton"
#include <QGraphicsView>
Level4::Level4(int score,int health,QObject *parent)
    : QGraphicsScene(parent), newScreenTriggered(false) {
    this->score=score;
    this->healthCount=health;
    QPixmap bgPixmap(":/images/bg2.png");
    setSceneRect(0, 0, bgPixmap.width(), bgPixmap.height());
    background = new QGraphicsPixmapItem(bgPixmap);
    addItem(background);
    background->setPos(0, 0);

    player = new Player();
    addItem(player);
    player->setPosition(100, 380);

    scoreText = new QGraphicsTextItem("Score: 0");
    scoreText->setDefaultTextColor(Qt::black);
    scoreText->setFont(QFont("Arial", 16));
    scoreText->setPos(30, 30);
    addItem(scoreText);

    generateEnemies();
    generateRocks();
    generateCoins();
  addHearts();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level4::updateScene);
    timer->start(12);
    restartButton = new QPushButton("Restart Game");
    restartButton->setFont(QFont("Arial", 16));
    restartButton->setFixedSize(150, 50);
    restartButton->setVisible(false);
    updateScene();
    restartButton = new QPushButton("Restart Game");
    restartButton->setFont(QFont("Arial", 16));
    restartButton->setFixedSize(150, 50);
    restartButton->setVisible(false);
}

void Level4::drawBackground(QPainter *painter, const QRectF &rect) {
    QPixmap backgroundPixmap(":/images/back.jpg");
    painter->drawPixmap(0, 0, this->sceneRect().width(), this->sceneRect().height(), backgroundPixmap);
}

void Level4::keyPressEvent(QKeyEvent *event) {
    player->handleInput(event, true);
}

void Level4::keyReleaseEvent(QKeyEvent *event) {
    player->handleInput(event, false);
}

void Level4::triggerNewScreen() {
    QPixmap bgPixmap2(":/images/bg3.png");
    setSceneRect(0, 0, bgPixmap2.width(), bgPixmap2.height());
    background->setPixmap(bgPixmap2);
    background->setPos(0, 0);

    player->reset();
    player->update();
    player->setPosition(0, 380);

    clearEnemiesAndCoins();
    generateEnemies();
    generateCoins();
    generateRocks();
}

void Level4::triggerEndScreen() {
    QPixmap bgPixmap3(":/images/end.jpg");
    double sceneWidth = sceneRect().width();
    double sceneHeight = sceneRect().height();
    QPixmap scaledBgPixmap = bgPixmap3.scaled(sceneWidth, sceneHeight, Qt::KeepAspectRatioByExpanding);

    setSceneRect(0, 0, scaledBgPixmap.width(), scaledBgPixmap.height());
    background->setPixmap(scaledBgPixmap);
    background->setPos(0, 0);

    player->setPosition(450, 290);
    player->stopMoving();
    clearEnemiesAndCoins();

    QGraphicsRectItem *overlay = new QGraphicsRectItem();
    overlay->setRect(sceneRect());
    overlay->setBrush(QColor(0, 0, 0, 150));
    overlay->setZValue(1);
    addItem(overlay);

    QGraphicsTextItem *textItem = new QGraphicsTextItem("Game Complete! Thank you for playing!");
    textItem->setDefaultTextColor(Qt::white);
    textItem->setFont(QFont("Arial", 24, QFont::Bold));
    textItem->setZValue(2);
    textItem->setPos(sceneRect().width() / 2 - textItem->boundingRect().width() / 2,
                     sceneRect().height() / 2 - textItem->boundingRect().height() / 2);
    addItem(textItem);
    QPushButton *level4Button = new QPushButton("Go to Level 5");
    level4Button->setFont(QFont("Arial", 16));
    level4Button->setFixedSize(150, 50);

    // Connect the button's click signal to a slot or signal
    connect(level4Button, &QPushButton::clicked, this, &Level4::goToLevel5Triggered);

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(level4Button);
    proxyWidget->setZValue(2);
    proxyWidget->setPos(sceneRect().width() / 2 - level4Button->width() / 2,
                        sceneRect().height() / 2 - level4Button->height() / 2 + 50);
    addItem(proxyWidget);
}

void Level4::goToLevel5Triggered() {
    // Create Level 3 scene
    Level5 *level5 = new Level5(score,healthCount, this);

    // Get the view that is displaying the current scene
    QList<QGraphicsView *> views = this->views();
    if (!views.isEmpty()) {
        QGraphicsView *view = views.first();

        // Set the new scene in the same view
        view->setScene(level5);

        // Optionally delete this scene to free memory

    } else {
        qDebug() << "No view is associated with the current scene!";
    }
}


void Level4::updateScene() {
    player->update();

    for (Coin *coin : coins) {
        coin->setPos(coin->x() - player->getVelocityX(), coin->y());
    }

    for (Enemy *enemy : enemies) {
        enemy->update();
    }

    for (fallingRock *rockfall : rocks) {
        rockfall->update();
    }

    for (Coin *coin : coins) {
        if (player->collidesWithItem(coin)) {
            updateScore(1);
            removeItem(coin);
            coins.removeOne(coin);
            delete coin;
            break;
        }
    }

    handleEnemyCollisions();

    if (!newScreenTriggered && player->x() >= sceneRect().width()) {
        newScreenTriggered = true;
        triggerNewScreen();
    }

    if (newScreenTriggered && player->x() >= sceneRect().width()) {
        triggerEndScreen();
    }
}

void Level4::showGameOverMessage(const QString &message) {
    QGraphicsRectItem *overlay = new QGraphicsRectItem();
    overlay->setRect(sceneRect());
    overlay->setBrush(QColor(0, 0, 0, 150));
    overlay->setZValue(1);
    addItem(overlay);

    QGraphicsTextItem *textItem = new QGraphicsTextItem(message);
    textItem->setDefaultTextColor(Qt::white);
    textItem->setFont(QFont("Arial", 24, QFont::Bold));
    textItem->setZValue(2);
    textItem->setPos(sceneRect().width() / 2 - textItem->boundingRect().width() / 2,
                     sceneRect().height() / 2 - textItem->boundingRect().height() / 2);
    addItem(textItem);
}

void Level4::clearEnemiesAndCoins() {
    for (Enemy *enemy : enemies) {
        removeItem(enemy);
        delete enemy;
    }
    enemies.clear();

    for (Coin *coin : coins) {
        removeItem(coin);
        delete coin;
    }
    coins.clear();

    for (fallingRock *rock : rocks) {
        removeItem(rock);
        delete rock;
    }
    rocks.clear();
}
void Level4::addHearts() {
    // Clear any existing hearts
    for (QGraphicsPixmapItem *heart : hearts) {
        removeItem(heart);
        delete heart;
    }
    hearts.clear();

    // Calculate the number of hearts based on the health
    int numHearts = healthCount / 30;  // For example, if health is 300, we show 3 hearts

    // Generate hearts in a horizontal row based on the health
    for (int i = 0; i < numHearts; ++i) {
        QGraphicsPixmapItem *heartItem = new QGraphicsPixmapItem();
        QPixmap heartPixmap("C:/Users/hp/Desktop/csProjectMarioGame/marioGameProject-main/heart.png");
        heartItem->setPixmap(heartPixmap.scaled(30, 30));  // Scale the heart image to 30x30 pixels
        addItem(heartItem);

        // Set the position of each heart, spacing them 30 pixels apart
        heartItem->setPos(30 + i * 30, 60);
        hearts.append(heartItem);  // Store the heart in the hearts list
    }
}

void Level4::updateHealthDisplay() {
    int heartsToShow = healthCount / 30;  // For each 100 health, we show 1 heart

    // Update the hearts display by removing or adding hearts
    int currentHeartsCount = hearts.size();

    if (currentHeartsCount > heartsToShow) {
        // Remove hearts if health decreased
        for (int i = currentHeartsCount - 1; i >= heartsToShow; --i) {
            removeItem(hearts[i]);
            delete hearts[i];
            hearts.removeAt(i);
        }
    } else if (currentHeartsCount < heartsToShow) {
        // Add hearts if health increased
        for (int i = currentHeartsCount; i < heartsToShow; ++i) {
            QGraphicsPixmapItem *heartItem = new QGraphicsPixmapItem();
            QPixmap heartPixmap("C:/Users/hp/Desktop/csProjectMarioGame/marioGameProject-main/heart.png");
            heartItem->setPixmap(heartPixmap.scaled(30, 30));  // Scale the heart image to 30x30 pixels
            addItem(heartItem);

            // Position the new heart
            heartItem->setPos(30 + i * 30, 60);
            hearts.append(heartItem);  // Add the heart to the hearts list
        }
    }
}

void Level4::generateEnemies() {
    for (int i = 0; i < 5; ++i) {
        bool validPosition = false;
        int randomX;

        while (!validPosition) {
            randomX = QRandomGenerator::global()->bounded(100, 800);
            validPosition = true;

            for (Enemy *existingEnemy : enemies) {
                if (qAbs(existingEnemy->x() - randomX) < 100) {
                    validPosition = false;
                    break;
                }
            }
        }

        Enemy *enemy = new Enemy(randomX, 420);
        enemies.append(enemy);
        addItem(enemy);
    }
}

void Level4::generateCoins() {
    for (int i = 0; i < 10; ++i) {
        bool validPosition = false;
        int randomX, randomY;

        while (!validPosition) {
            randomX = QRandomGenerator::global()->bounded(100, 800);
            randomY = QRandomGenerator::global()->bounded(250, 350);

            validPosition = true;

            for (Coin *existingCoin : coins) {
                if (qAbs(existingCoin->x() - randomX) < 50 && qAbs(existingCoin->y() - randomY) < 50) {
                    validPosition = false;
                    break;
                }
            }

            for (Enemy *existingEnemy : enemies) {
                if (qAbs(existingEnemy->x() - randomX) < 50 && qAbs(existingEnemy->y() - randomY) < 50) {
                    validPosition = false;
                    break;
                }
            }
        }

        Coin *coin = new Coin(randomX, randomY);
        coins.append(coin);
        addItem(coin);
    }
}


void Level4::generateRocks() {
    for (int i = 0; i < 3; ++i) {
        fallingRock *rock = new fallingRock();
        rock->setPos(QRandomGenerator::global()->bounded(sceneRect().width()),
                     QRandomGenerator::global()->bounded(sceneRect().height() - 100));
        rocks.append(rock);
        addItem(rock);
    }
}



void Level4::handleEnemyCollisions() {
    for (Enemy *enemy : enemies) {
        if (player->collidesWithItem(enemy) && player->getVelocityY() > 0) {
            updateScore(10);
            removeItem(enemy);
            enemies.removeOne(enemy);
            delete enemy;
            break;
        }

        if (player->collidesWithItem(enemy)) {
            // Decrement health
            if (healthCount > 0) {
                updateHealthDisplay();
                healthCount--;
                qDebug() <<healthCount;

                enemy->setEnabled(false);  // Disable interactions
                qDebug() << "Enemy frozen";

                // Set up a timer to unfreeze the enemy

            }
            else{
                triggerGameOver();
                break;
            }

        }
    }
    for (fallingRock *rock : rocks) {
        if (player->collidesWithItem(rock)) {
            healthCount--;
            updateHealthDisplay();
            qDebug() << "Collided with a falling rock! Health: " << healthCount;

            if (healthCount <= 0) {
                triggerGameOver();
                break;
            }
        }
    }
}
void Level4::triggerGameOver() {
    restartButton->setVisible(true);
    score = 0;
    healthCount=300;
    scoreText->setPlainText("Score: 0");
    // Clear all items (enemies, coins, hearts, etc.)
    clearEnemiesAndCoins();
    // Reset the player and reposition it at the starting point
    player->reset();
    player->setPosition(100, 380);  // Reset to the initial position
    // Reset hearts and health display
    connect(restartButton, &QPushButton::clicked, this, &Level4::restartRequested);
    // Add restart button to scene using QGraphicsProxyWidget

}
void Level4::restartRequested(){
    GameScene *level1 = new GameScene(this);

    // Get the view that is displaying the current scene
    QList<QGraphicsView *> views = this->views();
    if (!views.isEmpty()) {
        QGraphicsView *view = views.first();

        // Set the new scene in the same view
        view->setScene(level1);

        // Optionally delete this scene to free memory

    } else {
        qDebug() << "No view is associated with the current scene!";
    }
}
void Level4::updateScore(int change) {
    score += change;
    // Update the score display
    scoreText->setPlainText(QString("Score: %1").arg(score));
}
