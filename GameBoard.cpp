#include "GameBoard.h"

GameBoard::GameBoard() : board(SIDE * SIDE, 0), currentPlayer(1) {}

bool GameBoard::makeMove(int position) {
    if (position < 0 || position >= SIDE * SIDE || board[position] != 0) {
        std::cout << "Invalid move! Position: " << position << std::endl;
        return false;
    }
    board[position] = currentPlayer;
    currentPlayer = -currentPlayer;
    return true;
}

bool GameBoard::isGameOver() const {
    return getWinner() != 2;
}

int GameBoard::getWinner() const {
    // Check rows
    for (int i = 0; i < SIDE; ++i) {
        if (board[i * SIDE] != 0 && board[i * SIDE] == board[i * SIDE + 1] && board[i * SIDE] == board[i * SIDE + 2])
            return board[i * SIDE];
    }
    // Check columns
    for (int i = 0; i < SIDE; ++i) {
        if (board[i] != 0 && board[i] == board[i + SIDE] && board[i] == board[i + 2 * SIDE])
            return board[i];
    }
    // Check diagonals
    if (board[0] != 0 && board[0] == board[4] && board[0] == board[8]) return board[0];
    if (board[2] != 0 && board[2] == board[4] && board[2] == board[6]) return board[2];
    // Check draw
    for (int cell : board) {
        if (cell == 0) return 2; // Ongoing
    }
    return 0; // Draw
}

std::vector<float> GameBoard::getState() const {
    std::vector<float> state(SIDE * SIDE);
    for (int i = 0; i < SIDE * SIDE; ++i) state[i] = static_cast<float>(board[i]);
    return state;
}

void GameBoard::printBoard() const {
    for (int i = 0; i < SIDE; ++i) {
        for (int j = 0; j < SIDE; ++j) {
            char symbol = board[i * SIDE + j] == 1 ? 'X' : (board[i * SIDE + j] == -1 ? 'O' : ' ');
            std::cout << symbol << (j < SIDE - 1 ? " | " : "");
        }
        std::cout << std::endl;
        if (i < SIDE - 1) std::cout << "---------" << std::endl;
    }
    std::cout << std::endl;
}

void GameBoard::reset() {
    board.assign(SIDE * SIDE, 0);
    currentPlayer = 1;
}