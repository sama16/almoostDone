#include "Coin.h"

Coin::Coin(int x, int y) {
    setPixmap(QPixmap("C:/Users/hp/Documents/marioGame/coin.png").scaled(30, 30)); // Load coin sprite
    setPos(x, y);
}
