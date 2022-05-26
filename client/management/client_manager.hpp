#ifndef ROBOTS_CLIENT_CLIENT_MANAGER_HPP
#define ROBOTS_CLIENT_CLIENT_MANAGER_HPP

#include <boost/asio.hpp>

#include "client_options.hpp"
#include "../networking/tcp_bytestream.hpp"
#include "../networking/udp_bytestream.hpp"

class ClientManager {
public:
    explicit ClientManager(ClientOptions client_options);

private:
    ClientOptions client_options;
    boost::asio::io_context io_context{};
    std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket;
    std::shared_ptr<boost::asio::ip::udp::socket> udp_socket;
    boost::asio::ip::udp::endpoint udp_endpoint;

    void listen_server();
    void listen_gui();

public:
    void listen_all();
};

#endif //ROBOTS_CLIENT_CLIENT_MANAGER_HPP
