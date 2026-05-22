#ifndef WALL_H
#define WALL_H

using namespace std;

// File: Wall.h
// Represents a 2-cell barrier placed on edges between squares in Quoridor.
// Walls use gap-space coordinates (0–7 on a 9x9 board) at intersection points.
// Orientation: HORIZONTAL blocks vertical movement; VERTICAL blocks horizontal.

// Shared orientation enum — used by Wall, WallValidator, and Board.
enum class Orientation {
    HORIZONTAL,
    VERTICAL
};

class Wall {
public:

    // Attributes

    int x;                  // Column of the wall's anchor intersection (0–7)
    int y;                  // Row of the wall's anchor intersection (0–7)
    Orientation orientation; // Direction: HORIZONTAL or VERTICAL


    // Constructors

    // Create a wall at position (x,y) with given orientation
    Wall(int x, int y, Orientation orientation);

    // Default constructor — creates wall at (0,0) HORIZONTAL
    // Needed for using Wall in standard containers like vector
    Wall();


    // Methods

    // Returns true if this wall is horizontal
    bool isHorizontal() const;

    // Returns true if this wall is vertical
    bool isVertical() const;


    // Equality operator — useful for removing temporary walls

    // Two walls are equal if same position AND orientation
    bool operator==(const Wall& other) const;
};
#endif // WALL_H
