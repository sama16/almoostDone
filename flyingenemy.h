#ifndef FLYINGENEMY_H
#define FLYINGENEMY_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class FlyingEnemy : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit FlyingEnemy(int x, int y, QObject *parent = nullptr);
    void update();

private slots:
    void changeDirection();

private:
    int direction; // 1 for down, -1 for up
    QTimer *movementTimer;
};

#endif // FLYINGENEMY_H
