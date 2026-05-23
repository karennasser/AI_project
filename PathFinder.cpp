#include "PathFinder.h"
#include <queue>
#include <utility>

using namespace std;

int PathFinder::bfs(const Board& board, int playerIndex) const {
    // Get starting position of the requested player
    vector<pair<int, int>> playerPositions = board.getPlayers();
    if (playerIndex < 0 || playerIndex >= static_cast<int>(playerPositions.size())) {
        return -1;
    }

    pair<int, int> startPos = playerPositions[playerIndex];

    // Player 1 needs to reach the bottom row, Player 2 needs to reach the top row
    int goalRow = (playerIndex == 0) ? (board.size - 1) : 0;

    // Already at the goal line
    if (startPos.first == goalRow) {
        return 0;
    }

    // Queue stores: { {row, col}, current_distance }
    queue<pair<pair<int, int>, int>> q;

    // 2D visited array matches board size dynamically to support custom board sizes
    vector<vector<bool>> visited(board.size, vector<bool>(board.size, false));

    // Initialize BFS with the player's current position
    q.push({startPos, 0});
    visited[startPos.first][startPos.second] = true;

    // Direction vectors for orthogonal movement (Up, Down, Left, Right)
    const int dRow[] = {-1, 1, 0, 0};
    const int dCol[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        int currRow = current.first.first;
        int currCol = current.first.second;
        int dist = current.second;

        // BFS guarantees the first time we hit the goal row is the shortest path
        if (currRow == goalRow) {
            return dist;
        }

        // Check all 4 neighboring cells
        for (int i = 0; i < 4; ++i) {
            int nextRow = currRow + dRow[i];
            int nextCol = currCol + dCol[i];

            // Verify neighbor cell is on the board
            if (board.isInBounds(nextRow, nextCol)) {
                // Skip if cell was already evaluated
                if (!visited[nextRow][nextCol]) {
                    // Call Member 1's method to check if a placed wall blocks this edge
                    if (!board.isWallBetween(currRow, currCol, nextRow, nextCol)) {
                        visited[nextRow][nextCol] = true;
                        q.push({{nextRow, nextCol}, dist + 1});
                    }
                }
            }
        }
    }

    // Target row was unreachable
    return -1;
}

bool PathFinder::hasPath(const Board& board, int playerIndex) const {
    return bfs(board, playerIndex) != -1;
}

int PathFinder::findShortestPath(const Board& board, int playerIndex) const {
    return bfs(board, playerIndex);
}
