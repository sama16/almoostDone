#include "level2.h"
#include "GameScene.h"
#include "WelcomeWindow.h"
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include "Level3.h"
Level2::Level2(int score,int health,QObject *parent)
    : QGraphicsScene(parent), newScreenTriggered(false) {
    this->score=score;
    this->healthCount=health;
    // Set scene size to match the background size
    QPixmap bgPixmap(":/images/bg2.png");
    setSceneRect(0, 0, bgPixmap.width(), bgPixmap.height());
    // Create the background item and add it to the scene
    background = new QGraphicsPixmapItem(bgPixmap);
    addItem(background);
    background->setPos(0, 0); // Set background position to (0, 0)
    // Add the player
    player = new Player();
    addItem(player);
    player->setPosition(100, 380);
 // Add score display
    scoreText = new QGraphicsTextItem("Score: "+QString::number(score));
    scoreText->setDefaultTextColor(Qt::black);
    scoreText->setFont(QFont("Arial", 16));
    scoreText->setPos(30, 30); // Top-left corner
    addItem(scoreText);
    // Add health display

    // Generate enemies and coins
    generateEnemies();
    generateCoins();
    addHearts();
    // Set up a timer to update the scene periodically
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level2::updateScene);
    timer->start(15);
    updateScene();
    restartButton = new QPushButton("Restart Game");
    restartButton->setFont(QFont("Arial", 16));
    restartButton->setFixedSize(150, 50);
    restartButton->setVisible(false);
}
void Level2::drawBackground(QPainter *painter, const QRectF &rect) {
    QPixmap backgroundPixmap(":/images/back.jpg"); // Path to background image
    painter->drawPixmap(0, 0, this->sceneRect().width(), this->sceneRect().height(), backgroundPixmap);
}

void Level2::keyPressEvent(QKeyEvent *event) {
    player->handleInput(event, true); // true means the key is pressed
}

void Level2::keyReleaseEvent(QKeyEvent *event) {
    player->handleInput(event, false); // false means the key is released
}

void Level2::triggerNewScreen() {
    QPixmap bgPixmap2(":/images/bg3.png");
    setSceneRect(0, 0, bgPixmap2.width(), bgPixmap2.height());
    background->setPixmap(bgPixmap2);
    background->setPos(0, 0);

    // Reset player position
    player->reset();
    player->update();
    player->setPosition(0, 380);

    // Optionally regenerate coins and enemies
    clearEnemiesAndCoins();
    generateEnemies();
    generateCoins();
      addHearts();
}

void Level2::triggerEndScreen() {
    QPixmap bgPixmap3(":/images/end.jpg");

    // Scale and set up the end screen background
    QPixmap scaledBgPixmap = bgPixmap3.scaled(sceneRect().width(), sceneRect().height(), Qt::KeepAspectRatioByExpanding);
    setSceneRect(0, 0, scaledBgPixmap.width(), scaledBgPixmap.height());
    background->setPixmap(scaledBgPixmap);
    background->setPos(0, 0);

    // Reset player position
    player->setPosition(450, 290);
    player->stopMoving();

    clearEnemiesAndCoins();

    // Add overlay and game over message
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
    // QPushButton *level3Button = new QPushButton("Go to Level 3");
    // level3Button->setFont(QFont("Arial", 16));
    // level3Button->setFixedSize(150, 50);

    // // Connect the button's click signal to a slot for transitioning to level 2
    // connect(level3Button, &QPushButton::clicked, this, &Level2::goToLevel3);



    // QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    // proxyWidget->setWidget(level3Button);
    // proxyWidget->setZValue(2);
    // proxyWidget->setPos(sceneRect().width() / 2 - level3Button->width() / 2,
    //                     sceneRect().height() / 2 - level3Button->height() / 2 + 50);
    // addItem(proxyWidget);

    QPushButton *level3Button = new QPushButton("Go to Level 3");
    level3Button->setFont(QFont("Arial", 16));
    level3Button->setFixedSize(150, 50);

    // Connect the button's click signal to a slot or signal
    connect(level3Button, &QPushButton::clicked, this, &Level2::goToLevel3Triggered);

    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(level3Button);
    proxyWidget->setZValue(2);
    proxyWidget->setPos(sceneRect().width() / 2 - level3Button->width() / 2,
                        sceneRect().height() / 2 - level3Button->height() / 2 + 50);
    addItem(proxyWidget);
}
void Level2::goToLevel3Triggered() {
    // Create Level 3 scene
    Level3 *level3 = new Level3(score,healthCount, this);

    // Get the view that is displaying the current scene
    QList<QGraphicsView *> views = this->views();
    if (!views.isEmpty()) {
        QGraphicsView *view = views.first();

        // Set the new scene in the same view
        view->setScene(level3);

        // Optionally delete this scene to free memory

    } else {
        qDebug() << "No view is associated with the current scene!";
    }
}


void Level2::updateScene() {
    player->update();

    for (Coin *coin : coins) {
        coin->setPos(coin->x() - player->getVelocityX(), coin->y());
    }

    for (Enemy *enemy : enemies) {
        enemy->update();
    }

    for (Coin *coin : coins) {
        if (player->collidesWithItem(coin)) {
            updateScore(10);
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

// void Level2::showGameOverMessage(const QString &message) {
//     QGraphicsRectItem *overlay = new QGraphicsRectItem();
//     overlay->setRect(sceneRect());
//     overlay->setBrush(QColor(0, 0, 0, 150));
//     overlay->setZValue(1);
//     addItem(overlay);

//     QGraphicsTextItem *textItem = new QGraphicsTextItem(message);
//     textItem->setDefaultTextColor(Qt::white);
//     textItem->setFont(QFont("Arial", 24, QFont::Bold));
//     textItem->setZValue(2);
//     textItem->setPos(sceneRect().width() / 2 - textItem->boundingRect().width() / 2,
//                      sceneRect().height() / 2 - textItem->boundingRect().height() / 2);
//     addItem(textItem);
// }

void Level2::clearEnemiesAndCoins() {
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
}

void Level2::generateEnemies() {
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
void Level2::addHearts() {
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

void Level2::updateHealthDisplay() {
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

void Level2::generateCoins() {
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


void Level2::handleEnemyCollisions() {
    // Check for collisions with enemies (mushrooms)

    for (Enemy *enemy : enemies) {

        if (player->collidesWithItem(enemy) && player->getVelocityY() > 0) {
            // Mario defeats the enemy by jumping on top
            updateScore(10);
            removeItem(enemy);
            enemies.removeOne(enemy);
            delete enemy;
            break;  // Exit loop since this enemy is handled
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

            qDebug() << "Player hit by enemy!";
        }
    }
}
void Level2::triggerGameOver() {
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
    connect(restartButton, &QPushButton::clicked, this, &Level2::restartRequested);
    // Add restart button to scene using QGraphicsProxyWidget
    if (!restartButtonProxy) {
        restartButtonProxy = new QGraphicsProxyWidget();
        restartButtonProxy->setWidget(restartButton);
        restartButtonProxy->setZValue(2);
        restartButtonProxy->setPos(sceneRect().width() / 2 - restartButton->width() / 2,
                                   sceneRect().height() / 2 - restartButton->height() / 2 + 50);
        addItem(restartButtonProxy);
    }
}
void Level2::restartRequested(){
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

void Level2::updateScore(int change) {
    score += change;
    // Update the score display
    scoreText->setPlainText(QString("Score: %1").arg(score));
}
