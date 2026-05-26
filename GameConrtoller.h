#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Board.h"
#include "Wall.h"
#include "WallValidator.h"
#include "PathFinder.h"
#include "AIPlayer.h"

enum class GameMode {
    HUMAN_VS_HUMAN,
    HUMAN_VS_AI
};

class GameController {

private:
    Board board;
    WallValidator wallValidator;
    PathFinder pathFinder;
    AIPlayer aiPlayer;
    int currentPlayer;
    int playerWalls[2];
    GameMode mode;

public:

    GameController();
    explicit GameController(GameMode mode);
    void startGame();
    void resetGame();
    void switchTurn();
    void makeAIMove();
    void setAIDifficulty(int level);
    bool movePawn(int newRow, int newCol);
    bool isValidPawnMove(int currentRow,
                         int currentCol,
                         int newRow,
                         int newCol) const;

    bool placeWall(const Wall& wall);

    bool checkWinner() const;

    int getWinner() const;

    void setMode(GameMode newMode);

    Board& getBoard();

    const Board& getBoard() const;

    int getCurrentPlayer() const;

    GameMode getMode() const;

    int getRemainingWalls(int playerIndex) const;
    std::vector<std::pair<int, int>> getValidMoves() const;
};

#endif // GAMECONTROLLER_H
