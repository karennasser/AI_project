#ifndef MOVE_H
#define MOVE_H

#include <cmath> // Included for std::abs to compute position differences

class Move {
private:
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;

public:
    // Constructor defines a movement from source coordinates to destination coordinates
    Move(int fRow, int fCol, int tRow, int tCol);

    // Checks basic orthogonal single-step validity (without factoring in walls or jumps)
    bool isValidBasic() const;
};

#endif // MOVE_H