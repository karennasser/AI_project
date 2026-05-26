#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Board.h"
#include "PathFinder.h"
#include "WallValidator.h"
#include "Wall.h"
#include <utility>

// AIPlayer Class
// Handles AI decision-making for Quoridor. 
// Uses Minimax with Alpha-Beta pruning to evaluate future game states,
// satisfying the "Multiple AI Difficulty Levels" bonus requirement.
class AIPlayer {
public:
    int difficulty;     // Maps to Minimax search depth (e.g., 1=Easy, 2=Medium, 3=Hard)
    int playerIndex;    // 0 for Player 1, 1 for Player 2
    int opponentIndex;  // The opposing player

    // Constructor to initialize the AI with a specific difficulty level
    AIPlayer(int diffLevel, int pIndex);

    // Main entry point called by the game loop. 
    // Returns true if a wall was placed, false if the pawn was moved.
    bool takeTurn(Board& board, WallValidator& validator, PathFinder& pathFinder);
    std::pair<int, int> chooseMove(const Board& board, PathFinder& pathFinder, WallValidator& validator);
private:
    // Core decision methods
    Wall chooseWall(const Board& board, WallValidator& validator, PathFinder& pathFinder);
    
    // Static evaluation functions to score a specific board state
    int evaluateMoves(const Board& board, PathFinder& pathFinder);
    int evaluateWalls(const Board& board, PathFinder& pathFinder);

    // Recursive Game Tree algorithm (Minimax with Alpha-Beta Pruning)
    // Used to look ahead multiple turns based on the difficulty attribute
    int minimax(Board boardState, int depth, int alpha, int beta, bool isMaximizingPlayer, PathFinder& pathFinder, WallValidator& validator);
};

#endif // AIPLAYER_H