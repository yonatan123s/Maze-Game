#ifndef ROOM_H
#define ROOM_H

#include "General_types.h"
#include "Maze.h"



class VirtualRoom
{
public:
    VirtualRoom(uint_t x, uint_t y, Side left, Side top, int treasure = 0);
    virtual ~VirtualRoom();
    Side left() const { return _left; }
    Side top() const { return _top; }
    virtual Side right() const = 0;
    virtual Side bottom() const = 0;
    void setTreasure(int treasure) { _treasure = treasure; }
    int getTreasure() const { return _treasure; }
    bool isHole() const { return _treasure < 0; }
    bool hasExternalDoor() const;
    virtual void fillBuffer(strBuffer& buffer) const;
    Position position() const { return _position; }
    Position corner() const { return Position((CELL_SX + 1) * _position.x, (CELL_SY + 1) * _position.y); }
protected:
    void fillVertical(strBuffer& buffer, uint_t i0, uint_t j0, Side s) const;
    void fillHorizontal(strBuffer& buffer, uint_t i0, uint_t j0, Side s) const;
    int _treasure = 0;
    Position _position;
private:
    Side _left;
    Side _top;
};

class NormalRoom : public VirtualRoom
{
public:
    NormalRoom(uint_t x, uint_t y, Side left, Side top, int treasure = 0);
    virtual ~NormalRoom();
    virtual Side right() const;
    virtual Side bottom() const;
    virtual void fillBuffer(strBuffer& buffer) const;
};

class HRoom : public NormalRoom
{
private:
    Side _right;
public:
    HRoom(uint_t x, uint_t y, Side left, Side top, Side right, int treasure = 0);
    virtual ~HRoom();
    virtual Side right() const;
    virtual void fillBuffer(strBuffer& buffer) const;
};

class VRoom : public NormalRoom
{
private:
    Side _bottom;
public:
    VRoom(uint_t x, uint_t y, Side left, Side top, Side bottom, int treasure = 0);
    virtual ~VRoom();
    virtual Side bottom() const;
    virtual void fillBuffer(strBuffer& buffer) const;
};

class HVRoom : public VirtualRoom
{
private:
    Side _right;
    Side _bottom;
public:
    HVRoom(uint_t x, uint_t y, Side left, Side top, Side right, Side bottom, int treasure = 0);
    virtual ~HVRoom();
    virtual Side right() const;
    virtual Side bottom() const;
    virtual void fillBuffer(strBuffer& buffer) const;
};

#endif // ROOM_H
