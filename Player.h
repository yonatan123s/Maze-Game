#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include "General_types.h"


class Player
{
public:
    enum room_type {INVALID,NOT_IN_MAZE,HOLE,ROOM};
    Player(std::string name = "X");
    virtual ~Player();
    std::string getName() const { return _name; }
    void setName(const std::string& name) { _name = name; }
    room_type getRoomTypeAt(char direction) const;
    void moveTo(const Position& pos) { _movList.push_back(pos); }
    void displacement(const Position& pos) { moveTo(position() + pos); }
    void displacement(char direction) { displacement(displacementPos(direction)); }
    Position position() const { return *(_movList.end() - 1); }
    Position positionAt(char direction) const { return position() + displacementPos(direction); }
    uint_t nearestTreasure(uint_t& nearest_treasure) const;
    uint_t nbSteps() const { return _movList.size() - 1; }
    void addPoint(int pts) { _points += pts; }
    void clearPoints() { _points = 0; }
    int points() const { return _points; }
    bool haveWallTo(int direction) const;
    void clear() { _movList.clear(); nbClues = 0; }
    void addClue() { nbClues++; }
    int getNbClues() { return nbClues; }
protected:

private:
    Position displacementPos(char direction) const;
    std::string _name;
    std::vector<Position> _movList;
    int _points;
    int nbClues;
};

#endif // PLAYER_H
