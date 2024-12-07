#include "Heart.h"
#include <QPixmap>

Heart::Heart(int x, int y, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), posX(x), posY(y) {
    // Load the heart image and set the position
    setPixmap(QPixmap("C:/Users/hp/Desktop/csProjectMarioGame/marioGameProject-main/heart.png").scaled(30, 30));
    setPos(30, 60);  // Set the heart's position in the scene
}

void Heart::removeHeart() {
    // Removes the heart from the scene and deletes the object
    scene()->removeItem(this);  // Remove the heart item from the scene
    delete this;  // Delete the heart object
}

int Heart::getPositionX() const {
    return posX;
}

int Heart::getPositionY() const {
    return posY;
}
