#include "weapon.h"
#include "Enemy.h"   // Updated to include Enemy header
#include <QGraphicsScene>
#include <QTimer>

Weapon::Weapon(int x, int y, int direction, Player *player) : direction(direction), player(player) {
    // Set initial position and other properties of the weapon (e.g., size)
    setPixmap(QPixmap("C:/Users/hp/Documents/marioGame/bullet.png").scaled(30, 30)); // Load coin sprite
    setPos(x, y);

    // Create a timer to destroy the weapon after a certain amount of time (e.g., 3 seconds)
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Weapon::deleteLater);
    timer->start(3000); // Destroy the weapon after 3 seconds

    // Add the weapon to the scene
    player->scene()->addItem(this);
}

Weapon::~Weapon() {
    // Clean up when the weapon is destroyed
    delete timer;
}

void Weapon::move() {

}

void Weapon::collideWithEnemy(Enemy *enemy) {

}
