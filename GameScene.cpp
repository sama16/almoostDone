#include "GameScene.h"
#include "level2.h"
#include <QDebug>
#include <QTimer>
#include <QPainter>
#include <QRandomGenerator>
#include<QGraphicsView>
#include <QPushButton>
#include <QGraphicsProxyWidget>
GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent),newScreenTriggered(false),score(0),health(90) {
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
    connect(timer, &QTimer::timeout, this, &GameScene::updateScene);
    timer->start(20);
    updateScene();
    restartButton = new QPushButton("Restart Game");
    restartButton->setFont(QFont("Arial", 16));
    restartButton->setFixedSize(150, 50);
    restartButton->setVisible(false);
}

void GameScene::drawBackground(QPainter *painter, const QRectF &rect) {
    // Load the background image and stretch it to cover the entire scene
    QPixmap backgroundPixmap(":/images/back.jpg");  // Path to background image
    painter->drawPixmap(0, 0, this->sceneRect().width(), this->sceneRect().height(), backgroundPixmap);
}

void GameScene::keyPressEvent(QKeyEvent *event) {
    player->handleInput(event, true);  // true means the key is pressed
}

void GameScene::keyReleaseEvent(QKeyEvent *event) {
    player->handleInput(event, false);  // false means the key is released
}
void GameScene::triggerNewScreen() {

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
void GameScene::triggerEndScreen() {
    // Set up the end screen background
    QPixmap bgPixmap3(":/images/end.jpg");

    // Get the scene dimensions (width and height)
    double sceneWidth = sceneRect().width();
    double sceneHeight = sceneRect().height();

    // Scale the background image to match the scene size
    QPixmap scaledBgPixmap = bgPixmap3.scaled(sceneWidth, sceneHeight, Qt::KeepAspectRatioByExpanding);

    // Update the scene with the new scaled background
    setSceneRect(0, 0, scaledBgPixmap.width(), scaledBgPixmap.height());
    background->setPixmap(scaledBgPixmap);
    background->setPos(0, 0);  // Position the background at the top-left corner
// Reset Mario's position to the calculated ground level
    player->setPosition(450, 290);
      player->stopMoving();
    clearEnemiesAndCoins() ;

   // Create a dark overlay to show a message
    QGraphicsRectItem *overlay = new QGraphicsRectItem();
    overlay->setRect(sceneRect());  // Ensure it covers the whole scene
    overlay->setBrush(QColor(0, 0, 0, 150));  // Black with some opacity
    overlay->setZValue(1);  // Overlay should be above the background
    addItem(overlay);

    // Create the game over message
    QGraphicsTextItem *textItem = new QGraphicsTextItem("Game Complete! Thank you for playing!");
    textItem->setDefaultTextColor(Qt::white);
    textItem->setFont(QFont("Arial", 24, QFont::Bold));
    textItem->setZValue(2);  // Make sure the text is above the overlay
    textItem->setPos(sceneRect().width() / 2 - textItem->boundingRect().width() / 2,
                     sceneRect().height() / 2 - textItem->boundingRect().height() / 2);  // Center the text
    addItem(textItem);

    // Create a button to go to level 2
    QPushButton *level2Button = new QPushButton("Go to Level 2");
    level2Button->setFont(QFont("Arial", 16));
    level2Button->setFixedSize(150, 50);

    // Connect the button's click signal to a slot for transitioning to level 2
    connect(level2Button, &QPushButton::clicked, this, &GameScene::goToLevel2Triggered);

    // Embed the button in the scene
    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(level2Button);
    proxyWidget->setZValue(2);
    proxyWidget->setPos(sceneRect().width() / 2 - level2Button->width() / 2,
                        sceneRect().height() / 2 - level2Button->height() / 2 + 50);
    addItem(proxyWidget);
}

void GameScene::updateScene() {
    player->update();
    // Move coins and enemies with the player
    for (Coin *coin : coins) {
        coin->setPos(coin->x() - player->getVelocityX(), coin->y());
    }

    // Move enemies with the player
    for (Enemy *enemy : enemies) {
        enemy->update();  // Update the movement of the enemies
    }

    // Check for collisions with coins
    for (Coin *coin : coins) {
        if (player->collidesWithItem(coin)) {
            updateScore(10); // Increase score
            removeItem(coin);
            coins.removeOne(coin);
            delete coin;
            break;
        }
    }

    // Check for collisions with enemies (mushrooms)
    for (int i = 0; i < enemies.size(); ++i) {
        handleEnemyCollisions();
    }
    if (!newScreenTriggered && player->x() >= sceneRect().width()) {
         newScreenTriggered = true;
        triggerNewScreen();
         // Prevent further triggering
    }
    // Trigger a new screen if the player reaches the right edge

    if (newScreenTriggered && player->x() >= sceneRect().width()) {
        triggerEndScreen();
    }
    // Trigger a new screen if Mario reaches the right edge of the first screen

}

void GameScene::showGameOverMessage(const QString &message) {
    QGraphicsRectItem *overlay = new QGraphicsRectItem();
    overlay->setRect(sceneRect());  // Ensure it covers the whole scene
    overlay->setBrush(QColor(0, 0, 0, 150));  // Black with 50% opacity
    overlay->setZValue(1);  // Make sure the overlay is above the background
    addItem(overlay);

    // Create a text item to display the message
    QGraphicsTextItem *textItem = new QGraphicsTextItem(message);
    textItem->setDefaultTextColor(Qt::white);
    textItem->setFont(QFont("Arial", 24, QFont::Bold));
    textItem->setZValue(2);  // Make sure the text is above the overlay
    textItem->setPos(sceneRect().width() / 2 - textItem->boundingRect().width() / 2,
                     sceneRect().height() / 2 - textItem->boundingRect().height() / 2);  // Center the text
    addItem(textItem);

    // Create a button to go to level 2
    QPushButton *level2Button = new QPushButton("Go to Level 2");
    level2Button->setFont(QFont("Arial", 16));
    level2Button->setFixedSize(150, 50);

    // Connect the button's click signal to a slot for transitioning to level 2
    connect(level2Button, &QPushButton::clicked, this, &GameScene::goToLevel2Triggered);

    // Embed the button in the scene
    QGraphicsProxyWidget *proxyWidget = new QGraphicsProxyWidget();
    proxyWidget->setWidget(level2Button);
    proxyWidget->setZValue(2);
    proxyWidget->setPos(sceneRect().width() / 2 - level2Button->width() / 2,
                        sceneRect().height() / 2 - level2Button->height() / 2 + 50);
    addItem(proxyWidget);
}
void GameScene::goToLevel2Triggered() {
    // Create Level 3 scene
    Level2 *level2 = new Level2(score,health, this);

    // Get the view that is displaying the current scene
    QList<QGraphicsView *> views = this->views();
    if (!views.isEmpty()) {
        QGraphicsView *view = views.first();

        // Set the new scene in the same view
        view->setScene(level2);

        // Optionally delete this scene to free memory

    } else {
        qDebug() << "No view is associated with the current scene!";
    }
}
void GameScene::clearEnemiesAndCoins() {

        // Delete and clear enemies
        for (auto enemy : enemies) {
            delete enemy;  // Free memory for each enemy
        }
        enemies.clear();

        // Delete and clear coins
        for (auto coin : coins) {
            delete coin;  // Free memory for each coin
        }
        coins.clear();


}
void GameScene::addHearts() {
    // Clear any existing hearts
    for (QGraphicsPixmapItem *heart : hearts) {
        removeItem(heart);
        delete heart;
    }
    hearts.clear();

    // Calculate the number of hearts based on the health
    int numHearts = health / 30;  // For example, if health is 300, we show 3 hearts

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

void GameScene::updateHealthDisplay() {
    int heartsToShow = health / 30;  // For each 100 health, show one heart

    // Update the hearts display
    int currentHeartsCount = hearts.size();

    // Remove hearts if health decreased
    while (currentHeartsCount > heartsToShow) {
        removeItem(hearts.back());
        delete hearts.back();
        hearts.removeLast();
        currentHeartsCount--;
    }

}

void GameScene::generateEnemies() {

        for (int i = 0; i < 3; ++i) {  // Adjust the number of enemies as needed
            bool validPosition = false;
            int randomX;
            // Try finding a valid position for the enemy
            while (!validPosition) {
                randomX = QRandomGenerator::global()->bounded(100, 800);  // Random x-coordinate (100 to 800)

                // Check if the new enemy overlaps with any existing enemy
                validPosition = true;
                for (Enemy *existingEnemy : enemies) {
                    if (qAbs(existingEnemy->x() - randomX) < 100) {  // Ensure minimum distance (100 pixels in this case)
                        validPosition = false;
                        break;  // Skip to the next iteration if overlap is detected
                    }
                }
            }

            // Create the enemy at the valid random position
            Enemy *enemy = new Enemy(randomX, 420);  // y-position fixed at 530 (ground level)
            enemies.append(enemy);
            addItem(enemy);
        }
}

void GameScene::generateCoins() {
    // Add coins randomly on the y-axis and ensure they are separated from mushrooms
        for (int i = 0; i < 6; ++i) {
            bool validPosition = false;
            int randomX, randomY;

            // Try finding a valid position for the coin
            while (!validPosition) {
                randomX = QRandomGenerator::global()->bounded(100, 800);  // Random x-coordinate (100 to 800)
                randomY = QRandomGenerator::global()->bounded(250, 350);  // Adjust y-coordinate (higher than before)

                // Check if the new coin overlaps with any existing coin
                validPosition = true;
                for (Coin *existingCoin : coins) {
                    if (qAbs(existingCoin->x() - randomX) < 50 && qAbs(existingCoin->y() - randomY) < 50) {
                        validPosition = false;
                        break;  // Skip to the next iteration if overlap is detected
                    }
                }

                // Ensure coins don't overlap with enemies
                for (Enemy *existingEnemy : enemies) {
                    if (qAbs(existingEnemy->x() - randomX) < 50 && qAbs(existingEnemy->y() - randomY) < 50) {
                        validPosition = false;
                        break;  // Skip to the next iteration if overlap with enemy is detected
                    }
                }
            }

            // Create the coin at the valid random position
            Coin *coin = new Coin(randomX, randomY);
            coins.append(coin);
            addItem(coin);
        }
}



void GameScene::handleEnemyCollisions() {
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

        if (player->collidesWithItem(enemy) && health) {
            // Collide with enemy and reduce health
          /*   updateHealthDisplay();*/  // Update health display
            // health--;
          // Update health display
            qDebug() << "Health: " << health;

            // // If health reaches 0 or below, trigger game over
            // if (health <= 0) {
            //     triggerGameOver();
            //     break;  // Stop further checks if health is 0
            // }

            enemy->setEnabled(false);  // Disable interactions with this enemy
            qDebug() << "Enemy frozen";
        }
    }
}

void GameScene::updateScore(int change) {
    score += change;
    // Update the score display
    scoreText->setPlainText(QString("Score: %1").arg(score));
}

void GameScene::triggerGameOver() {
      restartButton->setVisible(true);
      score = 0;
       health=300;
      scoreText->setPlainText("Score: 0");
      // Clear all items (enemies, coins, hearts, etc.)
      clearEnemiesAndCoins();
      // Reset the player and reposition it at the starting point
      player->reset();
      player->setPosition(100, 380);  // Reset to the initial position
      // Reset hearts and health display
    connect(restartButton, &QPushButton::clicked, this, &GameScene::restartRequested);
}

void GameScene::restartRequested() {
    restartButton->setVisible(false);
    delete restartButton; // Clean up restartButton
    restartButton = nullptr; // Prevent further use
    health = 300;
    addHearts();
    generateEnemies();
    generateCoins();
    updateScene();
}


