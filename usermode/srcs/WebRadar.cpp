#include "DMA.hpp"
#include "Socket.hpp"
#include "WebRadar.hpp"
#include "offsets.hpp"

namespace client            = cs2_dumper::offsets::client_dll;
namespace base_entity       = cs2_dumper::offsets::client_dll::C_BaseEntity;
namespace player_pawn       = cs2_dumper::offsets::client_dll::C_CSPlayerPawn;
namespace player_controller = cs2_dumper::offsets::client_dll::CCSPlayerController;

void WebRadar::run(DMA *dma, Socket *socket)
{
    auto process = dma->get_process();
    uint64_t client_base = process._modules_base_address["client.dll"];

    // uint64_t entity_list;
    // if (!dma->read_process_memory(client_base + client::dwEntityList, &entity_list, sizeof(entity_list)))
    //     return ;

    while (true) {
    
        for (auto player_index = 0; player_index <= MAX_PLAYERS; player_index++) {
 
        }
    }
}
