#include "Enemy.h"
#include <QPixmap>

// Constructor for Enemy class
Enemy::Enemy(int x, int y) {
    setPixmap(QPixmap("C:/Users/hp/Documents/marioGame/enemy.png").scaled(30, 30)); // Load coin sprite
    setPos(x, y);

    // Initialize horizontal velocity
    velocityX = 1.2;  // Speed of movement (positive for right, negative for left)
}

// Update the enemy's position and movement
void Enemy::update() {
    // Move the enemy horizontally by velocityX
    setPos(x() + velocityX, y());

    // Get the width of the scene (assuming 800 pixels)
    int sceneWidth = 800;  // Set the scene width

    // Get the width of the enemy
    int enemyWidth = pixmap().width();  // Get the width of the enemy sprite

    // If the enemy hits the left boundary (x <= 0), reverse direction
    if (x() <= 0) {
        velocityX = -velocityX;  // Reverse direction
    }

    // If the enemy hits the right boundary (x >= sceneWidth - enemyWidth), reverse direction
    else if (x() >= sceneWidth - enemyWidth) {
        velocityX = -velocityX;  // Reverse direction
    }
}
