#ifndef ROBOTS_GAME_CLIENT_HPP
#define ROBOTS_GAME_CLIENT_HPP

#include <optional>
#include <memory>
#include <mutex>
#include <thread>
#include <boost/asio.hpp>
#include <exception>

#include "../common/types.hpp"
#include "server_game_state.hpp"
#include "server_lobby.hpp"
#include "server_message_sender.hpp"
#include "../common/models/player_movement.hpp"
#include "../common/models/client_message.hpp"

class GameClient {
public:
    GameClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
               boost::asio::ip::tcp::endpoint endpoint, ServerOptions &server_options,
               std::mutex &manager_mutex, ServerLobby &server_lobby);

    ~GameClient();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    boost::asio::ip::tcp::endpoint endpoint;
    ServerOptions &server_options;
    std::optional<PlayerId> player_id;
    std::optional<PlayerMovement> last_movement;
    std::mutex &manager_mutex;
    ServerLobby &server_lobby;
    ServerMessageSender sender;
    bool error = false;
    mutable std::mutex error_mutex;
    std::optional<std::thread> client_thread;

    void listen_client();
    void set_player_movement(PlayerMovement movement);
    void set_error();

public:
    void send_hello_message();
    void start_communication();
    bool get_error() const;

    std::optional<PlayerMovement> get_movement() const {
        return last_movement;
    }

    void clear_movement() {
        last_movement = std::nullopt;
    }

    void enqueue_message(std::shared_ptr<ServerMessage> server_message) {
        sender.enqueue(server_message);
    }

    void set_thread(std::thread&& thread) {
        client_thread = std::move(thread);
    }

    std::optional<PlayerId> get_player_id() const {
        return player_id;
    }

    void end_game() {
        player_id = std::nullopt;
    }
};

#endif //ROBOTS_GAME_CLIENT_HPP
