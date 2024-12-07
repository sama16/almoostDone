#ifndef HEART_H
#define HEART_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class Heart : public QGraphicsPixmapItem {
public:
    // Constructor to initialize the heart with its position
    Heart(int x, int y, QGraphicsItem *parent = nullptr);

    // Method to remove the heart from the scene
    void removeHeart();

    // Getter for the heart's position (if needed for logic)
    int getPositionX() const;
    int getPositionY() const;

private:
    int posX;
    int posY;
};

#endif // HEART_H
