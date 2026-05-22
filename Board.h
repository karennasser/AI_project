#ifndef BOARD_H
#define BOARD_H

#include "Wall.h"
#include <vector>
#include <utility>
using namespace std;

// File: Board.h
// The Board class is the single source of truth for Quoridor game state.
// Stores: board size, player pawn positions, placed walls, and temp walls for validation.
//
// Coordinate system:
// - Pawns: (row, col) with row 0 at top, row 8 at bottom (9x9 board)
// - Player 1 starts at (0,4), goal is row 8
// - Player 2 starts at (8,4), goal is row 0
// - Walls: (x,y) in gap-space (0–7), where x=column-gap, y=row-gap

// Forward declaration to avoid circular include with PathFinder
class PathFinder;

class Board {
public:

    // Attributes

    int size;                           // Board dimension (9 for standard)
    vector<pair<int,int>> players;      // players[0]=Player1 pos, players[1]=Player2 pos
    vector<Wall> walls;                 // All permanently placed walls


    // Constructors

    // Create standard 9x9 board with pawns at starting positions
    Board();

    // Create board with custom odd size >=5 (for bonus feature)
    explicit Board(int size);


    // Pawn methods

    // Move player's pawn to (newRow, newCol)
    // NOTE: Does NOT validate legality — call MoveValidator first
    void updatePawnPosition(int playerIndex, int newRow, int newCol);

    // Return copy of all player positions
    vector<pair<int,int>> getPlayers() const;

    // Convenience: get Player 1's position
    pair<int,int> getPlayer1Pos() const;

    // Convenience: get Player 2's position
    pair<int,int> getPlayer2Pos() const;


    // Cell query methods

    // Returns true if any pawn occupies (row, col)
    bool isOccupied(int row, int col) const;


    // Wall methods

    // Add a permanent wall to the board
    // NOTE: Does NOT validate — call WallValidator::isValidWall() first
    void addWall(const Wall& wall);

    // Return const reference to placed walls (matches WallValidator call)
    const vector<Wall>& getPlacedWalls() const;


    // Temporary-wall helpers (used by WallValidator for path checks)

    // Add a wall temporarily (doesn't consume player's wall count)
    void addTemporaryWall(const Wall& wall);

    // Remove the most recently added temporary wall
    void removeTemporaryWall();


    // Board query helpers

    // Returns true if a wall blocks movement between two adjacent cells
    // Used by pathfinding and move validation
    bool isWallBetween(int row1, int col1, int row2, int col2) const;

    // Returns true if (row, col) is within board bounds [0, size)
    bool isInBounds(int row, int col) const;

private:
    // Tracks how many temp walls were added (for proper rollback)
    int temporaryWallCount;
};
#endif // BOARD_H
