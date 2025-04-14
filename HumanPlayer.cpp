#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(int symbol) : Player(symbol) {}

int HumanPlayer::getMove(const GameBoard& board) {
    int move;
    while (true) {
        std::cout << "Player " << (symbol == 1 ? "X" : "O") << ", enter move (0-8): ";
        std::cin >> move;
        if (std::cin.fail() || move < 0 || move >= 9 || board.getState()[move] != 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid move! Try again.\n";
        } else {
            return move;
        }
    }
}