#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameBoard.h"
#include "Player.h"

class GameManager {
private:
    GameBoard board;
    Player* playerX;
    Player* playerO;

public:
    GameManager(Player* px, Player* po);
    void playGame();
};

#endif // GAMEMANAGER_H