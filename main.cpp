#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>
#include "Board.h"
#include "gameboardwidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Global Stylesheet for a Professional Dark Theme
    a.setStyleSheet(
        "QWidget { font-family: 'Segoe UI', sans-serif; color: #E0E0E0; }"
        "QFrame#Sidebar { background-color: #252526; border-left: 2px solid #333333; }"
        "QLabel#SectionTitle { color: #FF9800; font-size: 16px; font-weight: bold; border-bottom: 1px solid #333333; padding-bottom: 5px; margin-top: 10px; }"
        "QLabel#StatusLabel { font-size: 13px; color: #AAAAAA; }"
        "QComboBox { background-color: #333333; border: 1px solid #444444; border-radius: 4px; padding: 5px; color: white; }"
        "QComboBox:hover { border: 1px solid #FF9800; }"
        "QPushButton { background-color: #3E3E42; border: none; border-radius: 4px; padding: 8px; font-weight: bold; color: white; }"
        "QPushButton:hover { background-color: #505050; }"
        "QPushButton#RestartBtn { background-color: #A12621; }"
        "QPushButton#RestartBtn:hover { background-color: #C62828; }"
        "QPushButton#UndoBtn { background-color: #2D3D45; }"
        "QPushButton#UndoBtn:hover { background-color: #37474F; }"
        );

    Board *myLogicBoard = new Board(9);
    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("Quoridor AI Master 2026");
    mainWindow->resize(1200, 850);
    mainWindow->setStyleSheet("background-color: #1E1E1E;");

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWindow);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ---- SIDEBAR ----
    QFrame *sidebar = new QFrame();
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(300);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20, 20, 20, 20);
    sidebarLayout->setSpacing(15);

    // 1. SETTINGS SECTION
    QLabel *settingsHeader = new QLabel("GAME SETTINGS");
    settingsHeader->setObjectName("SectionTitle");
    sidebarLayout->addWidget(settingsHeader);

    auto addLabeledCombo = [&](QString labelText, QStringList items) -> QComboBox* {
        QLabel *lbl = new QLabel(labelText);
        lbl->setStyleSheet("color: #888888; font-size: 11px; text-transform: uppercase;");
        QComboBox *cb = new QComboBox();
        cb->addItems(items);
        sidebarLayout->addWidget(lbl);
        sidebarLayout->addWidget(cb);
        return cb;
    };

    QComboBox *modeBox = addLabeledCombo("Match Mode", {"Human vs AI", "Human vs Human"});
    QComboBox *sizeBox = addLabeledCombo("Grid Dimensions", {"9 x 9 (Standard)", "7 x 7 (Blitz)", "5 x 5 (Mini)"});
    QComboBox *tileBox = addLabeledCombo("Board Zoom", {"Medium", "Small", "Large"});
    QComboBox *diffBox = addLabeledCombo("AI Intellect", {"Easy", "Medium", "Hard"});

    // 2. STATUS SECTION
    QLabel *statusHeader = new QLabel("MATCH STATUS");
    statusHeader->setObjectName("SectionTitle");
    sidebarLayout->addWidget(statusHeader);

    QLabel *turnLabel = new QLabel("Current Turn: Player 1");
    turnLabel->setStyleSheet("color: #FFFFFF; font-size: 14px; font-weight: bold;");
    sidebarLayout->addWidget(turnLabel);

    QLabel *movesLabel = new QLabel("Total Moves: 0");
    movesLabel->setObjectName("StatusLabel");
    sidebarLayout->addWidget(movesLabel);

    // Wall Stats Group
    QFrame *wallStats = new QFrame();
    QVBoxLayout *wallLayout = new QVBoxLayout(wallStats);
    wallLayout->setContentsMargins(0,0,0,0);

    QLabel *p1WallsLabel = new QLabel("P1 (Red) Walls: 10");
    p1WallsLabel->setStyleSheet("color: #F44336; font-weight: bold;");
    QLabel *p2WallsLabel = new QLabel("P2 (Blue) Walls: 10");
    p2WallsLabel->setStyleSheet("color: #2196F3; font-weight: bold;");

    wallLayout->addWidget(p1WallsLabel);
    wallLayout->addWidget(p2WallsLabel);
    sidebarLayout->addWidget(wallStats);

    sidebarLayout->addStretch(); // Important: This pushes the buttons to the very bottom

    // 3. ACTION BUTTONS
    QPushButton *undoButton = new QPushButton("UNDO MOVE");
    undoButton->setObjectName("UndoBtn");
    sidebarLayout->addWidget(undoButton);

    QPushButton *redoButton = new QPushButton("REDO MOVE");
    sidebarLayout->addWidget(redoButton);

    QPushButton *restartButton = new QPushButton("RESTART MATCH");
    restartButton->setObjectName("RestartBtn");
    sidebarLayout->addWidget(restartButton);

    // ---- BOARD VIEW ----
    GameBoardWidget *boardWidget = new GameBoardWidget(myLogicBoard, turnLabel, movesLabel, p1WallsLabel, p2WallsLabel);
    mainLayout->addWidget(boardWidget, 1); // 1 = stretch factor, board takes remaining space
    mainLayout->addWidget(sidebar);

    // --- CONNECTIONS (Logical Wiring) ---
    QObject::connect(modeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        boardWidget->isHumanVsAI = (index == 0);
        boardWidget->resetGame(sizeBox->currentIndex() == 1 ? 7 : (sizeBox->currentIndex() == 2 ? 5 : 9));
    });

    QObject::connect(sizeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        int s = (index == 1) ? 7 : (index == 2 ? 5 : 9);
        boardWidget->resetGame(s);
    });

    QObject::connect(tileBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        boardWidget->setTileSize(index == 1 ? 40 : (index == 2 ? 65 : 52));
    });

    QObject::connect(diffBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        boardWidget->setAIDifficulty(index + 1);
    });

    QObject::connect(undoButton, &QPushButton::clicked, boardWidget, &GameBoardWidget::undoLastMove);
    QObject::connect(redoButton, &QPushButton::clicked, boardWidget, &GameBoardWidget::redoNextMove);
    QObject::connect(restartButton, &QPushButton::clicked, [=]() {
        int s = (sizeBox->currentIndex() == 1) ? 7 : (sizeBox->currentIndex() == 2 ? 5 : 9);
        boardWidget->resetGame(s);
    });

    mainWindow->show();
    return a.exec();
}