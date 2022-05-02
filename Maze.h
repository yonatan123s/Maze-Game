#ifndef MAZE_H
#define MAZE_H

#include <string>
#include <ostream>
#include "General_types.h"
#include "Player.h"


class VirtualRoom;


class Maze
{
private:
    typedef Bidim<VirtualRoom*> room_matrix_t;
    Maze(const std::string& description);
    virtual ~Maze();
    static Maze* gMaze;
    room_matrix_t _rooms;
    static Player _player[2];
public:
    static bool createMaze(const std::string& description);
    static Maze* getMaze() { return gMaze; }
    VirtualRoom* getRoom(Position p) const { return _rooms(p.x, p.y); }
    VirtualRoom* getRoom(uint_t i, uint_t j) const { return _rooms(i, j); }
    VirtualRoom* getRoom(uint_t n) const { return _rooms[n]; }
    uint_t nx() const { return _rooms.nx(); }
    uint_t ny() const { return _rooms.ny(); }
    uint_t nbRooms() const { return _rooms.n(); }
    static Player& player(uint_t i) { return _player[i]; }
    Position randomStart() const;
    bool hasPosition(const Position& pos) const { return pos.x < nx() && pos.y < ny(); }
    void distributeTreasure();
    static void initPlayers();
};
std::ostream& operator<<(std::ostream& o, const Maze& maze);


#endif // MAZE_H
