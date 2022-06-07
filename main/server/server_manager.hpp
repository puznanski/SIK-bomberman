#ifndef ROBOTS_SERVER_MANAGER_HPP
#define ROBOTS_SERVER_MANAGER_HPP

#include <utility>

#include "server_options.hpp"

class ServerManager {
public:
    explicit ServerManager(ServerOptions server_options) : server_options(std::move(server_options)) {};

private:
    ServerOptions server_options;
};

#endif //ROBOTS_SERVER_MANAGER_HPP
