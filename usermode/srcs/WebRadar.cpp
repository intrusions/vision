#include "DMA.hpp"
#include "Socket.hpp"
#include "GameInformations.hpp"
#include "WebRadar.hpp"
#include "offsets.hpp"

bool get_entity_list(DMA *dma, const uintptr_t client_base, uintptr_t *entity_list)
{
    uintptr_t entity_list_tmp;
    if (!dma->read_process_memory(
                        client_base + client::dwEntityList,
                        &entity_list_tmp,
                        sizeof(entity_list_tmp)))
        return false;
    
    *entity_list = entity_list_tmp;
    return true;
}

void WebRadar::run(DMA *dma, Socket *socket)
{
    auto process = dma->get_process();
    uint64_t client_base = process._modules_base_address["client.dll"];

    GameInformations game;
    Vec3 position;

    uintptr_t entity_list;
    if (!get_entity_list(dma, client_base, &entity_list))
        return ;
    
    for (;;) {

        for (auto player_index = 1; player_index <= 10; player_index++) {
            
            uint64_t list_entry;
            if (!dma->read_process_memory(
                                entity_list + (8LL * (player_index & 0x7FFF) >> 9) + 16,
                                &list_entry,
                                sizeof(list_entry)))
                return ;

            uintptr_t player_controller;
            if (!dma->read_process_memory(
                                (list_entry + 120 * (player_index & 0x1FF)),
                                &player_controller,
                                sizeof(player_controller)))
                return ;

            uint32_t player_pawn;
            if (!dma->read_process_memory(
                                player_controller + player_controller::m_hPlayerPawn,
                                &player_pawn,
                                sizeof(player_pawn)))
                return ;

            uintptr_t second_list_entry;
            if (!dma->read_process_memory(
                                entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16,
                                &second_list_entry,
                                sizeof(second_list_entry)))
                return ;
        
            uintptr_t cs_player_pawn;
            if (!dma->read_process_memory(
                                second_list_entry + 120 * (player_pawn & 0x1FF),
                                &cs_player_pawn,
                                sizeof(cs_player_pawn)))
                return ;

            if (!game.players[player_index - 1].find_player_name(dma, player_controller))
                continue ;

            if (!game.players[player_index - 1].find_player_health(dma, cs_player_pawn))
                continue ;

            if (!game.players[player_index - 1].find_player_color(dma, player_controller))
                continue ;

            if (!game.players[player_index - 1].find_player_position(dma, cs_player_pawn))
                continue ;
        }

        socket->send_game_informations(&game);
        usleep(100000);
    }
}
