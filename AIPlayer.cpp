#include "AIPlayer.h"
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

AIPlayer::AIPlayer(int diffLevel, int pIndex) 
    : difficulty(diffLevel), playerIndex(pIndex) 
{
    opponentIndex = (playerIndex == 0) ? 1 : 0;
}

// ---------------------------------------------------------
// Evaluation Functions (Leaf Node Scoring)
// ---------------------------------------------------------

// Evaluates the board state purely based on pawn distances.
// We want to minimize our distance to the goal and maximize the opponent's.
int AIPlayer::evaluateMoves(const Board& board, PathFinder& pathFinder) {
    int myDistance = pathFinder.findShortestPath(board, playerIndex);
    int oppDistance = pathFinder.findShortestPath(board, opponentIndex);

    // If a path is blocked (-1), this is an illegal state based on game rules
    if (myDistance == -1 || oppDistance == -1) {
        return (myDistance == -1) ? -99999 : 99999;
    }

    // Heuristic: Opponent's distance minus my distance. 
    // Higher score means a better position for the AI.
    return oppDistance - myDistance;
}

// Wrapper for evaluating the board after a wall placement.
// In Quoridor, scoring a wall state is fundamentally the same as scoring a move state
// (comparing path lengths), but this keeps the separation of concerns clear.
int AIPlayer::evaluateWalls(const Board& board, PathFinder& pathFinder) {
    return evaluateMoves(board, pathFinder);
}

// ---------------------------------------------------------
// Minimax Algorithm with Alpha-Beta Pruning
// ---------------------------------------------------------

// Explores future turns to find the most optimal sequence of moves.
// Passing 'boardState' by value allows us to safely simulate moves without breaking the real game board.
int AIPlayer::minimax(Board boardState, int depth, int alpha, int beta, bool isMaximizingPlayer, PathFinder& pathFinder, WallValidator& validator) {
    
    // Base Case: Reached the depth limit (difficulty level) or game over
    int myDist = pathFinder.findShortestPath(boardState, playerIndex);
    int oppDist = pathFinder.findShortestPath(boardState, opponentIndex);
    
    if (depth == 0 || myDist == 0 || oppDist == 0) {
        return evaluateMoves(boardState, pathFinder);
    }

    int currentPlayer = isMaximizingPlayer ? playerIndex : opponentIndex;
    pair<int, int> currentPos = boardState.getPlayers()[currentPlayer];

    if (isMaximizingPlayer) {
        int maxEval = numeric_limits<int>::min();

        // 1. Simulate Pawn Moves
        const int dRow[] = {-1, 1, 0, 0};
        const int dCol[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            int nextRow = currentPos.first + dRow[i];
            int nextCol = currentPos.second + dCol[i];

            if (boardState.isInBounds(nextRow, nextCol) && 
                !boardState.isWallBetween(currentPos.first, currentPos.second, nextRow, nextCol) &&
                !boardState.isOccupied(nextRow, nextCol)) 
            {
                Board nextBoard = boardState;
                nextBoard.updatePawnPosition(currentPlayer, nextRow, nextCol);
                int eval = minimax(nextBoard, depth - 1, alpha, beta, false, pathFinder, validator);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break; // Alpha-Beta Pruning
            }
        }
        return maxEval;
    } else {
        // Minimizing player (simulating the opponent's best response)
        int minEval = numeric_limits<int>::max();

        // Simulate opponent's pawn moves
        const int dRow[] = {-1, 1, 0, 0};
        const int dCol[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; i++) {
            int nextRow = currentPos.first + dRow[i];
            int nextCol = currentPos.second + dCol[i];

            if (boardState.isInBounds(nextRow, nextCol) && 
                !boardState.isWallBetween(currentPos.first, currentPos.second, nextRow, nextCol) &&
                !boardState.isOccupied(nextRow, nextCol)) 
            {
                Board nextBoard = boardState;
                nextBoard.updatePawnPosition(currentPlayer, nextRow, nextCol);
                int eval = minimax(nextBoard, depth - 1, alpha, beta, true, pathFinder, validator);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break; // Alpha-Beta Pruning
            }
        }
        return minEval;
    }
}

// ---------------------------------------------------------
// Top-Level Decision Methods
// ---------------------------------------------------------

// Kicks off the Minimax search for all valid adjacent moves and picks the best one.
pair<int, int> AIPlayer::chooseMove(const Board& board, PathFinder& pathFinder, WallValidator& validator) {
    pair<int, int> currentPos = board.getPlayers()[playerIndex];
    pair<int, int> bestMove = currentPos;
    int bestScore = numeric_limits<int>::min();

    const int dRow[] = {-1, 1, 0, 0};
    const int dCol[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nextRow = currentPos.first + dRow[i];
        int nextCol = currentPos.second + dCol[i];

        if (board.isInBounds(nextRow, nextCol) && 
            !board.isWallBetween(currentPos.first, currentPos.second, nextRow, nextCol) &&
            !board.isOccupied(nextRow, nextCol)) 
        {
            Board simBoard = board;
            simBoard.updatePawnPosition(playerIndex, nextRow, nextCol);
            
            // Start Minimax tree from this branch
            int score = minimax(simBoard, difficulty - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, pathFinder, validator);
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = {nextRow, nextCol};
            }
        }
    }
    return bestMove;
}

// Scans the board for strategic wall placements and scores them using Minimax.
Wall AIPlayer::chooseWall(const Board& board, WallValidator& validator, PathFinder& pathFinder) {
    Wall bestWall(-1, -1, Orientation::HORIZONTAL);
    int bestScore = numeric_limits<int>::min();

    // To prevent the AI from taking too long on higher difficulties, we only check 
    // wall placements if we have walls left.
    if (board.walls.size() >= 20) return bestWall;

    // Scan the 8x8 gap grid
    for (int x = 0; x <= 7; x++) {
        for (int y = 0; y <= 7; y++) {
            
            // Evaluate Horizontal Wall
            Wall hWall(x, y, Orientation::HORIZONTAL);
            Board simBoardH = board; // Copy board to test validator safely
            if (validator.isValidWall(hWall, simBoardH, pathFinder)) {
                simBoardH.addWall(hWall);
                int score = minimax(simBoardH, difficulty - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, pathFinder, validator);
                if (score > bestScore) {
                    bestScore = score;
                    bestWall = hWall;
                }
            }

            // Evaluate Vertical Wall
            Wall vWall(x, y, Orientation::VERTICAL);
            Board simBoardV = board; 
            if (validator.isValidWall(vWall, simBoardV, pathFinder)) {
                simBoardV.addWall(vWall);
                int score = minimax(simBoardV, difficulty - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, pathFinder, validator);
                if (score > bestScore) {
                    bestScore = score;
                    bestWall = vWall;
                }
            }
        }
    }
    return bestWall;
}

// The master control function called by the game loop.
bool AIPlayer::takeTurn(Board& board, WallValidator& validator, PathFinder& pathFinder) {
    
    // Step 1: Calculate the best possible pawn move and its future score
    pair<int, int> bestMove = chooseMove(board, pathFinder, validator);
    Board simMoveBoard = board;
    simMoveBoard.updatePawnPosition(playerIndex, bestMove.first, bestMove.second);
    int moveScore = evaluateMoves(simMoveBoard, pathFinder);

    // Step 2: If the AI is on Medium/Hard difficulty, calculate the best possible wall placement
    int wallScore = numeric_limits<int>::min();
    Wall bestWall(-1, -1, Orientation::HORIZONTAL);
    
    if (difficulty >= 2) {
        bestWall = chooseWall(board, validator, pathFinder);
        if (bestWall.x != -1) {
            Board simWallBoard = board;
            simWallBoard.addWall(bestWall);
            wallScore = evaluateWalls(simWallBoard, pathFinder);
        }
    }

    // Step 3: Action Execution - Compare the static scores to make the final choice
    // If placing a wall creates a vastly better board state than moving, do it.
    if (wallScore > moveScore && bestWall.x != -1) {
        board.addWall(bestWall);
        return true; // Wall placed
    }

    // Default to moving the pawn
    board.updatePawnPosition(playerIndex, bestMove.first, bestMove.second);
    return false; // Pawn moved
}