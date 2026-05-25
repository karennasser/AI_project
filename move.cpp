#include "move.h"

Move::Move(int fRow, int fCol, int tRow, int tCol) {
    fromRow = fRow;
    fromCol = fCol;
    toRow = tRow;
    toCol = tCol;
}

// Validates if the move is exactly one orthogonal step (up, down, left, or right)
bool Move::isValidBasic() const {
    int rowDiff = std::abs(toRow - fromRow);
    int colDiff = std::abs(toCol - fromCol);

    // A valid basic move must change either the row by 1 or the column by 1, never both (no diagonal moves)
    return (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1);
}