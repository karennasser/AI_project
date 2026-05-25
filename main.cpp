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

    Board *myLogicBoard = new Board(9);

    QWidget *mainWindow = new QWidget();
    mainWindow->setWindowTitle("Quoridor Game - Ultimate Bonus Pack");
    mainWindow->resize(1000, 720);
    mainWindow->setStyleSheet("background-color: #1e1e1e;");

    QHBoxLayout *mainLayout = new QHBoxLayout(mainWindow);

    QFrame *sidebar = new QFrame();
    sidebar->setFixedWidth(260);
    sidebar->setStyleSheet("background-color: #2d2d2d; border-radius: 10px; padding: 10px;");
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);

    QLabel *titleLabel = new QLabel("MATCH STATUS");
    titleLabel->setStyleSheet("color: #ff9800; font-size: 18px; font-weight: bold;");
    sidebarLayout->addWidget(titleLabel, 0, Qt::AlignCenter);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #444444;");
    sidebarLayout->addWidget(line);

    // ---- Settings Controls (Dropdowns) ----
    QLabel *sizeTitle = new QLabel("Board Grid Size:");
    sizeTitle->setStyleSheet("color: #ff9800; font-weight: bold; margin-top: 5px;");
    sidebarLayout->addWidget(sizeTitle);

    QComboBox *sizeBox = new QComboBox();
    sizeBox->addItems({"9 x 9 (Standard)", "7 x 7 (Blitz)", "5 x 5 (Mini)"});
    sizeBox->setStyleSheet("background-color: #424242; color: white; padding: 4px; border-radius: 3px;");
    sidebarLayout->addWidget(sizeBox);

    // ---- NEW: Tile Size Customizer Dropdown ----
    QLabel *tileTitle = new QLabel("Square Tile Zoom:");
    tileTitle->setStyleSheet("color: #ff9800; font-weight: bold; margin-top: 10px;");
    sidebarLayout->addWidget(tileTitle);

    QComboBox *tileBox = new QComboBox();
    tileBox->addItems({"Medium (50px)", "Small (40px)", "Large (60px)"});
    tileBox->setStyleSheet("background-color: #424242; color: white; padding: 4px; border-radius: 3px;");
    sidebarLayout->addWidget(tileBox);

    QLabel *diffTitle = new QLabel("AI Difficulty:");
    diffTitle->setStyleSheet("color: #ff9800; font-weight: bold; margin-top: 10px;");
    sidebarLayout->addWidget(diffTitle);

    QComboBox *diffBox = new QComboBox();
    diffBox->addItems({"Easy ", "Medium ", "Hard "});
    diffBox->setStyleSheet("background-color: #424242; color: white; padding: 4px; border-radius: 3px;");
    sidebarLayout->addWidget(diffBox);

    QFrame *lineSettings = new QFrame();
    lineSettings->setFrameShape(QFrame::HLine);
    lineSettings->setStyleSheet("color: #444444; margin-top: 10px;");
    sidebarLayout->addWidget(lineSettings);

    // Statistics Monitor Display
    QLabel *turnLabel = new QLabel();
    turnLabel->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold; margin-top: 5px;");
    sidebarLayout->addWidget(turnLabel);

    QLabel *movesLabel = new QLabel();
    movesLabel->setStyleSheet("color: #b0bec5; font-size: 14px; margin-top: 5px;");
    sidebarLayout->addWidget(movesLabel);

    QLabel *p1Title = new QLabel("Player 1 (Red)");
    p1Title->setStyleSheet("color: #d32f2f; font-size: 14px; font-weight: bold; margin-top: 10px;");
    sidebarLayout->addWidget(p1Title);

    QLabel *p1WallsLabel = new QLabel();
    p1WallsLabel->setStyleSheet("color: #ffffff; font-size: 13px;");
    sidebarLayout->addWidget(p1WallsLabel);

    QLabel *p2Title = new QLabel("Player 2 (Blue)");
    p2Title->setStyleSheet("color: #1976d2; font-size: 14px; font-weight: bold; margin-top: 10px;");
    sidebarLayout->addWidget(p2Title);

    QLabel *p2WallsLabel = new QLabel();
    p2WallsLabel->setStyleSheet("color: #ffffff; font-size: 13px;");
    sidebarLayout->addWidget(p2WallsLabel);

    sidebarLayout->addStretch();

    // Game Action Buttons
    QPushButton *undoButton = new QPushButton("Undo Move");
    undoButton->setStyleSheet("background-color: #37474f; color: white; font-weight: bold; padding: 6px; border-radius: 5px;");
    sidebarLayout->addWidget(undoButton);

    QPushButton *redoButton = new QPushButton("Redo Move");
    redoButton->setStyleSheet("background-color: #4f5b62; color: white; font-weight: bold; padding: 6px; border-radius: 5px; margin-top: 3px;");
    sidebarLayout->addWidget(redoButton);

    QPushButton *restartButton = new QPushButton("Restart Match");
    restartButton->setStyleSheet("background-color: #c62828; color: white; font-weight: bold; padding: 6px; border-radius: 5px; margin-top: 5px;");
    sidebarLayout->addWidget(restartButton);

    // Render Widget Instance Setup
    GameBoardWidget *boardWidget = new GameBoardWidget(myLogicBoard, turnLabel, movesLabel, p1WallsLabel, p2WallsLabel);
    mainLayout->addWidget(boardWidget);
    mainLayout->addWidget(sidebar);

    // ---- Connections Wiring ----

    QObject::connect(sizeBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        int selectedSize = 9;
        if (index == 1) selectedSize = 7;
        if (index == 2) selectedSize = 5;
        boardWidget->resetGame(selectedSize);
    });

    // Connect New Tile Pixel Size Customizer Dropdown
    QObject::connect(tileBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        int targetPixelSize = 50; // Medium
        if (index == 1) targetPixelSize = 40; // Small
        if (index == 2) targetPixelSize = 60; // Large
        boardWidget->setTileSize(targetPixelSize); // Updates visual scale instantly
    });

    QObject::connect(diffBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        boardWidget->setAIDifficulty(index + 1);
    });

    QObject::connect(undoButton, &QPushButton::clicked, boardWidget, &GameBoardWidget::undoLastMove);
    QObject::connect(redoButton, &QPushButton::clicked, boardWidget, &GameBoardWidget::redoNextMove);

    QObject::connect(restartButton, &QPushButton::clicked, [=]() {
        int currentSize = 9;
        if (sizeBox->currentIndex() == 1) currentSize = 7;
        if (sizeBox->currentIndex() == 2) currentSize = 5;
        boardWidget->resetGame(currentSize);
    });

    mainWindow->show();
    return a.exec();
}