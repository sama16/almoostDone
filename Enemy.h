#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>

class Enemy : public QGraphicsPixmapItem {
    public:
    Enemy(int x, int y);

    void update();
    double getVelocityX(){return velocityX; }



private:
    double velocityX;  // Horizontal velocity of the enemy

};

#endif // ENEMY_H
