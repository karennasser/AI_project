#include "Wall.h"


// Constructors

Wall::Wall(int x, int y, Orientation orientation)
    : x(x), y(y), orientation(orientation)
{}

Wall::Wall()
    : x(0), y(0), orientation(Orientation::HORIZONTAL)
{}


// Methods

bool Wall::isHorizontal() const {
    return orientation == Orientation::HORIZONTAL;
}

bool Wall::isVertical() const {
    return orientation == Orientation::VERTICAL;
}


// Equality operator

bool Wall::operator==(const Wall& other) const {
    return x == other.x &&
           y == other.y &&
           orientation == other.orientation;
}
