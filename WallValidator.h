#ifndef WALLVALIDATOR_H
#define WALLVALIDATOR_H

#include <vector>
#include "Wall.h"
#include "Board.h"
#include "PathFinder.h"

class WallValidator {
public:
    bool isValidWall(const Wall& newWall, Board& board, PathFinder& pathFinder);

private:
    bool isInsideBoard(const Wall& wall);

    bool doesOverlap(
        const Wall& wall,
        const std::vector<Wall>& existingWalls
    );

    bool doesCross(
        const Wall& wall,
        const std::vector<Wall>& existingWalls
    );

    bool hasValidPaths(
        const Wall& newWall,
        Board& board,
        PathFinder& pathFinder
    );
};

#endif // WALLVALIDATOR_H