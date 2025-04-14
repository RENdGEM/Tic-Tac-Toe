#include "MinimaxPlayer.h"
#include <climits>

MinimaxPlayer::MinimaxPlayer(int symbol) : Player(symbol) {}

int MinimaxPlayer::getMove(const GameBoard& board) {
    int bestMove = -1;
    int bestScore = INT_MIN;
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (int pos = 0; pos < 9; ++pos) {
        if (board.getState()[pos] == 0) {
            GameBoard tempBoard = board;
            tempBoard.makeMove(pos);
            int score = minimax(tempBoard, 0, alpha, beta, false);
            if (score > bestScore) {
                bestScore = score;
                bestMove = pos;
            }
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha) break; // Alpha-beta pruning
        }
    }
    return bestMove;
}

int MinimaxPlayer::minimax(GameBoard board, int depth, int alpha, int beta, bool isMaximizing) {
    if (board.isGameOver()) {
        int winner = board.getWinner();
        if (winner == symbol) return 10 - depth;
        if (winner == -symbol) return -10 + depth;
        return 0;
    }

    if (isMaximizing) {
        int bestScore = INT_MIN;
        for (int pos = 0; pos < 9; ++pos) {
            if (board.getState()[pos] == 0) {
                GameBoard tempBoard = board;
                tempBoard.makeMove(pos);
                int score = minimax(tempBoard, depth + 1, alpha, beta, false);
                bestScore = std::max(bestScore, score);
                alpha = std::max(alpha, bestScore);
                if (beta <= alpha) break; // Pruning
            }
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        for (int pos = 0; pos < 9; ++pos) {
            if (board.getState()[pos] == 0) {
                GameBoard tempBoard = board;
                tempBoard.makeMove(pos);
                int score = minimax(tempBoard, depth + 1, alpha, beta, true);
                bestScore = std::min(bestScore, score);
                beta = std::min(beta, bestScore);
                if (beta <= alpha) break; // Pruning
            }
        }
        return bestScore;
    }
}