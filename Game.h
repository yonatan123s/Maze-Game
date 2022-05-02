#ifndef GAME_H
#define GAME_H

#include <string>
#include"General_types.h"
#include "Player.h"

class Game
{
public:
    static const unsigned int NB_GAMES;
    static const std::string GAMES[];
    Game();
    virtual ~Game();

    void startGame(bool debugMode, uint_t game_num);
    void startGame(bool debugMode = false);

private:
    int idir(char c) const;
    Player::room_type room_type_at(char direction, Player::room_type* roomType);
    bool check(char c, Player::room_type* roomType);
    int totPoints[2] = { 0,0 };
    void fail_player(int iplayer, int* points);
    void win_player(int iplayer, int* points);
};

#endif // GAME_H
