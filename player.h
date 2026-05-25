#ifndef PLAYER_H
#define PLAYER_H

// Forward declaration of the Board class to indicate association (Invokes: Board)
class Board;

class Player {
private:
    int id;               // Player identifier (e.g., 0 for Player 1, 1 for Player 2)
    int row;              // Current row coordinate of the pawn
    int col;              // Current column coordinate of the pawn
    int wallsRemaining;   // Number of remaining walls available to place

public:
    // Constructor initializes the player dynamically based on ID and board size
    Player(int id, int boardSize);

    // Core methods required for Player actions
    void move(int newRow, int newCol);
    void useWall();

    // Getters for GUI updates and rule validation
    int getId() const;
    int getRow() const;
    int getCol() const;
    int getWallsRemaining() const;
};

#endif // PLAYER_H