#include "Room.h"

VirtualRoom::VirtualRoom(uint_t x, uint_t y, Side left, Side top, int treasure) :
    _treasure(treasure), _position(x, y), _left(left), _top(top)
{
}

VirtualRoom::~VirtualRoom()
{
    //dtor
}
//checking if there is an external door to a room
bool VirtualRoom::hasExternalDoor() const
{
    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        return false;
    }
    if (left() == Side::DOOR) {
        if (_position.x == 0 || maze->getRoom(_position.x - 1, _position.y)->isHole()) {
            return true;
        }
    }

    if (right() == Side::DOOR) {
        if (_position.x == maze->nx() - 1 || maze->getRoom(_position.x + 1, _position.y)->isHole()) {
            return true;
        }
    }

    if (bottom() == Side::DOOR) {
        if (_position.y == maze->ny() - 1 || maze->getRoom(_position.x, _position.y + 1)->isHole()) {
            return true;
        }
    }

    if (top() == Side::DOOR) {
        if (_position.y == 0 || maze->getRoom(_position.x, _position.y - 1)->isHole()) {
            return true;
        }
    }

    return false;

}
void VirtualRoom::fillVertical(strBuffer& buffer, uint_t i0, uint_t j0, Side s) const
{
    char c = ' ';
    if (s == Side::WALL) {
        c = '|';
    }
    else if (s == Side::DOOR) {
        c = '.';
    }
    buffer(i0, j0) = buffer(i0, j0 + CELL_SY + 1) = '*';
    buffer.vline(i0, j0 + 1, CELL_SY, c);
}
void VirtualRoom::fillHorizontal(strBuffer& buffer, uint_t i0, uint_t j0, Side s) const
{
    char c = ' ';
    if (s == Side::WALL) {
        c = '-';
    }
    else if (s == Side::DOOR) {
        c = '.';
    }
    buffer(i0, j0) = buffer(i0 + CELL_SX + 1, j0) = '*';
    buffer.hline(i0 + 1, j0, CELL_SX, c);
}
void VirtualRoom::fillBuffer(strBuffer& buffer) const
{
    Position p = corner();
    if (isHole()) {
        buffer.rect(p.x + 1, p.y + 1, CELL_SX, CELL_SY);
    }
    if (getTreasure() > 0) {
        buffer.txt(p.x + 2, p.y + 2, getTreasure());
    }
}


// -------------------- NormalRoom -------------------------//
NormalRoom::NormalRoom(uint_t x, uint_t y, Side left, Side top, int treasure) : VirtualRoom(x, y, left, top, treasure)
{
}

NormalRoom::~NormalRoom()
{
}
Side NormalRoom::right() const {
    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        return INVALID;
    }
    VirtualRoom* room = maze->getRoom(_position.x + 1, _position.y);
    return room == nullptr ? INVALID : room->left();
}

Side NormalRoom::bottom() const {
    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        return INVALID;
    }
    VirtualRoom* room = maze->getRoom(_position.x, _position.y + 1);
    return room == nullptr ? INVALID : room->top();
}

void NormalRoom::fillBuffer(strBuffer& buffer) const
{
    VirtualRoom::fillBuffer(buffer);
    Position p = corner();
    fillHorizontal(buffer, p.x, p.y, top());
    fillVertical(buffer, p.x, p.y, left());
}


// -------------------- HRoom -------------------------//
HRoom::HRoom(uint_t x, uint_t y, Side left, Side top, Side right, int treasure) : NormalRoom(x, y, left, top, treasure), _right(right)
{
}

HRoom::~HRoom()
{
}
Side HRoom::right() const {
    return _right;
}

void HRoom::fillBuffer(strBuffer& buffer) const
{
    NormalRoom::fillBuffer(buffer);
    Position p = corner();
    fillVertical(buffer, p.x + CELL_SX + 1, p.y, right());
}


// -------------------- VRoom -------------------------//
VRoom::VRoom(uint_t x, uint_t y, Side left, Side top, Side bottom, int treasure) : NormalRoom(x, y, left, top, treasure), _bottom(bottom)
{
}

VRoom::~VRoom()
{
}
Side VRoom::bottom() const {
    return _bottom;
}

void VRoom::fillBuffer(strBuffer& buffer) const
{
    NormalRoom::fillBuffer(buffer);
    Position p = corner();
    fillHorizontal(buffer, p.x, p.y + CELL_SY + 1, bottom());
}

// -------------------- HVRoom -------------------------//
HVRoom::HVRoom(uint_t x, uint_t y, Side left, Side top, Side right, Side bottom, int treasure) : VirtualRoom(x, y, left, top, treasure), _right(right), _bottom(bottom)
{
}

HVRoom::~HVRoom()
{
}
Side HVRoom::right() const {
    return _right;
}

Side HVRoom::bottom() const {
    return _bottom;
}

void HVRoom::fillBuffer(strBuffer& buffer) const
{
    VirtualRoom::fillBuffer(buffer);
    Position p = corner();
    fillHorizontal(buffer, p.x, p.y, top());
    fillVertical(buffer, p.x, p.y, left());
    fillHorizontal(buffer, p.x, p.y + CELL_SY + 1, bottom());
    fillVertical(buffer, p.x + CELL_SX + 1, p.y, right());
}
