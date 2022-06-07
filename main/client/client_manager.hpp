#ifndef ROBOTS_CLIENT_CLIENT_MANAGER_HPP
#define ROBOTS_CLIENT_CLIENT_MANAGER_HPP

#include <boost/asio.hpp>
#include <memory>

#include "client_options.hpp"
#include "client_game_state.hpp"
#include "../common/networking/tcp_bytestream.hpp"
#include "../common/networking/udp_bytestream.hpp"
#include "../common/models/client_message.hpp"
#include "../common/models/draw_message.hpp"

class ClientManager {
public:
    explicit ClientManager(ClientOptions client_options);

private:
    ClientOptions client_options;
    ClientGameState client_game_state;
    boost::asio::io_context io_context{};
    std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket;
    std::shared_ptr<boost::asio::ip::udp::socket> udp_socket;
    boost::asio::ip::udp::endpoint udp_endpoint;

    void listen_server();
    void listen_gui();
    static std::shared_ptr<ClientMessage> generate_client_message(UdpBytestream& bytestream);
    std::shared_ptr<DrawMessage> generate_draw_message(TcpBytestream& bytestream);

public:
    void listen_all();
};

#endif //ROBOTS_CLIENT_CLIENT_MANAGER_HPP
