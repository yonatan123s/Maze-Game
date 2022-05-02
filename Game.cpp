#include "game.h"
#include <random>
#include <iostream>
#include <chrono>
#include <algorithm>

#include "general_types.h"
#include "maze.h"
#include "room.h"

using namespace std;

//description of the mazes
const unsigned int Game::NB_GAMES = 5;
const string Game::GAMES[Game::NB_GAMES] = {
    "3x3:FDW,FDW,HDVV,HVW,FWD,FDWW,FWDD,FDWW,FDDDW"  ,
    "3x3:HVV,FDW,HWVV,FDW,FDD,FDWW,HVWV,FWDD,FWDWW"  ,
    "3x3:FDD,FWD,HWVV,FWD,FDD,FWWW,FWDW,FWDW,FDDWW" ,
    "3x3:FWW,FWD,FDWW,FDD,HWW,FWDD,FWDW,FDDD,FDDWW" ,
    "3x3:HVV,HVV,FWWW,FDW,FDW,FWDD,FDDD,FWDW,FDDWW"
};

Game::Game()
{
    //ctor
}

Game::~Game()
{
    //dtor
}

// update the points when the player iplayer step out of the maze
void Game::fail_player(int iplayer, int* points)
{
    Maze* maze = Maze::getMaze();
    Player& player = maze->player(iplayer);
    uint_t iplayer2 = 1 - iplayer;
    Player& player2 = maze->player(iplayer2);

    uint_t treasure2;
    player2.nearestTreasure(treasure2);
    points[iplayer] = - (int)player.nbSteps() - player.getNbClues();
    points[iplayer2] = treasure2 -(int)player2.nbSteps() - player2.getNbClues();
    player.addPoint(points[iplayer]);
    player2.addPoint(points[iplayer2]);

    cout << "YOU FAILED : " << endl;
    cout << "YOUR POINTS : " << points[iplayer] << endl;
    cout << player2.getName() << " POINTS : " << points[iplayer2] << endl;
}
//update the points of the player in the end of a round
void Game::win_player(int iplayer,int* points)
{
    Maze* maze = Maze::getMaze();
    Player& player = maze->player(iplayer);
    uint_t iplayer2 = 1 - iplayer;
    Player& player2 = maze->player(iplayer2);
    VirtualRoom* room = maze->getRoom(player.position());

    int treasure = room->getTreasure();
    points[iplayer] = treasure - (int)player.nbSteps() - player.getNbClues();
    points[iplayer2] = -(int)player2.nbSteps() - player2.getNbClues();
    player.addPoint(points[iplayer]);
    player2.addPoint(points[iplayer2]);

    cout << "YOU FOUND THE TREASURE : " << treasure << endl;
    cout << "YOUR POINTS : " << points[iplayer] << endl;
    cout << player2.getName() << " POINTS : " << points[iplayer2] << endl;
}
int Game::idir(char c) const
{
    switch (c) {
        case 'L':return 0;
        case 'R':return 1;
        case 'U':return 2;
        case 'D':return 3;
    }
    return -1;
}

//type of room
Player::room_type Game::room_type_at(char direction, Player::room_type* roomType)
{
    int d = idir(direction);
    if (d >= 0) {
    return roomType[d];
    }
    else {
        return Player::INVALID;
    }
}
bool Game::check(char c, Player::room_type* roomType)
{
    return room_type_at(c,roomType) == Player::room_type::ROOM;
}


//starting a specific maze game
void Game::startGame(bool debugMode, uint_t game_num)
{
    if (debugMode) cout << "Playing the game #" << game_num + 1 << endl;

    string description = GAMES[game_num];
    if (!Maze::createMaze(description)) {
        cerr << "Unable to create the maze, exiting" << endl;
        return;
    }

    Maze* maze = Maze::getMaze();
    if (maze == nullptr) {
        cerr << "Unable to create the maze, exiting" << endl;
        return;
    }

    if (debugMode) {
        cout << *maze << endl;
    }
    uint_t iplayer = 0;
    int points[2] = { 0,0 };
    while (true) {
        Player& player = maze->player(iplayer);

        Player::room_type roomType[4];
        bool hasWall[4];
        roomType[0] = player.getRoomTypeAt('L');
        roomType[1] = player.getRoomTypeAt('R');
        roomType[2] = player.getRoomTypeAt('U');
        roomType[3] = player.getRoomTypeAt('D');

        hasWall[0] = player.haveWallTo('L');
        hasWall[1] = player.haveWallTo('R');
        hasWall[2] = player.haveWallTo('U');
        hasWall[3] = player.haveWallTo('D');


        string command;
        cout << "Player " + player.getName() + " choose action : ";
        if (!hasWall[0] ) cout << "(L)eft ";
        if (!hasWall[1] ) cout << "(R)ight ";
        if (!hasWall[2] ) cout << "(U)p ";
        if (!hasWall[3] ) cout << "(D)own ";
        cout << "(N)earest, (C)heck+(";
        cout << "L";
        cout << "R";
        cout << "U";
        cout << "D";
        cout << ") or (S)tay :";
        cin >> command;

        transform(command.begin(), command.end(), command.begin(), ::toupper);

        bool actionDone = false;

        if (command.length() == 1) {
            char c = command[0];
            int d = idir(c);
            if ( d>=0 ){
                if (!hasWall[d]) {
                    if (check(c, roomType)) {
                        player.displacement(c);
                        actionDone = true;
                    }
                    else { // player have lost the round
                        fail_player(iplayer, points);
                        break;
                    }
                }
            } else {
                switch (c) {
                case 'N': {
                        uint_t v;
                        uint_t dist = player.nearestTreasure(v);
                        cout << "Nearest treasure is at " << dist << endl;
                        player.addClue();
                        actionDone = true;
                        break;
                    }
                case 'S': {
                        actionDone = true;
                        break;
                    }
                }
            }
        } else if (command.length() == 2 && command[0] == 'C') {
            char c = command[1];
            int d = idir(c);
            if (d>=0){
                if (hasWall[d]) {
                    cout << "There's a wall to go to this direction" << endl;
                }
                switch (room_type_at(c, roomType)) {                    
                    case Player::room_type::INVALID:
                        cout << "INVALID ROOM??" << endl;
                        break;
                    case Player::room_type::HOLE:
                        cout << "HOLE" << endl;
                        break;
                    case Player::room_type::NOT_IN_MAZE:
                        cout << "NOT IN MAZE" << endl;
                        break;
                    case Player::room_type::ROOM: {
                        Position pos = player.positionAt(c);
                        VirtualRoom* room = maze->getRoom(pos);
                        cout << "The treasure at " << c << " is " << room->getTreasure() << endl;
                        
                    }
                }
                actionDone = true;
                player.addClue();
            }
        }

        if (!actionDone) {
            cout << "bad command, please again" << endl;
            continue;
        }

        VirtualRoom* room = maze->getRoom(player.position());
        if (room != nullptr && room->getTreasure() > 0) {
            win_player(iplayer,points);
            break;
        }

        iplayer = 1 - iplayer;


        if (debugMode) {
            cout << *maze << endl;
        }
    }
    if (!debugMode) cout << *maze << endl;
}
//starting a game
void Game::startGame(bool debugMode)
{
    Player& player1 = Maze::player(0);
    Player& player2 = Maze::player(1);
    player1.clearPoints();
    player2.clearPoints();

    vector<int> stillGame;
    for (uint_t game_num = 0; game_num < NB_GAMES; ++game_num) {
        stillGame.push_back(game_num);
    }
    std::random_device os_seed;
    const uint_t seed = os_seed();
    using engine = std::mt19937;
    engine generator(seed);

    uint_t NB_TOUR = 3;
    for (uint_t tour_num = 0; tour_num < NB_TOUR; ++tour_num) {
        if (debugMode) {
            cout << "Still list games: " ;
            for (size_t i = 0; i < stillGame.size(); ++i) cout << stillGame[i] << ",";
            cout << endl;
        }
        std::uniform_int_distribution< size_t > distribute(0, stillGame.size() - 1);
        size_t elem = distribute(generator);
        int game_num = stillGame[elem];
        if (debugMode) {
            cout << "Chosen element " << elem << " which is " << game_num << endl;
        }
        stillGame.erase(stillGame.begin() + elem);
        cout << "Starting tour " << tour_num+1 << endl;
        if (debugMode) {
            cout << "Now, Still list games: ";
            for (size_t i = 0; i < stillGame.size(); ++i) cout << stillGame[i] << ",";
            cout << endl;
        }
        startGame(debugMode, game_num);
    }

    cout << "TOTAL POINTS" << endl;

    cout << player1.getName() << ": got " << player1.points() << " points" << endl;
    cout << player2.getName() << ": got " << player2.points() << " points" << endl;
    if (player1.points() < player2.points()) {
        cout << player2.getName() << " WINS" << endl;
    }
    else if (player1.points() > player2.points()) {
        cout << player1.getName() << " WINS" << endl;
    }
    else {
        cout << "NOBODY WINS" << endl;
    }
}
