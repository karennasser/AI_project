#ifndef GAMEBOARDWIDGET_H
#define GAMEBOARDWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QLabel>
#include <QStack>
#include "Board.h"
#include "PathFinder.h"
#include "WallValidator.h"
#include "AIPlayer.h"

struct GameState {
    std::vector<std::pair<int,int>> playersPos;
    std::vector<Wall> placedWalls;
    int p1Walls;
    int p2Walls;
};

class GameBoardWidget : public QGraphicsView {
    Q_OBJECT

private:
    QGraphicsScene *scene;
    Board *logicBoard;
    PathFinder pathFinder;
    WallValidator validator;
    AIPlayer *aiOpponent;

    int TILE_SIZE;
    const int GAP_SIZE = 15;

    QGraphicsEllipseItem *player0Pawn;
    QGraphicsEllipseItem *player1Pawn;

    QLabel *turnLabel;
    QLabel *movesLabel;
    QLabel *p1WallsLabel;
    QLabel *p2WallsLabel;

    int currentTurn;
    int totalMoves;
    int p1WallsRemaining;
    int p2WallsRemaining;

    QStack<GameState> undoStack;
    QStack<GameState> redoStack;

    void switchTurn();
    void executeAITurn();
    void saveCurrentStateToUndo();

    std::vector<std::pair<int, int>> highlightedMoves; // To store moves to draw
    void updateHighlights(); // Helper to calculate highlights

public:
    GameBoardWidget(Board *board, QLabel *turnLbl, QLabel *movesLbl, QLabel *p1Walls, QLabel *p2Walls, QWidget *parent = nullptr);
    void renderBoard();
    void resetGame(int newSize);
    void setAIDifficulty(int level);
    void setTileSize(int newPixelSize);

    bool isHumanVsAI = true;

    void undoLastMove();
    void redoNextMove();

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // GAMEBOARDWIDGET_H