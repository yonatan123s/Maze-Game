#include "Player.h"

#include <iostream>

#include "Maze.h"
#include "Room.h"

using namespace std;

Player::Player(std::string name) : _name(name), _movList()
{
    //ctor
}

Player::~Player()
{
    //dtor
}



Position Player::displacementPos(char direction) const
{
    switch (direction) {
    case 'L':
        return Position(-1, 0);
    case 'R':
        return Position(+1, 0);
    case 'D':
        return Position(0, +1);
    case 'B':
        return Position(0, +1);
    case 'U':
        return Position(0, -1);
    case 'T':
        return Position(0, -1);
    default:
        cerr << "Unknown direction " << direction << endl;
        return Position(0, 0);
    }
}
//returnig a room properties
Player::room_type  Player::getRoomTypeAt(char direction) const
{
    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        return INVALID;
    }

    Position pos = displacementPos(direction);

    Position newPos = position() + pos;
    if (!maze->hasPosition(newPos)) {
        return NOT_IN_MAZE;
    }

    VirtualRoom* room = maze->getRoom(newPos);

    if (room == nullptr) {
        return NOT_IN_MAZE;
    }

    if (room->isHole()) {
        return HOLE;
    }

    room = maze->getRoom(position());
    if (room == nullptr) {
        return HOLE;
    }
    return ROOM;

}
//checking if there is a wall
bool Player::haveWallTo(int direction) const
{
    Maze* maze = Maze::getMaze();
    VirtualRoom* room = maze->getRoom(position());

    Side checkedSide = room->left();
    switch (direction) {
    case 'L':
        checkedSide = room->left();
        break;
    case 'R':
        checkedSide = room->right();
        break;
    case 'D':
        checkedSide = room->bottom();
        break;
    case 'B':
        checkedSide = room->bottom();
        break;
    case 'U':
        checkedSide = room->top();
        break;
    case 'T':
        checkedSide = room->top();
        break;
    }
    return checkedSide == Side::WALL;
}
//finding the nearestTreasure and its distance
uint_t Player::nearestTreasure(uint_t& nearest_treasure) const
{
    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        return 1000;
    }
    uint_t nearest = 1000;
    nearest_treasure = 0;
    for (uint_t n = 0; n < maze->nbRooms(); ++n) {
        VirtualRoom* room = maze->getRoom(n);
        if (room == nullptr) continue;
        int treasure = room->getTreasure();
        if (treasure > 0) {
            int dx = (int)room->position().x - (int)position().x;
            int dy = (int)room->position().y - (int)position().y;
            uint_t dist = abs(dx) + abs(dy);
            if (dist < nearest) {
                nearest = dist;
                nearest_treasure = treasure;
            }
        }
    }
    return nearest;
}


