#include "GameBoard.h"
#include "HumanPlayer.h"
#include "MinimaxPlayer.h" // Use MinimaxPlayer instead of AIPlayer
#include "GameManager.h"
#include <iostream>

int main() {
    MinimaxPlayer ai(-1); // Unbeatable O
    std::cout << "AI initialized with minimax alpha-beta pruning!\n";

    char playAgain;
    do {
        std::cout << "1) Human vs Human, 2) Human vs AI: ";
        int mode;
        std::cin >> mode;
        HumanPlayer humanX(1);
        Player* playerO = (mode == 1) ? static_cast<Player*>(new HumanPlayer(-1)) : static_cast<Player*>(&ai);
        GameManager game(&humanX, playerO);
        game.playGame();
        std::cout << "Play again? (y/n): ";
        std::cin >> playAgain;
        if (mode == 1) delete playerO;
    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}