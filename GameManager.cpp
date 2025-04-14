#include "GameManager.h"
#include <iostream>

GameManager::GameManager(Player* px, Player* po) : playerX(px), playerO(po) {}

void GameManager::playGame() {
    Player* currentPlayer = playerX;
    while (!board.isGameOver()) {
        board.printBoard();
        int move = currentPlayer->getMove(board);
        if (board.makeMove(move)) {
            currentPlayer = (currentPlayer == playerX) ? playerO : playerX;
        } else {
            std::cout << "Invalid move, try again!\n";
        }
    }
    board.printBoard();
    int winner = board.getWinner();
    if (winner == 1) std::cout << "Player X wins!\n";
    else if (winner == -1) std::cout << "Player O wins!\n";
    else std::cout << "It’s a draw!\n";
}