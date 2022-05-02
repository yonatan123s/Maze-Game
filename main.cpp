#include <iostream>
#include <string>
#include "Maze.h"
#include "Room.h"
#include "Game.h"
#include "General_types.h"

using namespace std;


int main()
{

    Maze::initPlayers();
    bool debug = false;
    while (true) {
        string s;
        cout << " (D)ebug or (N)ormal" << endl;
        cin >> s;
        if (s == "D" || s == "d") {
            debug = true;
            break;
        }
        if (s == "N" || s == "n") {
            debug = false;
            break;
        }
    }

    Game game;
    game.startGame(debug);

    return 0;
}
