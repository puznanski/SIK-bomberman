#ifndef ROBOTS_CLIENT_TCP_BYTESTREAM_HPP
#define ROBOTS_CLIENT_TCP_BYTESTREAM_HPP

#include <boost/asio.hpp>
#include <cstdint>

#include "../types.hpp"

class TcpBytestream {
public:
    explicit TcpBytestream(std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket;

public:
    ByteList get_bytes(size_t number_of_bytes);
    std::uint8_t get_byte();
};

#endif //ROBOTS_CLIENT_TCP_BYTESTREAM_HPP
