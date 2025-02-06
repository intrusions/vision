#pragma once

#include <iostream>
#include <cstdint>

#include "Player.hpp"

class GameInformations
{
private:

public :
    GameInformations() = default;
    virtual ~GameInformations() = default;

    std::string map_name;
    Player players[10] = { };

};
