#pragma once

#include "DMA.hpp"

#define MAX_PLAYERS 10

class WebRadar
{
private:
    struct Player
    {
        std::string name;
        uint8_t     team;
        uint8_t     color;
        bool        is_dead;
        float       position_x;
        float       position_y;
        float       eye_angle;
    };

    Player player;

public:
    WebRadar() = default;
    virtual ~WebRadar() = default;

    void run(DMA *dma, Socket *socket);

};
