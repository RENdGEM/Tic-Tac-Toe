#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(int symbol);
    int getMove(const GameBoard& board); // No override
};

#endif // HUMANPLAYER_H