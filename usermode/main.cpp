//    :::     ::: ::::::::::: :::::::: ::::::::::: ::::::::  ::::    :::
//   :+:     :+:     :+:    :+:    :+:    :+:    :+:    :+: :+:+:   :+: 
//  +:+     +:+     +:+    +:+           +:+    +:+    +:+ :+:+:+  +:+  
// +#+     +:+     +#+    +#++:++#++    +#+    +#+    +:+ +#+ +:+ +#+   
// +#+   +#+      +#+           +#+    +#+    +#+    +#+ +#+  +#+#+#    
// #+#+#+#       #+#    #+#    #+#    #+#    #+#    #+# #+#   #+#+#     
//  ###     ########### ######## ########### ########  ###    ####      

#include "DMA.hpp"
#include "Socket.hpp"
#include "WebRadar.hpp"

// int main(void)
// {
//     DMA dma;
//     Socket socket;

//     if (!socket.client_init("0.0.0.0", 3000)
//                 || !dma.dma_init()
//                 || !dma.process_init("cs2.exe")) {
//         return EXIT_FAILURE;
//     }

//     WebRadar webradar;
//     webradar.run(&dma, &socket);

//     return EXIT_SUCCESS;
// }


/* socket tester */
int main(void)
{
    Socket socket;
    if (!socket.client_init("127.0.0.1", 4000)) {
        return EXIT_FAILURE;
    }

    GameInformations game;
    game.map_name = "mirage";

    for (uint8_t player_idx = 0; player_idx < 10; player_idx++) {
        game.players[player_idx].name = "intrusions";
        game.players[player_idx].team = ct;
        game.players[player_idx].color = green;
        game.players[player_idx].is_dead = false;
        game.players[player_idx].position_x = 30.f;
        game.players[player_idx].position_y = 60.f;
    }

    if (!socket.send_game_informations(&game)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
