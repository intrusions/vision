#include "Game.hpp"

bool Game::find_map_name(DMA *dma, const uintptr_t matchmaking_base)
{
    uintptr_t map_name_addr;
    if (!dma->read_process_memory(
                        matchmaking_base + matchmaking::dwGameTypes + matchmaking::dwGameTypes_mapName,
                        &map_name_addr,
                        sizeof(map_name_addr)))
        return false;

    char map_name_tmp[32];
    if (!dma->read_process_memory(
                        map_name_addr + 0x4,
                        &map_name_tmp,
                        sizeof(map_name_tmp)))
        return false;

    map_name = std::string(map_name_tmp);
    return true;
}

bool Game::is_in_game(DMA *dma, const uintptr_t engine_base)
{
    uintptr_t network_client_ptr;
    if (!dma->read_process_memory(
                        engine_base + engine::dwNetworkGameClient,
                        &network_client_ptr,
                        sizeof(network_client_ptr)))
            return false;

    bool is_background;
    if (!dma->read_process_memory(
                        network_client_ptr + engine::dwNetworkGameClient_isBackgroundMap,
                        &is_background,
                        sizeof(is_background)))
            return false;

    int32_t state;
    if (!dma->read_process_memory(
                        network_client_ptr + engine::dwNetworkGameClient_signOnState,
                        &state,
                        sizeof(state)))
            return false;

    return (!is_background && state >= 6);
}
