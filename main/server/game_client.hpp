#ifndef ROBOTS_GAME_CLIENT_HPP
#define ROBOTS_GAME_CLIENT_HPP

#include <optional>
#include <memory>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>

#include "../common/types.hpp"
#include "server_game_state.hpp"
#include "server_lobby.hpp"
#include "server_message_sender.hpp"
#include "../common/models/player_movement.hpp"

class GameClient {
public:
    GameClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
               boost::asio::ip::tcp::endpoint endpoint, ServerOptions &server_options,
               std::mutex &manager_mutex, ServerLobby &server_lobby);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    boost::asio::ip::tcp::endpoint endpoint;
    ServerOptions &server_options;
    std::optional<PlayerId> player_id;
    std::optional<PlayerMovement> last_movement;
    std::mutex &manager_mutex;
    ServerLobby &server_lobby;
    ServerMessageSender sender;

    void send_hello_message();
    void listen_client();

public:
    void start_communication();

};

#endif //ROBOTS_GAME_CLIENT_HPP
