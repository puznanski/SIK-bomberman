#ifndef ROBOTS_SERVER_MANAGER_HPP
#define ROBOTS_SERVER_MANAGER_HPP

#include <boost/asio.hpp>
#include <utility>
#include <thread>
#include <list>
#include <mutex>
#include <memory>
#include <map>

#include "server_options.hpp"
#include "server_game_state.hpp"
#include "game_client.hpp"
#include "server_lobby.hpp"
#include "../common/models/server_message.hpp"

class ServerManager {
public:
    explicit ServerManager(ServerOptions server_options);

private:
    ServerOptions server_options;
    ServerGameState server_game_state;
    ServerLobby lobby;
    boost::asio::io_context io_context{};
    boost::asio::ip::tcp::acceptor acceptor;
    mutable std::mutex manager_mutex;
    std::list<GameClient> game_clients;
    std::map<PlayerId, Player> players;
    bool game_started = false;

    void handle();
    void start_game();
    void end_game();
    void listen_for_clients();
    void send_to_clients(std::shared_ptr<ServerMessage> msg);
    void clear_events();

public:
    void start();
};

#endif //ROBOTS_SERVER_MANAGER_HPP
