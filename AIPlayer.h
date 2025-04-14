#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"
#include "ANN.h"

class AIPlayer : public Player {
private:
    ANN ann;

public:
    AIPlayer(int symbol);
    int getMove(const GameBoard& board); // No override
    void trainBatch(const std::vector<std::vector<float> >& inputs, const std::vector<int>& targets);
    void saveWeights() const;
};

#endif // AIPLAYER_H