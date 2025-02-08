#include "DMA.hpp"
#include "Socket.hpp"
#include "Game.hpp"
#include "WebRadar.hpp"
#include "offsets.hpp"

void WebRadar::run(DMA *dma, Socket *socket)
{
    Game game;

    auto process = dma->get_process();
    uintptr_t client_base = process._modules_base_address["client.dll"];
    uintptr_t matchmaking_base = process._modules_base_address["matchmaking.dll"];
    uintptr_t engine_base = process._modules_base_address["engine2.dll"];

    while (true) {

        while (!game.is_in_game(dma, engine_base)) ;

        if (!game.find_map_name(dma, matchmaking_base))
            continue ;

        uintptr_t entity_list;
        if (!dma->read_process_memory(
                            client_base + client::dwEntityList,
                            &entity_list,
                            sizeof(entity_list)))
            continue;

        for (auto player_index = 1; player_index <= 10; player_index++) {
            
            uint64_t list_entry;
            if (!dma->read_process_memory(
                                entity_list + (8LL * (player_index & 0x7FFF) >> 9) + 16,
                                &list_entry,
                                sizeof(list_entry)))
                continue ;

            uintptr_t player_controller;
            if (!dma->read_process_memory(
                                (list_entry + 120 * (player_index & 0x1FF)),
                                &player_controller,
                                sizeof(player_controller)))
                continue ;

            if (!player_controller)
                continue ;

            uint32_t player_pawn;
            if (!dma->read_process_memory(
                                player_controller + player_controller::m_hPlayerPawn,
                                &player_pawn,
                                sizeof(player_pawn)))
                continue ;

            uintptr_t second_list_entry;
            if (!dma->read_process_memory(
                                entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16,
                                &second_list_entry,
                                sizeof(second_list_entry)))
                continue ;

            uintptr_t cs_player_pawn;
            if (!dma->read_process_memory(
                                second_list_entry + 120 * (player_pawn & 0x1FF),
                                &cs_player_pawn,
                                sizeof(cs_player_pawn)))
                continue ;

            if (!game.players[player_index - 1].find_player_name(dma, player_controller))
                continue ;

            if (!game.players[player_index - 1].find_player_color(dma, player_controller))
                continue ;
            
            if (!game.players[player_index - 1].find_player_is_alive(dma, player_controller))
                continue ;
            
            if (!game.players[player_index - 1].find_player_health(dma, cs_player_pawn))
                continue ;

            if (!game.players[player_index - 1].find_player_position(dma, cs_player_pawn))
                continue ;

            if (!game.players[player_index - 1].find_player_team(dma, cs_player_pawn))
                continue ;
        }

        socket->send_game_informations(&game);
        usleep(1000000);
    }
}
