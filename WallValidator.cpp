#include "WallValidator.h"
#include <cmath>

// Note: Implementation assumes Board and PathFinder have these basic methods defined
// in your project's other classes.

bool WallValidator::isValidWall(const Wall& newWall, Board& board, PathFinder& pathFinder) {
    // 1. Boundary check
    if (!isInsideBoard(newWall)) return false;

    // 2. Structural checks against existing walls
    // (Assuming board.getPlacedWalls() returns a std::vector<Wall>)
    const auto& existingWalls = board.getPlacedWalls();

    if (doesOverlap(newWall, existingWalls)) return false;
    if (doesCross(newWall, existingWalls)) return false;

    // 3. Pathfinding check (The most computationally expensive, done last)
    if (!hasValidPaths(newWall, board, pathFinder)) return false;

    return true;
}

bool WallValidator::isInsideBoard(const Wall& wall) {
    // On a 9x9 board, there are 8 gaps/intersections between cells.
    // Coordinates for walls should be between 0 and 7.
    return (wall.x >= 0 && wall.x <= 7 && wall.y >= 0 && wall.y <= 7);
}

bool WallValidator::doesOverlap(const Wall& wall, const std::vector<Wall>& existingWalls) {
    for (const auto& ex : existingWalls) {
        if (wall.orientation == ex.orientation) {
            if (wall.orientation == Orientation::HORIZONTAL) {
                // Check if they share the same row and are adjacent/overlapping
                if (wall.y == ex.y && std::abs(wall.x - ex.x) <= 1) return true;
            } else {
                // Check if they share the same column and are adjacent/overlapping
                if (wall.x == ex.x && std::abs(wall.y - ex.y) <= 1) return true;
            }
        }
    }
    return false;
}

bool WallValidator::doesCross(const Wall& wall, const std::vector<Wall>& existingWalls) {
    for (const auto& ex : existingWalls) {
        // A cross occurs if a H wall and a V wall share the exact same intersection point
        if (wall.orientation != ex.orientation) {
            if (wall.x == ex.x && wall.y == ex.y) return true;
        }
    }
    return false;
}

bool WallValidator::hasValidPaths(const Wall& newWall, Board& board, PathFinder& pathFinder) {
    // 1. Temporarily add the wall to the board state
    board.addTemporaryWall(newWall);

    // 2. Check if BOTH players can still reach their goal
    // Player 1 goal is usually row 8, Player 2 goal is row 0
    bool p1Path = pathFinder.checkPathExists(board.getPlayer1Pos(), 8);
    bool p2Path = pathFinder.checkPathExists(board.getPlayer2Pos(), 0);

    // 3. Remove the temporary wall
    board.removeTemporaryWall();

    return (p1Path && p2Path);
}