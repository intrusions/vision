#pragma once

#include <iostream>
#include <cstdint>

#include "DMA.hpp"
#include "offsets.hpp"

namespace client            = cs2_dumper::offsets::client_dll;
namespace engine            = cs2_dumper::offsets::engine2_dll;
namespace matchmaking       = cs2_dumper::offsets::matchmaking_dll;
namespace base_entity       = cs2_dumper::offsets::client_dll::C_BaseEntity;
namespace player_pawn       = cs2_dumper::offsets::client_dll::C_CSPlayerPawn;
namespace player_controller = cs2_dumper::offsets::client_dll::CCSPlayerController;
namespace base_player_pawn  = cs2_dumper::offsets::client_dll::C_BasePlayerPawn;
namespace game_scene_node   = cs2_dumper::offsets::client_dll::CGameSceneNode;

struct Vec3
{
    float x;
    float y;
    float z;
};

class Player
{
private:
    std::string name;
    int32_t     health;
    int32_t     color;
    Vec3        position;
    bool        is_alive;
    uint8_t     team;

public:
    Player() = default;
    virtual ~Player() = default;

    bool find_player_name(DMA *dma, const uintptr_t player_controller);
    bool find_player_color(DMA *dma, const uintptr_t player_controller);
    bool find_player_is_alive(DMA *dma, const uintptr_t player_controller);
    bool find_player_health(DMA *dma, const uintptr_t cs_player_pawn);
    bool find_player_position(DMA *dma, const uintptr_t cs_player_pawn);
    bool find_player_team(DMA *dma, const uintptr_t cs_player_pawn);

    std::string get_name();
    int32_t     get_health();
    int32_t     get_color();
    Vec3        get_position();
    bool        get_is_alive();
    uint8_t     get_team();

};
