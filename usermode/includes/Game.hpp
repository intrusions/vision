#pragma once

#include <iostream>
#include <cstdint>
#include <unistd.h>

#include "Player.hpp"
#include "DMA.hpp"

namespace client            = cs2_dumper::offsets::client_dll;
namespace engine            = cs2_dumper::offsets::engine2_dll;
namespace matchmaking       = cs2_dumper::offsets::matchmaking_dll;
namespace base_entity       = cs2_dumper::offsets::client_dll::C_BaseEntity;
namespace player_pawn       = cs2_dumper::offsets::client_dll::C_CSPlayerPawn;
namespace player_controller = cs2_dumper::offsets::client_dll::CCSPlayerController;
namespace base_player_pawn  = cs2_dumper::offsets::client_dll::C_BasePlayerPawn;

class Game
{
private:

public :
    Game() = default;
    virtual ~Game() = default;

    std::string map_name;
    Player players[10] = { };

    bool is_in_game(DMA *dma, const uintptr_t matchmaking_base);
    bool find_map_name(DMA *dma, const uintptr_t matchmaking_base);
};
