#include "Player.h"
#include <QGraphicsScene>
#include <QKeyEvent>


Player::Player() {
    // Load the image for Mario
    QPixmap marioPixmap(":/images/mario.png");  // Path to your image file
    marioPixmap = marioPixmap.scaled(70, 70, Qt::KeepAspectRatio);  // Resize
    setPixmap(marioPixmap);
    // Initialize Mario's velocity and state
    velocityX = 0;  // Horizontal velocity (no movement initially)
    velocityY = 0;  // Initial vertical velocity (not jumping initially)
    isJumping = false;
    setPos(100, 380);  // Initial position at x=100, y=380 (ground level)
}

void Player::handleInput(QKeyEvent *event, bool pressed) {

    // Handle left and right movement based on key press
    if (event->key() == Qt::Key_Left) {
        velocityX = pressed ? -5 : 0;  // Move left (negative velocity) when pressed
    } else if (event->key() == Qt::Key_Right) {
        velocityX = pressed ? 5 : 0;  // Move right (positive velocity) when pressed
    }

    // Handle jump action based on the spacebar
    if (event->key() == Qt::Key_Space && pressed && !isJumping) {
        jump();  // If Space is pressed and Mario is not already jumping
    }
}
void Player::reset(){
    velocityX = 0;  // Horizontal velocity (no movement initially)
    velocityY = 0;  // Initial vertical velocity (not jumping initially)
    isJumping = false;

}
void Player::stopMoving() {
    // Set the velocities to 0 to stop Mario from moving
    velocityX = 0;
    velocityY = 0;
}

void Player::update() {
    applyGravity();  // Apply gravity to pull Mario down

    // Update Mario's position based on horizontal and vertical velocities
    setPos(x() + velocityX, y() + velocityY);

    // Ensure Mario doesn't fall through the ground (or any platforms)
    if (y() >= 380) {
        land();  // If Mario reaches the ground, he lands
    }

    // Limit the falling speed
    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }
}

void Player::jump() {
    // Initiate a jump by setting vertical velocity to a negative value
    velocityY = jumpStrength;
    isJumping = true;
}

void Player::applyGravity() {
    // If Mario is in the air, gravity pulls him down
    if (isJumping) {
        velocityY += gravity;  // Add gravity to the velocity
    }
}

void Player::land() {
    // Mario has hit the ground, stop falling and reset vertical velocity
    if (y() >= 380) {  // If Mario reaches or is below the ground level
        velocityY = 0;  // Reset vertical velocity to stop falling
        isJumping = false;  // Mario is no longer jumping
        setPos(x(), 380);  // Set y-position to the ground level
    }
}


