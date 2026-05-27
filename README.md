This is a professionally structured README.md file for your Quoridor project.
You can copy this text directly into a file named README.md in your project
folder.

♟️ Quoridor AI Master 2026

A sophisticated, modern implementation of the classic strategy board game
Quoridor, built using C++ and the Qt Framework. This version features a
high-performance AI opponent powered by the Minimax algorithm with Alpha-Beta
pruning.

📝 Game Description

Quoridor is a tactical game where the objective is simple: be the first to move
your pawn to any square on the opponent's starting side.

On your turn, you must choose between:

1.  Moving your pawn to an adjacent square.
2.  Placing a wall to block your opponent's progress.

The catch? You have a limited number of walls, and you can never completely
block an opponent; there must always be at least one valid path to their goal.

Key Features

  - Intelligent AI: Three difficulty levels (Easy, Medium, Hard) using Minimax
    search.
  - Dynamic Board Sizes: Play on standard 9x9, or fast-paced 7x7 and 5x5 boards.
  - Game Modes: Local Human vs. Human or Human vs. AI.
  - Modern UI: A clean, dark-themed dashboard with real-time move highlighting
    and wall counters.
  - History System: Full Undo and Redo support for reviewing strategies.

📸 Screenshots

| Main Gameplay                                                                                                                                     |
| :-----------------------------------------------------------------------------------------------------------------------------------------------: |
| ![Main Board](<img width="908" height="800" alt="image" src="https://github.com/user-attachments/assets/0f1d073e-a11c-4282-b361-47a0a44ec0bb" />) |
| *Pawn movement and highlights*                                                                                                                    |                                           

 Sidebar Controls                                                                                                                                |
|:--------------------------------------------------------------------------------------------------------------------------------------------:  |
| ![Sidebar](<img width="371" height="992" alt="image" src="https://github.com/user-attachments/assets/c804b130-acd0-4842-a10c-0bc2dd508560" />) |
| *Settings and move history*                                                                                                                    |


🛠️ Installation & Running Instructions

Prerequisites

  - Qt 6.x (or Qt 5.15+)
  - C++17 compatible compiler (GCC, Clang, or MSVC)
  - Qt Creator (Recommended IDE)

Setup

1.  Clone the Repository:
    git clone https://github.com/yourusername/Quoridor-AI-2026.git
    cd Quoridor-AI-2026
2.  Open Project:
      - Launch Qt Creator.
      - Go to File > Open File or Project... and select Quoridor.pro.
3.  Configure Project:
      - Select your preferred kit (e.g., Desktop Qt 6.5.0 MinGW).
4.  Build and Run:
      - Press Ctrl + R (Build and Run) to launch the game.

🎮 Controls Explanation

The game is designed with an intuitive, mouse-only interface:

Pawn Movement

  - Click on a Tile: If a tile is highlighted in Green, simply click it to move
    your pawn there.
  - Jump Rules: If you are adjacent to your opponent, the game automatically
    calculates valid straight or diagonal jumps according to Quoridor rules.

Wall Placement

  - Click on the Gaps: Click on the spaces (gutters) between the square tiles to
    place a wall.
      - Vertical Walls: Click the gap to the right of a square.
      - Horizontal Walls: Click the gap below a square.
  - Invalid Placements: The game will automatically block you from placing walls
    that overlap or completely trap a player.

UI Buttons

  - Undo/Redo: Correct a mistake or re-apply a move.
  - Restart: Instantly wipe the board and start a fresh match with your current
    settings.
  - Settings Dropdowns: Change game mode or AI difficulty in real-time.

🎥 Demo Video

Watch the AI Master 2026 in action, showcasing pawn jumps, strategic wall
placements, and "Hard" AI difficulty:

👉 Watch the Demo Video Here

👤 Author

  - Your Name                -code
  - karen nasser abd almlak  -2300354
  - Marina George Boushra    -2300509
