#include "client_manager.hpp"

#include <utility>
#include <thread>
#include <iostream>

#include "../common/models/server_message.hpp"
#include "../common/models/gui_message.hpp"

ClientManager::ClientManager(ClientOptions client_options) : client_options(std::move(client_options)) {
    boost::asio::ip::tcp::resolver tcp_resolver(io_context);
    auto tcp_endpoint = tcp_resolver.resolve(this->client_options.get_server_address_host_name(), this->client_options.get_server_address_port());
    tcp_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    boost::asio::connect(*tcp_socket, tcp_endpoint);
    tcp_socket->set_option(boost::asio::ip::tcp::no_delay(true));


    udp_socket = std::make_shared<boost::asio::ip::udp::socket>(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v6(), client_options.get_port()));
    boost::asio::ip::udp::resolver udp_resolver(io_context);
    udp_endpoint = *(udp_resolver.resolve(this->client_options.get_gui_address_host_name(), this->client_options.get_gui_address_port()).begin());
}

void ClientManager::listen_all() {
    std::thread gui(&ClientManager::listen_gui, this);
    listen_server();
}

void ClientManager::listen_server() {
    TcpBytestream tcp_bytestream(tcp_socket);

    while (true) {
        try {
            auto draw_message = generate_draw_message(tcp_bytestream);

            if (draw_message != nullptr) {
                ByteList serialized_draw_message = draw_message->serialize();
                udp_socket->send_to(boost::asio::buffer(serialized_draw_message), udp_endpoint);
            }
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            exit(1);
        }
    }
}

void ClientManager::listen_gui() {
    UdpBytestream udp_bytestream(udp_socket);

    while (true) {
        udp_bytestream.get_message();

        try {
            auto client_message = generate_client_message(udp_bytestream);

            if (client_game_state.is_game_started()) {
                ByteList serialized_client_message = client_message->serialize();
                boost::asio::write(*tcp_socket, boost::asio::buffer(serialized_client_message));
            }
            else if (!client_game_state.is_join_message_sent()) {
                ByteList serialized_client_message = ClientMessageJoin(client_options.get_player_name()).serialize();
                client_game_state.set_join_message_sent(true);
                boost::asio::write(*tcp_socket, boost::asio::buffer(serialized_client_message));
            }
        }
        catch (const UdpWrongMessageException& e) {
            continue;
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
            exit(1);
        }
    }
}

std::shared_ptr<ClientMessage> ClientManager::generate_client_message(UdpBytestream &bytestream) {
    auto gui_message_type = static_cast<GuiMessageType>(bytestream.get_byte());
    std::shared_ptr<GuiMessage> gui_message;

    switch (gui_message_type) {
        case PlaceBombGui: {
            gui_message = std::make_shared<GuiMessagePlaceBomb>();
            break;
        }
        case PlaceBlockGui: {
            gui_message = std::make_shared<GuiMessagePlaceBlock>();
            break;
        }
        case MoveGui: {
            gui_message = std::make_shared<GuiMessageMove>(bytestream);
            break;
        }
        default: {
            throw UdpWrongMessageException();
        }
    }

    if (!bytestream.is_buffer_empty()) throw UdpWrongMessageException();

    return gui_message->get_client_message();
}

std::shared_ptr<DrawMessage> ClientManager::generate_draw_message(TcpBytestream &bytestream) {
    auto server_message_type = static_cast<ServerMessageType>(bytestream.get_byte());
    std::shared_ptr<ServerMessage> server_message;

    switch (server_message_type) {
        case Hello: {
            server_message = std::make_shared<ServerMessageHello>(bytestream);
            break;
        }
        case AcceptedPlayer: {
            server_message = std::make_shared<ServerMessageAcceptedPlayer>(bytestream);
            break;
        }
        case GameStarted: {
            server_message = std::make_shared<ServerMessageGameStarted>(bytestream);
            break;
        }
        case Turn: {
            server_message = std::make_shared<ServerMessageTurn>(bytestream);
            break;
        }
        case GameEnded: {
            server_message = std::make_shared<ServerMessageGameEnded>(bytestream);
            break;
        }
        default: {
            throw TcpWrongMessageException();
        }
    }

    return server_message->get_draw_message(client_game_state);
}
