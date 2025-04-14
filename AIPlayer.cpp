#include "AIPlayer.h"

AIPlayer::AIPlayer(int symbol) : Player(symbol) {}

int AIPlayer::getMove(const GameBoard& board) {
    std::vector<float> state = board.getState();
    std::vector<float> probs = ann.predict(state);
    int bestMove = -1;
    float maxProb = -1.0f;

    for (int i = 0; i < 9; ++i) {
        if (state[i] == 0 && probs[i] > maxProb) {
            maxProb = probs[i];
            bestMove = i;
        }
    }
    return bestMove;
}

void AIPlayer::trainBatch(const std::vector<std::vector<float> >& inputs, const std::vector<int>& targets) {
    ann.trainBatch(inputs, targets);
}

void AIPlayer::saveWeights() const {
    ann.saveWeights();
}