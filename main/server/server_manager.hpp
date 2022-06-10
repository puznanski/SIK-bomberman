#ifndef ROBOTS_SERVER_MANAGER_HPP
#define ROBOTS_SERVER_MANAGER_HPP

#include <boost/asio.hpp>
#include <utility>
#include <thread>

#include "server_options.hpp"
#include "server_game_state.hpp"

class ServerManager {
public:
    explicit ServerManager(ServerOptions server_options);

private:
    ServerOptions server_options;
    ServerGameState server_game_state;

public:
    void start();
};

#endif //ROBOTS_SERVER_MANAGER_HPP
