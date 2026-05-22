#include "Board.h"
#include <stdexcept>
#include <cmath>
using namespace std;

// -----------------------------------------------------------------------
// Constructors
// -----------------------------------------------------------------------
Board::Board()
    : size(9), temporaryWallCount(0)
{
    // Standard Quoridor starting positions:
    // Player 1: top-center (0,4) → goal row 8
    // Player 2: bottom-center (8,4) → goal row 0
    players.push_back({0, size / 2});
    players.push_back({size - 1, size / 2});
}

Board::Board(int size)
    : size(size), temporaryWallCount(0)
{
    // Custom size must be odd and at least 5
    if (size < 5 || size % 2 == 0) {
        throw invalid_argument("Board size must be an odd number >= 5.");
    }
    players.push_back({0, size / 2});
    players.push_back({size - 1, size / 2});
}


// Pawn methods

void Board::updatePawnPosition(int playerIndex, int newRow, int newCol) {
    if (playerIndex < 0 || playerIndex >= static_cast<int>(players.size())) {
        throw out_of_range("Invalid player index.");
    }
    if (!isInBounds(newRow, newCol)) {
        throw out_of_range("Target cell is outside the board.");
    }
    players[playerIndex] = {newRow, newCol};
}

vector<pair<int,int>> Board::getPlayers() const {
    return players;
}

pair<int,int> Board::getPlayer1Pos() const {
    return players[0];
}

pair<int,int> Board::getPlayer2Pos() const {
    return players[1];
}


// Cell query methods

bool Board::isOccupied(int row, int col) const {
    for (const auto& pos : players) {
        if (pos.first == row && pos.second == col) {
            return true;
        }
    }
    return false;
}


// Wall methods

void Board::addWall(const Wall& wall) {
    walls.push_back(wall);
}

const vector<Wall>& Board::getPlacedWalls() const {
    return walls;
}

}


// Temporary-wall helpers

void Board::addTemporaryWall(const Wall& wall) {
    walls.push_back(wall);
    temporaryWallCount++;
}

void Board::removeTemporaryWall() {
    if (temporaryWallCount <= 0) return;
    walls.pop_back();
    temporaryWallCount--;
}


// Board query helpers

bool Board::isInBounds(int row, int col) const {
    return row >= 0 && row < size && col >= 0 && col < size;
}

// Wall blocking logic:
// - Walls stored as anchor (x,y) in gap-space (0 to size-2)
// - HORIZONTAL wall at (x,y) blocks vertical movement between rows y and y+1, cols x and x+1
// - VERTICAL wall at (x,y) blocks horizontal movement between cols x and x+1, rows y and y+1
bool Board::isWallBetween(int row1, int col1, int row2, int col2) const {
    // Ensure cells are orthogonally adjacent (not diagonal)
    const int dr = abs(row2 - row1);
    const int dc = abs(col2 - col1);
    if ((dr + dc) != 1) return false;

    for (const auto& w : walls) {
        if (dr == 1 && dc == 0) {
            // Vertical movement: only HORIZONTAL walls can block
            if (w.isHorizontal()) {
                int gapRow = min(row1, row2);
                int gapCol = col1; // same as col2
                // Wall at (x, gapRow) covers columns gapCol and gapCol+1
                if (w.y == gapRow && (w.x == gapCol || w.x == gapCol - 1)) {
                    return true;
                }
            }
        } else {
            // Horizontal movement: only VERTICAL walls can block
            if (w.isVertical()) {
                int gapCol = min(col1, col2);
                int gapRow = row1; // same as row2
                // Wall at (gapCol, y) covers rows gapRow and gapRow+1
                if (w.x == gapCol && (w.y == gapRow || w.y == gapRow - 1)) {
                    return true;
                }
            }
        }
    }
    return false;
}
