#ifndef WEAPON_H
#define WEAPON_H
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QTimer>
#include "Player.h"  // Assuming Mario is the Player class
#include "Enemy.h"   // Assuming Enemy is a class for various enemies (e.g., Mushroom)
#include <QGraphicsScene>

class Weapon : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    Weapon(int x, int y, int direction, Player *player);
    ~Weapon();

    void move();
    void collideWithEnemy(Enemy *enemy);  // Handle collision with enemies

private:
    int direction;  // Direction the weapon will move in (e.g., left or right)
    Player *player; // Mario (Player) object
    QTimer *timer;  // Timer to disable weapon after a set time
};

#endif // WEAPON_H
