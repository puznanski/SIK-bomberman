#include "server_manager.hpp"

ServerManager::ServerManager(ServerOptions server_options) : server_options(std::move(server_options)),
                                                             server_game_state(server_options) {}

void ServerManager::start() {

};
