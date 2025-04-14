#ifndef PLAYER_H
#define PLAYER_H

#include "GameBoard.h"

class Player {
protected:
    int symbol;

public:
    Player(int symbol) : symbol(symbol) {}
    virtual ~Player() {}
    virtual int getMove(const GameBoard& board) = 0;
    int getSymbol() const { return symbol; }
};

#endif // PLAYER_H