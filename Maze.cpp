#include "Maze.h"
#include "Room.h"

#include <exception>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

Maze* Maze::gMaze = nullptr;
Player Maze::_player[2];

//building a maze
Maze::Maze(const string& description)  
{
    size_t p = description.find(":");
    if (p == string::npos) {
        throw invalid_argument("format need to start with NxM: and semicolon not found");
    }

    string sSize = description.substr(0, p);
    string sDescr = description.substr(p + 1);

    p = sSize.find('x');
    if (p == string::npos) {
        throw invalid_argument("format need to start with NxM: and x not found");
    }

    uint_t nx = stoi(sSize.substr(0, p));
    uint_t ny = stoi(sSize.substr(p + 1));

    _rooms.redim(nx, ny);
    _rooms.fill(nullptr);

    vector<string> cells;
    while ((p = sDescr.find(',')) != string::npos) {
        string s = sDescr.substr(0, p);
        cells.push_back(s);
        sDescr = sDescr.substr(p + 1);
    }
    cells.push_back(sDescr);

    if (cells.size() != nx * ny) {
        throw invalid_argument("wrong number of elements");
    }

    for (uint_t n = 0; n < cells.size(); ++n) {
        // first letter : H or F (Hole or Full)
        const string& s = cells[n];

        if (s.length() < 1) {
            throw invalid_argument("element empty");
        }
        int treasure = 0;
        if (s[0] == 'H') {
            treasure = -1;
        }
        else if (s[0] != 'F') {
            throw invalid_argument("element must starts with H or F");
        }


        Position pos = _rooms.n_ij(n);
        uint_t i = pos.x;
        uint_t j = pos.y;

        // create the room at position (i,j)
        VirtualRoom* room = nullptr;

        if ((i == nx - 1) && (j == ny - 1)) {
            // HVRoom
            if (s.length() != 5) {
                throw invalid_argument("HV room : must have 5 chars");
            }
            Side left = strSide(s[1]);
            Side top = strSide(s[2]);
            Side right = strSide(s[3]);
            Side bottom = strSide(s[4]);
            room = new HVRoom(i, j, left, top, right, bottom, treasure);
        }
        else if (i == nx - 1) {
            if (s.length() != 4) {
                throw invalid_argument("H room : must have 4 chars");
            }
            Side left = strSide(s[1]);
            Side top = strSide(s[2]);
            Side right = strSide(s[3]);;
            room = new HRoom(i, j, left, top, right, treasure);
        }
        else if (j == ny - 1) {
            if (s.length() != 4) {
                throw invalid_argument("V room : must have 4 chars");
            }
            Side left = strSide(s[1]);
            Side top = strSide(s[2]);
            Side bottom = strSide(s[3]);;
            room = new VRoom(i, j, left, top, bottom, treasure);
        }
        else {
            if (s.length() != 3) {
                throw invalid_argument("normal room : must have 3 chars");
            }
            Side left = strSide(s[1]);
            Side top = strSide(s[2]);
            room = new NormalRoom(i, j, left, top, treasure);
        }
        _rooms(i, j) = room;
    }
}

Maze::~Maze()
{
    _rooms.apply_rw_nopos([](VirtualRoom*& room)->void { if (room != nullptr) { delete room; room = nullptr; } });
}
//alocating the players
Position Maze::randomStart() const
{
    vector<uint_t> candidates;
    for (uint_t n = 0; n < nbRooms(); ++n) {
        VirtualRoom* room = getRoom(n);
        if (room != nullptr && !room->isHole() && room->hasExternalDoor()) {
            candidates.push_back(n);
        }
    }


    std::random_device os_seed;
    const uint_t seed = os_seed();
    using engine = std::mt19937;
    engine generator(seed);
    std::uniform_int_distribution< uint_t > distribute(0, candidates.size() - 1);
    uint_t i = distribute(generator);
    VirtualRoom* room = getRoom(candidates[i]);
    return room->position();
}


void Maze::initPlayers()
{
    _player[0].setName("A");
    _player[1].setName("B");
}
//adding randomly the treasure
void Maze::distributeTreasure()
{

    vector<uint_t> candidates;
    for (uint_t n = 0; n < nbRooms(); ++n) {
        VirtualRoom* room = getRoom(n);
        if (room != nullptr && !room->isHole() && _player[0].position() != room->position() && _player[1].position() != room->position()) {
            candidates.push_back(n);
        }
    }

    std::random_device os_seed;
    const uint_t seed = os_seed();
    using engine = std::mt19937;
    engine generator(seed);
    std::uniform_int_distribution< uint_t > distNb(1, candidates.size() / 2);
    std::uniform_int_distribution< int > distPrice(1, 100);
    uint_t nb = distNb(generator);
    std::uniform_int_distribution< uint_t > distPlace(0, candidates.size() - 1);
    for (uint_t i = 0; i < nb; ++i) {
        int price = distPrice(generator);
        uint_t n = candidates[i];
        VirtualRoom* room = getRoom(n);
        if (room != nullptr && !room->isHole()) {
            room->setTreasure(price);
        }
    }
}

//creating a specific maze
bool Maze::createMaze(const std::string& description)
{
    if (gMaze != nullptr) {
        delete gMaze;
        gMaze = nullptr;
    }
    try {
        gMaze = new Maze(description);
    }
    catch (exception& E) {
        cerr << E.what() << endl;
        gMaze = nullptr;
        return false;
    }
    for (int i = 0; i < 2; ++i) {
        Position position = gMaze->randomStart();
        Player& player = gMaze->player(i);
        player.clear();
        player.moveTo(position);
    }
    gMaze->distributeTreasure();

    return true;
}

std::ostream& operator<<(std::ostream& o, const Maze& maze)
{
    strBuffer buffer((CELL_SX + 1) * maze.nx() + 1, (CELL_SY + 1) * maze.ny() + 1);
    buffer.fill(' ');
    for (uint_t j = 0; j < maze.ny(); ++j) for (uint_t i = 0; i < maze.nx(); ++i) {
        VirtualRoom* room = maze.getRoom(i, j);
        if (room != nullptr) {
            room->fillBuffer(buffer);
        }
    }
    for (int i = 0; i < 2; ++i) {
        const Player& player = maze.player(i);
        Position p = player.position();
        VirtualRoom* room = maze.getRoom(p);
        Position txtPos = room->corner() + Position(1 + i, 3);
        buffer.txt(txtPos.x, txtPos.y, player.getName());
    }
    o << buffer;
    return o;
}
