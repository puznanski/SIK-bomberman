#include "client_manager.hpp"

#include <utility>
#include <thread>

ClientManager::ClientManager(ClientOptions client_options) : client_options(std::move(client_options)) {
    boost::asio::ip::tcp::resolver tcp_resolver(io_context);
    auto tcp_endpoint = tcp_resolver.resolve(client_options.get_server_address_host_name(), client_options.get_server_address_port());
    tcp_socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
    boost::asio::connect(*tcp_socket, tcp_endpoint);

    boost::asio::ip::udp::resolver udp_resolver(io_context);
    udp_endpoint = *(udp_resolver.resolve(client_options.get_gui_address_host_name(), client_options.get_gui_address_port()).begin());
    udp_socket = std::make_shared<boost::asio::ip::udp::socket>(io_context);
}

void ClientManager::listen_all() {
    std::thread gui(&ClientManager::listen_gui, this);
    listen_server();
}

void ClientManager::listen_server() {
    TcpBytestream tcp_bytestream(tcp_socket);
}

void ClientManager::listen_gui() {
    UdpBytestream udp_bytestream(udp_socket);
}
