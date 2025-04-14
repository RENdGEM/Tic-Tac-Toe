#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include <iostream>

class GameBoard {
private:
    static const int SIDE = 3; // Replace constexpr if still an issue
    std::vector<int> board;
    int currentPlayer;

public:
    GameBoard();
    bool makeMove(int position);
    bool isGameOver() const;
    int getWinner() const;
    std::vector<float> getState() const;
    void printBoard() const;
    void reset();
    int getCurrentPlayer() const { return currentPlayer; }
};

#endif // GAMEBOARD_H