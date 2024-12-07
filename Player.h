#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QKeyEvent>

class Player : public QGraphicsPixmapItem {
public:
    Player();  // Constructor
    void handleInput(QKeyEvent *event, bool pressed);  // Handle user input (key presses)
    void update();  // Update player position and state

    // Getter for vertical velocity
    float getVelocityY() const { return velocityY; }

    // Getter for horizontal velocity
    float getVelocityX() const { return velocityX; }

    // Setters and getters for position (if needed)
    void setPosition(float x, float y) { setPos(x, y); }
    void reset();
    void stopMoving();


private:
    float velocityX;  // Horizontal velocity
    float velocityY;  // Vertical velocity (affects jumping and falling)
    bool isJumping;   // Flag to check if the player is in the air
    const float gravity = 0.5f;  // Gravity force to pull Mario down
    const float jumpStrength = -15.0f;  // How strong Mario's jump is
    const float maxFallSpeed = 10.0f;  // Max speed Mario can fall at
    const float groundY = 500.0f;  // Ground level (where Mario stands)


    void jump();  // Handle jumping action
    void applyGravity();  // Apply gravity to simulate falling
    void land();  // Handle landing after a jump
};

#endif // PLAYER_H
