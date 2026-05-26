#include "GameController.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

GameController::GameController()
    : board(),
      aiPlayer(2, 1), // Default difficulty = Medium
      currentPlayer(0),
      mode(GameMode::HUMAN_VS_HUMAN)
{
    playerWalls[0] = 10;
    playerWalls[1] = 10;
}

GameController::GameController(GameMode mode)
    : board(),
      aiPlayer(2, 1),
      currentPlayer(0),
      mode(mode)
{
    playerWalls[0] = 10;
    playerWalls[1] = 10;
}

void GameController::startGame() {

    board = Board();

    currentPlayer = 0;

    playerWalls[0] = 10;
    playerWalls[1] = 10;

    cout << "Game Started!" << endl;
}

void GameController::resetGame() {

    startGame();

    cout << "Game Reset!" << endl;
}


void GameController::setAIDifficulty(int level) {

    // Clamp level between 1 and 3
    if (level < 1) level = 1;
    if (level > 3) level = 3;

    aiPlayer = AIPlayer(level, 1);

    cout << "AI Difficulty Set To: " << level << endl;
}

// Turn Switching
void GameController::switchTurn() {

    currentPlayer = 1 - currentPlayer;
}

// Pawn Movement

bool GameController::movePawn(int newRow, int newCol) {

    vector<pair<int, int>> players = board.getPlayers();

    int currentRow = players[currentPlayer].first;
    int currentCol = players[currentPlayer].second;

    // Validate movement
    if (!isValidPawnMove(currentRow, currentCol,
                         newRow, newCol))
    {
        cout << "Invalid Pawn Move!" << endl;
        return false;
    }

    // Update board
    board.updatePawnPosition(currentPlayer,
                             newRow,
                             newCol);

    cout << "Player "
         << currentPlayer + 1
         << " moved pawn." << endl;

    // Winner check
    if (checkWinner()) {

        cout << "Player "
             << currentPlayer + 1
             << " Wins!" << endl;

        return true;
    }

    // Switch turns
    switchTurn();

    // Trigger AI if needed
    if (mode == GameMode::HUMAN_VS_AI &&
        currentPlayer == 1)
    {
        makeAIMove();
    }

    return true;
}

// Pawn Validation Includes: Normal move - Jump move- Diagonal jump
bool GameController::isValidPawnMove(int currentRow,
                                     int currentCol,
                                     int newRow,
                                     int newCol) const
{
    // Bounds check
    if (!board.isInBounds(newRow, newCol)) {
        return false;
    }

    // Cannot move onto occupied square
    if (board.isOccupied(newRow, newCol)) {
        return false;
    }

    int dr = newRow - currentRow;
    int dc = newCol - currentCol;

    int absDr = abs(dr);
    int absDc = abs(dc);

    // CASE 1:
    // Standard Orthogonal Move
    if ((absDr + absDc) == 1) {

        return !board.isWallBetween(currentRow,
                                    currentCol,
                                    newRow,
                                    newCol);
    }

    // CASE 2:
    // Straight Jump
    if ((absDr == 2 && absDc == 0) ||
        (absDr == 0 && absDc == 2))
    {
        int midRow = currentRow + dr / 2;
        int midCol = currentCol + dc / 2;

        // Opponent must exist in middle
        if (!board.isOccupied(midRow, midCol)) {
            return false;
        }

        // Path to opponent cannot be blocked
        if (board.isWallBetween(currentRow,
                                currentCol,
                                midRow,
                                midCol))
        {
            return false;
        }

        // Jump cannot be blocked
        if (board.isWallBetween(midRow,
                                midCol,
                                newRow,
                                newCol))
        {
            return false;
        }

        return true;
    }

    // CASE 3:
    // Diagonal Jump
if (absDr == 1 && absDc == 1) {

        // Horizontal neighbor case (Opponent is to the left or right)
        if (board.isOccupied(currentRow, newCol)) {

            bool canReachOpponent =
                !board.isWallBetween(currentRow, currentCol,
                                     currentRow, newCol);

            int jumpCol = newCol + (newCol - currentCol);

            // Legal if the straight jump cell is out of bounds OR blocked by a wall
            bool jumpBlocked =
                !board.isInBounds(currentRow, jumpCol) ||
                board.isWallBetween(currentRow, newCol,
                                    currentRow, jumpCol);

            bool canMoveDiagonal =
                !board.isWallBetween(currentRow, newCol,
                                     newRow, newCol);

            if (canReachOpponent && jumpBlocked && canMoveDiagonal) {
                return true;
            }
        }

        // Vertical neighbor case (Opponent is above or below)
        if (board.isOccupied(newRow, currentCol)) {

            bool canReachOpponent =
                !board.isWallBetween(currentRow, currentCol,
                                     newRow, currentCol);

            int jumpRow = newRow + (newRow - currentRow);

            // Legal if the straight jump cell is out of bounds OR blocked by a wall
            bool jumpBlocked =
                !board.isInBounds(jumpRow, currentCol) ||
                board.isWallBetween(newRow, currentCol,
                                    jumpRow, currentCol);

            bool canMoveDiagonal =
                !board.isWallBetween(newRow, currentCol,
                                     newRow, newCol);

            if (canReachOpponent && jumpBlocked && canMoveDiagonal) {
                return true;
            }
        }
    }

    return false;
}
// Wall Placement
bool GameController::placeWall(const Wall& wall) {

    // Remaining walls check
    if (playerWalls[currentPlayer] <= 0) {

        cout << "No walls remaining!" << endl;

        return false;
    }

    // Validate wall
    if (!wallValidator.isValidWall(wall,
                                   board,
                                   pathFinder))
    {
        cout << "Invalid Wall Placement!" << endl;

        return false;
    }

    // Add wall
    board.addWall(wall);

    // Consume wall
    playerWalls[currentPlayer]--;

    cout << "Player "
         << currentPlayer + 1
         << " placed wall." << endl;

    // Switch turns
    switchTurn();

    // Trigger AI
    if (mode == GameMode::HUMAN_VS_AI &&
        currentPlayer == 1)
    {
        makeAIMove();
    }

    return true;
}

// AI Turn

void GameController::makeAIMove() {
    if (mode != GameMode::HUMAN_VS_AI || currentPlayer != 1) return;

    cout << "AI Thinking..." << endl;
    bool placedWall = false;

    //If AI has no walls left, force it to step or choose move safely
    if (playerWalls[1] > 0) {
        placedWall = aiPlayer.takeTurn(board, wallValidator, pathFinder);
    } else {
        // AI has no walls left, pick best pawn movement manually
        pair<int, int> bestMove = aiPlayer.chooseMove(board, pathFinder, wallValidator);
        board.updatePawnPosition(1, bestMove.first, bestMove.second);
    }

    if (placedWall) {
        playerWalls[1]--;
        cout << "AI placed wall." << endl;
    } else {
        cout << "AI moved pawn." << endl;
    }

    if (checkWinner()) {
        cout << "AI Wins!" << endl;
        return;
    }

    switchTurn();
}

// Winner Detection

bool GameController::checkWinner() const {

    vector<pair<int, int>> players =
        board.getPlayers();

    // Player 1 reaches bottom
    if (players[0].first ==
        board.size - 1)
    {
        return true;
    }

    // Player 2 reaches top
    if (players[1].first == 0) {
        return true;
    }

    return false;
}

int GameController::getWinner() const {

    vector<pair<int, int>> players =
        board.getPlayers();

    if (players[0].first ==
        board.size - 1)
    {
        return 0;
    }

    if (players[1].first == 0) {
        return 1;
    }

    return -1;
}

// Mode Management

void GameController::setMode(GameMode newMode) {

    mode = newMode;
}
// Getters

Board& GameController::getBoard() {

    return board;
}

const Board& GameController::getBoard() const {

    return board;
}

int GameController::getCurrentPlayer() const {

    return currentPlayer;
}

GameMode GameController::getMode() const {

    return mode;
}

int GameController::getRemainingWalls(int playerIndex) const {

    if (playerIndex < 0 ||
        playerIndex > 1)
    {
        return -1;
    }

    return playerWalls[playerIndex];
}
// GUI Move Highlighting Helper
vector<pair<int, int>> GameController::getValidMoves() const {
    vector<pair<int, int>> validMoves;
    vector<pair<int, int>> players = board.getPlayers();
    int currRow = players[currentPlayer].first;
    int currCol = players[currentPlayer].second;

    // Secure search parameters within the dynamic bounds [0, board.size - 1]
    int startRow = max(0, currRow - 2);
    int endRow = min(board.size - 1, currRow + 2);
    int startCol = max(0, currCol - 2);
    int endCol = min(board.size - 1, currCol + 2);

    for (int r = startRow; r <= endRow; ++r) {
        for (int c = startCol; c <= endCol; ++c) {
            if (isValidPawnMove(currRow, currCol, r, c)) {
                validMoves.push_back({r, c});
            }
        }
    }
    return validMoves;
}
