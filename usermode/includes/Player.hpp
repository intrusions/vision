#pragma once

#include <iostream>
#include <cstdint>

enum e_team : uint8_t
{
    none,
    spec,
    t,
    ct
};

enum e_colors : uint8_t
{
    blue,
    green,
    yellow,
    orange,
    purple,
    white
};

class GameInformations
{
private:
    struct Player
    {
        std::string name;
        e_team      team;
        e_colors    color;
        bool        is_dead;
        float       position_x;
        float       position_y;
        float       eye_angle;
    };

public :
    GameInformations() = default;
    virtual ~GameInformations() = default;

    std::string map_name;
    Player players[10] = { };

};
