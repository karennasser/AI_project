#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "Board.h"
#include <vector>

class PathFinder {
public:
    PathFinder() = default;

    // Checks if a specific player has any clear, unblocked path to their goal line
    bool hasPath(const Board& board, int playerIndex) const;

    // Calculates the exact shortest path length (in steps) to the goal line; returns -1 if trapped
    int findShortestPath(const Board& board, int playerIndex) const;

private:
    // Core Breadth-First Search (BFS) engine used for both reachability and distance calculations
    int bfs(const Board& board, int playerIndex) const;
};

#endif
