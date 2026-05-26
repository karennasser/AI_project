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
    // 1. Logic Reset
    if (logicBoard) {
        delete logicBoard;
    }
    logicBoard = new Board(newSize);

    // 2. State Reset
    currentTurn = 0;
    totalMoves = 0;
    p1WallsRemaining = 10;
    p2WallsRemaining = 10;

    undoStack.clear();
    redoStack.clear();

    // 3. UI Label Reset (Crucial for the sidebar to update)
    if (turnLabel) turnLabel->setText("Current Turn:\nPlayer 1 (Red)");
    if (movesLabel) movesLabel->setText("Total Moves: 0");
    if (p1WallsLabel) p1WallsLabel->setText("Walls Remaining: 10");
    if (p2WallsLabel) p2WallsLabel->setText("Walls Remaining: 10");

    // 4. Refresh Visuals
    updateHighlights();
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

    updateHighlights();
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

    // 1. Draw grid squares
    for (int r = 0; r < dynamicSize; ++r) {
        for (int c = 0; c < dynamicSize; ++c) {
            int x = c * (TILE_SIZE + GAP_SIZE);
            int y = r * (TILE_SIZE + GAP_SIZE);

            QGraphicsRectItem *tile = scene->addRect(x, y, TILE_SIZE, TILE_SIZE);
            tile->setPen(QPen(Qt::NoPen));

            // --- CORRECTED HIGHLIGHT PART ---
            bool isHighlighted = false;
            for (auto& m : highlightedMoves) {
                if (m.first == r && m.second == c) isHighlighted = true;
            }

            if (isHighlighted) {
                tile->setBrush(QBrush(QColor(144, 238, 144, 180))); // Green highlight
            } else {
                tile->setBrush(QBrush(QColor(224, 224, 224))); // Default grey
            }
            // --------------------------------
        }
    }

    // 2. Draw placed walls (unchanged logic, just ensure it renders)
    const auto& placedWalls = logicBoard->getPlacedWalls();
    for (const auto& wall : placedWalls) {
        int x = wall.x * (TILE_SIZE + GAP_SIZE);
        int y = wall.y * (TILE_SIZE + GAP_SIZE);
        QGraphicsRectItem *wallItem = nullptr;
        if (wall.isHorizontal())
            wallItem = scene->addRect(x, y + TILE_SIZE, (TILE_SIZE * 2) + GAP_SIZE, GAP_SIZE);
        else
            wallItem = scene->addRect(x + TILE_SIZE, y, GAP_SIZE, (TILE_SIZE * 2) + GAP_SIZE);

        if (wallItem) {
            wallItem->setBrush(QBrush(QColor(255, 152, 0)));
            wallItem->setPen(QPen(Qt::NoPen));
        }
    }

    // 3. Draw Pawns (Ensure both are drawn)
    auto p1 = logicBoard->getPlayer1Pos();
    auto p2 = logicBoard->getPlayer2Pos();
    scene->addEllipse(p1.second * (TILE_SIZE + GAP_SIZE) + 5, p1.first * (TILE_SIZE + GAP_SIZE) + 5, TILE_SIZE - 10, TILE_SIZE - 10, QPen(Qt::NoPen), QBrush(QColor(211, 47, 47)));
    scene->addEllipse(p2.second * (TILE_SIZE + GAP_SIZE) + 5, p2.first * (TILE_SIZE + GAP_SIZE) + 5, TILE_SIZE - 10, TILE_SIZE - 10, QPen(Qt::NoPen), QBrush(QColor(25, 118, 210)));
}

void GameBoardWidget::mousePressEvent(QMouseEvent *event) {
    // Only block if it is the AI's turn
    if (isHumanVsAI && currentTurn == 1) return;

    QPointF clickPos = mapToScene(event->pos());
    int blockWidth = TILE_SIZE + GAP_SIZE;
    int col = (int)clickPos.x() / blockWidth;
    int row = (int)clickPos.y() / blockWidth;
    int insideX = (int)clickPos.x() % blockWidth;
    int insideY = (int)clickPos.y() % blockWidth;

    bool actionTaken = false;
    int& currentWalls = (currentTurn == 0) ? p1WallsRemaining : p2WallsRemaining;
    QLabel* wallLabel = (currentTurn == 0) ? p1WallsLabel : p2WallsLabel;

    // A. Wall Placement
    if (insideX < TILE_SIZE && insideY < TILE_SIZE) {
        if (logicBoard->isInBounds(row, col)) {
            for (auto& move : highlightedMoves) {
                if (move.first == row && move.second == col) {
                    saveCurrentStateToUndo();
                    redoStack.clear(); // --- CHANGE 2: CLEAR REDO ---
                    logicBoard->updatePawnPosition(currentTurn, row, col);
                    actionTaken = true;
                    break;
                }
            }
        }
    }
    // B. Pawn Movement (Uses Highlighting for validation)
    else if (currentWalls > 0) {
        // Vertical Wall: Clicked in the vertical gap (right of tile) but NOT in the corner junction
        if (insideX >= TILE_SIZE && insideY < TILE_SIZE) {
            if (col < logicBoard->size - 1 && row < logicBoard->size - 1) {
                Wall newWall(col, row, Orientation::VERTICAL);
                if (validator.isValidWall(newWall, *logicBoard, pathFinder)) {
                    saveCurrentStateToUndo();
                    redoStack.clear();
                    logicBoard->addWall(newWall);
                    currentWalls--;
                    wallLabel->setText("Walls Remaining: " + QString::number(currentWalls));
                    actionTaken = true;
                }
            }
        } else if (insideY >= TILE_SIZE && insideX < TILE_SIZE) {
            if (row < logicBoard->size - 1 && col < logicBoard->size - 1) {
                Wall newWall(col, row, Orientation::HORIZONTAL);
                if (validator.isValidWall(newWall, *logicBoard, pathFinder)) {
                    saveCurrentStateToUndo();
                    redoStack.clear();
                    logicBoard->addWall(newWall);
                    currentWalls--;
                    wallLabel->setText("Walls Remaining: " + QString::number(currentWalls));
                    actionTaken = true;
                }
            }
        }
    }

    if (actionTaken) {
        totalMoves++;
        movesLabel->setText("Total Moves: " + QString::number(totalMoves));

        // Win Condition Check
        bool p1Win = (logicBoard->getPlayer1Pos().first == logicBoard->size - 1);
        bool p2Win = (logicBoard->getPlayer2Pos().first == 0);

        if (p1Win || p2Win) {
            renderBoard(); // Final draw
            QString winnerName = p1Win ? "Player 1 (Red)" : (isHumanVsAI ? "AI Opponent" : "Player 2 (Blue)");
            QMessageBox::information(this, "Game Over", winnerName + " has won the match!");
            resetGame(logicBoard->size);
            return;
        }

        switchTurn();
    }
}
void GameBoardWidget::switchTurn() {
    currentTurn = (currentTurn == 0) ? 1 : 0;

    if (currentTurn == 0) {
        turnLabel->setText("Current Turn:\nPlayer 1 (Red)");
    } else {
        turnLabel->setText(isHumanVsAI ? "Current Turn:\nAI (Blue)" : "Current Turn:\nPlayer 2 (Blue)");
    }

    updateHighlights(); // Recalculate for the new turn
    renderBoard();

    if (isHumanVsAI && currentTurn == 1) {
        executeAITurn();
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
void GameBoardWidget::updateHighlights() {
    highlightedMoves.clear();
    if (!logicBoard) return;

    auto players = logicBoard->getPlayers();
    std::pair<int, int> curr = players[currentTurn];
    std::pair<int, int> opp = players[1 - currentTurn];

    int dr[] = {-1, 1, 0, 0}; // Up, Down, Left, Right
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = curr.first + dr[i];
        int nc = curr.second + dc[i];

        if (!logicBoard->isInBounds(nr, nc)) continue;
        if (logicBoard->isWallBetween(curr.first, curr.second, nr, nc)) continue;

        if (nr == opp.first && nc == opp.second) {
            int jr = nr + dr[i];
            int jc = nc + dc[i];

            if (logicBoard->isInBounds(jr, jc) && !logicBoard->isWallBetween(nr, nc, jr, jc)) {
                highlightedMoves.push_back({jr, jc}); // Straight Jump
            } else {
                // Diagonal Jump logic
                // If moving Up/Down (i < 2), check Left/Right (2, 3).
                // If moving Left/Right (i >= 2), check Up/Down (0, 1).
                std::vector<int> diagSides = (i < 2) ? std::vector<int>{2, 3} : std::vector<int>{0, 1};

                for (int d : diagSides) {
                    int dnr = nr + dr[d];
                    int dnc = nc + dc[d];
                    if (logicBoard->isInBounds(dnr, dnc) && !logicBoard->isWallBetween(nr, nc, dnr, dnc)) {
                        highlightedMoves.push_back({dnr, dnc});
                    }
                }
            }
        } else {
            highlightedMoves.push_back({nr, nc}); // Normal Move
        }
    }
}