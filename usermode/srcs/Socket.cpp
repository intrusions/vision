#include "Socket.hpp"

Socket::~Socket()
{
    if (sock != -1) {
        close(sock);
    }
}

bool Socket::client_init(const std::string ip, const uint32_t port)
{
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "socket creation failed" << std::endl;
        return false;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
        std::cerr << "server connection failed" << std::endl;
        return false;
    }

    return true;
}

bool Socket::send_game_informations(const GameInformations *data)
{
    nlohmann::json json = {
        {"map_name", "mirage"},
        {"players", nlohmann::json::array()}
    };

    for (uint8_t player_idx = 0; player_idx < 10; ++player_idx) {
        Player player = data->players[player_idx];

        json["players"].push_back({
            {"name", player.get_name()},
            {"health", player.get_health()},
            {"color", player.get_color()},
            {"position", {
                {"x", player.get_position().x},
                {"y", player.get_position().y},
                {"z", player.get_position().z}
            }}
        });
    }

    std::string json_str = json.dump();
    if (send(sock, json_str.c_str(), json_str.size(), 0) <= 0) {
        std::cerr << "packet sending failed" << std::endl;
        return false;
    }

    return true;
}
