#include "gameboardwidget.h"
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QMessageBox>

GameBoardWidget::GameBoardWidget(Board *board, QLabel *turnLbl, QLabel *movesLbl, QLabel *p1Walls, QLabel *p2Walls, QWidget *parent)
    : QGraphicsView(parent), logicBoard(board), turnLabel(turnLbl), movesLabel(movesLbl), p1WallsLabel(p1Walls), p2WallsLabel(p2Walls) {

    scene = new QGraphicsScene(this);
    this->setScene(scene);
    this->setRenderHint(QPainter::Antialiasing);
    scene->setBackgroundBrush(QBrush(QColor(46, 46, 46))); // Smooth dark background for drawing scene

    TILE_SIZE = 50; // Default standard square pixel size
    aiOpponent = new AIPlayer(1, 1);
    resetGame(9);
}

void GameBoardWidget::resetGame(int newSize) {
    delete logicBoard;
    logicBoard = new Board(newSize);

    currentTurn = 0;
    totalMoves = 0;
    p1WallsRemaining = 10;
    p2WallsRemaining = 10;

    undoStack.clear();
    redoStack.clear();

    turnLabel->setText("Current Turn:\nPlayer 1 (Red)");
    movesLabel->setText("Total Moves: 0");
    p1WallsLabel->setText("Walls Remaining: 10");
    p2WallsLabel->setText("Walls Remaining: 10");

    renderBoard();
}

void GameBoardWidget::setAIDifficulty(int level) {
    if (aiOpponent) {
        aiOpponent->difficulty = level;
    }
}

void GameBoardWidget::setTileSize(int newPixelSize) {
    TILE_SIZE = newPixelSize;
    renderBoard();
}

void GameBoardWidget::saveCurrentStateToUndo() {
    GameState state;
    state.playersPos = logicBoard->getPlayers();
    state.placedWalls = logicBoard->getPlacedWalls();
    state.p1Walls = p1WallsRemaining;
    state.p2Walls = p2WallsRemaining;
    undoStack.push(state);
}

void GameBoardWidget::undoLastMove() {
    if (undoStack.isEmpty()) return;

    GameState currentState;
    currentState.playersPos = logicBoard->getPlayers();
    currentState.placedWalls = logicBoard->getPlacedWalls();
    currentState.p1Walls = p1WallsRemaining;
    currentState.p2Walls = p2WallsRemaining;
    redoStack.push(currentState);

    GameState prevState = undoStack.pop();
    logicBoard->players = prevState.playersPos;
    logicBoard->walls = prevState.placedWalls;
    p1WallsRemaining = prevState.p1Walls;
    p2WallsRemaining = prevState.p2Walls;

    totalMoves = std::max(0, totalMoves - 1);
    movesLabel->setText("Total Moves: " + QString::number(totalMoves));

    currentTurn = (currentTurn == 0) ? 1 : 0;
    turnLabel->setText(currentTurn == 0 ? "Current Turn:\nPlayer 1 (Red)" : "Current Turn:\nPlayer 2 (AI)");

    renderBoard();
}

void GameBoardWidget::redoNextMove() {
    if (redoStack.isEmpty()) return;

    saveCurrentStateToUndo();

    GameState nextState = redoStack.pop();
    logicBoard->players = nextState.playersPos;
    logicBoard->walls = nextState.placedWalls;
    p1WallsRemaining = nextState.p1Walls;
    p2WallsRemaining = nextState.p2Walls;

    totalMoves++;
    movesLabel->setText("Total Moves: " + QString::number(totalMoves));

    currentTurn = (currentTurn == 0) ? 1 : 0;
    turnLabel->setText(currentTurn == 0 ? "Current Turn:\nPlayer 1 (Red)" : "Current Turn:\nPlayer 2 (AI)");

    renderBoard();
}

void GameBoardWidget::renderBoard() {
    scene->clear();
    int dynamicSize = logicBoard->size;

    // 1. Draw empty grid squares
    for (int r = 0; r < dynamicSize; ++r) {
        for (int c = 0; c < dynamicSize; ++c) {
            int x = c * (TILE_SIZE + GAP_SIZE);
            int y = r * (TILE_SIZE + GAP_SIZE);

            QGraphicsRectItem *tile = scene->addRect(x, y, TILE_SIZE, TILE_SIZE);
            tile->setBrush(QBrush(QColor(224, 224, 224)));
            tile->setPen(QPen(Qt::NoPen));
        }
    }

    // 2. Draw placed walls barrier overlays
    const auto& placedWalls = logicBoard->getPlacedWalls();
    for (const auto& wall : placedWalls) {
        int x = wall.x * (TILE_SIZE + GAP_SIZE);
        int y = wall.y * (TILE_SIZE + GAP_SIZE);

        QGraphicsRectItem *wallItem = nullptr;
        if (wall.isHorizontal()) {
            int wallY = y + TILE_SIZE;
            int wallLength = (TILE_SIZE * 2) + GAP_SIZE;
            wallItem = scene->addRect(x, wallY, wallLength, GAP_SIZE);
        } else {
            int wallX = x + TILE_SIZE;
            int wallLength = (TILE_SIZE * 2) + GAP_SIZE;
            wallItem = scene->addRect(wallX, y, GAP_SIZE, wallLength);
        }

        if (wallItem) {
            wallItem->setBrush(QBrush(QColor(255, 152, 0)));
            wallItem->setPen(QPen(Qt::NoPen));
        }
    }

    // 3. Draw Player 1 Pawn (Red) - Fixed: Starts at Top (Row 0)
    auto p0 = logicBoard->getPlayer1Pos();
    int p0X = p0.second * (TILE_SIZE + GAP_SIZE) + (TILE_SIZE / 10);
    int p0Y = p0.first * (TILE_SIZE + GAP_SIZE) + (TILE_SIZE / 10);
    player0Pawn = scene->addEllipse(p0X, p0Y, TILE_SIZE - (TILE_SIZE / 5), TILE_SIZE - (TILE_SIZE / 5));
    player0Pawn->setBrush(QBrush(QColor(211, 47, 47)));
    player0Pawn->setPen(QPen(Qt::NoPen));

    // 4. Draw Player 2 Pawn (Blue) - Fixed: Starts at Bottom (Last Row)
    auto p1 = logicBoard->getPlayer2Pos();
    int p1X = p1.second * (TILE_SIZE + GAP_SIZE) + (TILE_SIZE / 10);
    int p1Y = p1.first * (TILE_SIZE + GAP_SIZE) + (TILE_SIZE / 10);
    player1Pawn = scene->addEllipse(p1X, p1Y, TILE_SIZE - (TILE_SIZE / 5), TILE_SIZE - (TILE_SIZE / 5));
    player1Pawn->setBrush(QBrush(QColor(25, 118, 210)));
    player1Pawn->setPen(QPen(Qt::NoPen));
}

void GameBoardWidget::mousePressEvent(QMouseEvent *event) {
    if (currentTurn == 1) return;

    QPointF clickPos = mapToScene(event->pos());
    int clickX = clickPos.x();
    int clickY = clickPos.y();

    int blockWidth = TILE_SIZE + GAP_SIZE;
    int col = clickX / blockWidth;
    int row = clickY / blockWidth;

    int insideX = clickX % blockWidth;
    int insideY = clickY % blockWidth;

    bool actionTaken = false;

    // A. Handle Wall Selection Clicks
    if (insideX >= TILE_SIZE && insideY < TILE_SIZE && col < logicBoard->size - 1 && row < logicBoard->size - 1) {
        if (p1WallsRemaining > 0) {
            Wall newWall(col, row, Orientation::VERTICAL);
            if (validator.isValidWall(newWall, *logicBoard, pathFinder)) {
                saveCurrentStateToUndo();
                redoStack.clear();
                logicBoard->addWall(newWall);
                p1WallsRemaining--;
                p1WallsLabel->setText("Walls Remaining: " + QString::number(p1WallsRemaining));
                actionTaken = true;
            }
        }
    }
    else if (insideY >= TILE_SIZE && insideX < TILE_SIZE && col < logicBoard->size - 1 && row < logicBoard->size - 1) {
        if (p1WallsRemaining > 0) {
            Wall newWall(col, row, Orientation::HORIZONTAL);
            if (validator.isValidWall(newWall, *logicBoard, pathFinder)) {
                saveCurrentStateToUndo();
                redoStack.clear();
                logicBoard->addWall(newWall);
                p1WallsRemaining--;
                p1WallsLabel->setText("Walls Remaining: " + QString::number(p1WallsRemaining));
                actionTaken = true;
            }
        }
    }
    // B. Handle Pawn Selection Move Clicks
    else if (insideX < TILE_SIZE && insideY < TILE_SIZE && logicBoard->isInBounds(row, col)) {
        auto currentPos = logicBoard->getPlayer1Pos();
        if (!logicBoard->isWallBetween(currentPos.first, currentPos.second, row, col) && !logicBoard->isOccupied(row, col)) {
            int rDiff = std::abs(row - currentPos.first);
            int cDiff = std::abs(col - currentPos.second);
            if ((rDiff == 1 && cDiff == 0) || (rDiff == 0 && cDiff == 1)) {
                saveCurrentStateToUndo();
                redoStack.clear();
                logicBoard->updatePawnPosition(0, row, col);
                actionTaken = true;
            }
        }
    }

    if (actionTaken) {
        totalMoves++;
        movesLabel->setText("Total Moves: " + QString::number(totalMoves));
        renderBoard();

        // FIXED: Crystal Clear White Style Custom Popup Message for Player Win Condition
        if (logicBoard->getPlayer1Pos().first == logicBoard->size - 1) {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("Congrats");
            msgBox.setText("🎉 Congratulations! Player 1 (Red) Wins! 🎉");
            msgBox.setStyleSheet(
                "QMessageBox { background-color: #2d2d2d; }"
                "QLabel { color: white; font-size: 16px; font-weight: bold; }"
                "QPushButton { background-color: #37474f; color: white; padding: 6px; border-radius: 4px; min-width: 70px; }"
                );
            msgBox.exec();
            return;
        }
        switchTurn();
    }
    QGraphicsView::mousePressEvent(event);
}

void GameBoardWidget::switchTurn() {
    if (currentTurn == 0) {
        currentTurn = 1;
        turnLabel->setText("Current Turn:\nPlayer 2 (AI)");
        executeAITurn();
    } else {
        currentTurn = 0;
        turnLabel->setText("Current Turn:\nPlayer 1 (Red)");
    }
}

void GameBoardWidget::executeAITurn() {
    GameState preAIState;
    preAIState.playersPos = logicBoard->getPlayers();
    preAIState.placedWalls = logicBoard->getPlacedWalls();
    preAIState.p1Walls = p1WallsRemaining;
    preAIState.p2Walls = p2WallsRemaining;
    undoStack.push(preAIState);

    bool placedWall = aiOpponent->takeTurn(*logicBoard, validator, pathFinder);

    if (placedWall) {
        p2WallsRemaining--;
        p2WallsLabel->setText("Walls Remaining: " + QString::number(p2WallsRemaining));
    }

    totalMoves++;
    movesLabel->setText("Total Moves: " + QString::number(totalMoves));
    renderBoard();

    if (logicBoard->getPlayer2Pos().first == 0) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Game Over");
        msgBox.setText("🤖 AI Player 2 (Blue) Wins! Better luck next time!");
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #2d2d2d; }"
            "QLabel { color: white; font-size: 16px; font-weight: bold; }"
            "QPushButton { background-color: #37474f; color: white; padding: 6px; border-radius: 4px; min-width: 70px; }"
            );
        msgBox.exec();
        return;
    }
    switchTurn();
}