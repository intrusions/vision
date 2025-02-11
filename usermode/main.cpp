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

int main(void)
{
    DMA dma;
    Socket socket;
    WebRadar webradar;

    if (!socket.connect_to("127.0.0.1", 4000)
                || !dma.dma_init()
                || !dma.process_init("cs2.exe")
                || !dma.get_module_base_address("client.dll")
                || !dma.get_module_base_address("engine2.dll")
                || !dma.get_module_base_address("matchmaking.dll")) {
        return EXIT_FAILURE;
    }

    webradar.run(&dma, &socket);

    return EXIT_SUCCESS;
}
