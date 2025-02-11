#pragma once

#include <iostream>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <nlohmann/json.hpp>

#include "Game.hpp"

class Socket
{
private:
    int32_t sock = -1;
    sockaddr_in server;

public:
    Socket() = default;
    virtual ~Socket();

    bool connect_to(const std::string ip, const uint32_t port);
    bool send_game_informations(const Game *data);
};
