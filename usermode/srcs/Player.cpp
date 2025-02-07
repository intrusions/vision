#include "Player.hpp"

bool Player::find_player_name(DMA *dma, const uintptr_t player_controller)
{
    uint64_t player_name_addr;
    if (!dma->read_process_memory(
                        player_controller + player_controller::m_sSanitizedPlayerName,
                        &player_name_addr,
                        sizeof(player_name_addr)))
        return false;
    
    if (!player_name_addr)
        return false;

    char player_name_tmp[128];
    if (!dma->read_process_memory(
                        player_name_addr,
                        &player_name_tmp,
                        sizeof(player_name_tmp)))
        return false;

    name = player_name_tmp;
    return true;
}

bool Player::find_player_health(DMA *dma, const uintptr_t cs_player_pawn)
{
    int32_t player_health_tmp;
    if (!dma->read_process_memory(
                        cs_player_pawn + base_entity::m_iHealth,
                        &player_health_tmp,
                        sizeof(player_health_tmp)))
        return false;

    health = player_health_tmp;
    return true;
}

bool Player::find_player_color(DMA *dma, const uintptr_t player_controller)
{
    int32_t player_color_tmp;
    if (!dma->read_process_memory(
                        player_controller + player_controller::m_iCompTeammateColor,
                        &player_color_tmp,
                        sizeof(player_color_tmp)))
        return false;

    color = player_color_tmp;
    return true;
}

bool Player::find_player_position(DMA *dma, const uintptr_t cs_player_pawn)
{
    Vec3 player_position_tmp;
    if (!dma->read_process_memory(
                        cs_player_pawn + base_player_pawn::m_vOldOrigin,
                        &player_position_tmp,
                        sizeof(player_position_tmp)))
        return false;

    position = player_position_tmp;
    return true;
}

bool Player::find_player_is_alive(DMA *dma, const uintptr_t player_controller)
{
    bool player_is_alive_tmp;
    if (!dma->read_process_memory(
                        player_controller + player_controller::m_bPawnIsAlive,
                        &player_is_alive_tmp,
                        sizeof(player_is_alive_tmp)))
        return false;

    is_alive = player_is_alive_tmp;
    return true;
}

bool Player::find_player_team(DMA *dma, const uintptr_t cs_player_pawn)
{
    uint8_t player_team_tmp;
    if (!dma->read_process_memory(
                        cs_player_pawn + base_entity::m_iTeamNum,
                        &player_team_tmp,
                        sizeof(player_team_tmp)))
        return false;

    team = player_team_tmp;
    return true;
}

std::string Player::get_name()
{
    return name;
}

int32_t Player::get_health()
{
    return health;
}

int32_t     Player::get_color()
{
    return color;
}

Vec3     Player::get_position()
{
    return position;
}

bool     Player::get_is_alive()
{
    return is_alive;
}

uint8_t     Player::get_team()
{
    return team;
}
