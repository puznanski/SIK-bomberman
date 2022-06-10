#include "game_client.hpp"

#include <utility>
#include <iostream>

#include "../common/networking/tcp_bytestream.hpp"
#include "../common/models/client_message.hpp"

GameClient::GameClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket,
                       boost::asio::ip::tcp::endpoint endpoint, ServerOptions &server_options,
                       std::mutex &manager_mutex, ServerLobby &server_lobby) :
        socket(std::move(socket)), endpoint(std::move(endpoint)), server_options(server_options),
        manager_mutex(manager_mutex), server_lobby(server_lobby), sender(socket) {}

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
                    ClientMessageJoin message(tcp_bytestream);
                    Player player(message.name, endpoint.address().to_string());
                    player_id = server_lobby.add_player(player);
                    break;
                }
                case PlaceBomb:
                case PlaceBlock: {
                    last_movement = PlayerMovement(type);
                    break;
                }
                case Move: {
                    ClientMessageMove message(tcp_bytestream);
                    last_movement = PlayerMovement(type, message.direction);
                    break;
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            exit(1);
        }
    }
}

void GameClient::start_communication() {
    send_hello_message();
    listen_client();
};
