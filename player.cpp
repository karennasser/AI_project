#include "player.h"

Player::Player(int id, int boardSize) {
    this->id = id;

    // Standard Quoridor rule sets 10 walls per player for a 2-player game
    this->wallsRemaining = 10;

    // Dynamically calculate starting position at the center of the baseline based on board size
    if (id == 0) {
        row = 0;                     // Player 0 starts at the first row (Top)
        col = boardSize / 2;         // Center column
    } else {
        row = boardSize - 1;         // Player 1 starts at the last row (Bottom)
        col = boardSize / 2;         // Center column
    }
}

// Updates the pawn's grid coordinates to a new position
void Player::move(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// Decrements the remaining wall count by 1 when a wall is successfully placed
void Player::useWall() {
    if (wallsRemaining > 0) {
        wallsRemaining--;
    }
}

// Getter implementations
int Player::getId() const { return id; }
int Player::getRow() const { return row; }
int Player::getCol() const { return col; }
int Player::getWallsRemaining() const { return wallsRemaining; }