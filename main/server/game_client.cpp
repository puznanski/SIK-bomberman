#include "game_client.hpp"

#include <utility>
#include <iostream>

#include "../common/networking/tcp_bytestream.hpp"
#include "../common/models/client_message.hpp"

GameClient::GameClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       boost::asio::ip::tcp::endpoint endpoint, ServerOptions &server_options,
                       std::mutex &manager_mutex, ServerLobby &server_lobby) :
        socket(socket), endpoint(std::move(endpoint)), server_options(server_options),
        manager_mutex(manager_mutex), server_lobby(server_lobby), sender(socket) {}

GameClient::~GameClient() {
    if (client_thread) {
        client_thread->join();
    }
}

void GameClient::send_hello_message() {
    ServerMessageHello hello_message(server_options.get_server_name(), server_options.get_players_count(),
                                     server_options.get_size_x(), server_options.get_size_y(),
                                     server_options.get_game_length(), server_options.get_explosion_radius(),
                                     server_options.get_bomb_timer());

    sender.enqueue(std::make_shared<ServerMessageHello>(hello_message));
}

void GameClient::listen_client() {
    TcpBytestream tcp_bytestream(socket);

    while (true) {
        try {
            auto type = static_cast<ClientMessageType>(tcp_bytestream.get_byte());

            switch (type) {
                case Join: {
                    if (player_id) break;

                    ClientMessageJoin message(tcp_bytestream);
                    std::ostringstream address;
                    address << endpoint;
                    Player player(message.name, address.str());
                    player_id = server_lobby.add_player(player);
                    break;
                }
                case PlaceBomb:
                case PlaceBlock: {
                    if (!player_id) break;
                    set_player_movement(PlayerMovement(type));
                    break;
                }
                case Move: {
                    if (!player_id) break;
                    ClientMessageMove message(tcp_bytestream);
                    set_player_movement({type, message.direction});
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            set_error();
        }
    }
}

void GameClient::set_player_movement(PlayerMovement movement) {
    std::lock_guard<std::mutex> lock(manager_mutex);
    last_movement = movement;
}

void GameClient::set_error() {
    std::lock_guard<std::mutex> lock(error_mutex);
    error = true;
}

void GameClient::start_communication() {
    std::thread sender_thread(&ServerMessageSender::send_messages, &sender);
    try {
        listen_client();
    }
    catch (const std::exception& e) {
        sender.set_error();
        sender_thread.join();
        boost::system::error_code ec;
        socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        set_error();
    }
}

bool GameClient::get_error() const {
    std::lock_guard<std::mutex> lock(error_mutex);
    return error;
}


