#include "FlyingEnemy.h"
#include <QRandomGenerator>
#include <QPixmap>


FlyingEnemy::FlyingEnemy(int x, int y, QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), direction(1) {
    // Set the initial position
    setPos(x, y);

    // Set the flying enemy's pixmap
    setPixmap(QPixmap("C:/Users/hp/Desktop/csProjectMarioGame/marioGameProject-main/flyingenemy.png").scaled(50, 50));

    // Timer to periodically change direction
    movementTimer = new QTimer(this);
    connect(movementTimer, &QTimer::timeout, this, &FlyingEnemy::changeDirection);
    movementTimer->start(3000); // Change direction every 1 second
}

void FlyingEnemy::update() {
    // Move up or down
    setY(y() + direction * 2);

    // Prevent it from going out of bounds (adjust these limits as needed)
    if (y() < 80 || y() > 500) {
        direction *= -1; // Reverse direction
    }
}

void FlyingEnemy::changeDirection() {
    direction *= -1; // Reverse the flying direction
}
