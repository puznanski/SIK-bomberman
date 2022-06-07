#include "tcp_bytestream.hpp"

#include <utility>

TcpBytestream::TcpBytestream(std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket) : tcp_socket(std::move(tcp_socket)) {}

ByteList TcpBytestream::get_bytes(size_t number_of_bytes) {
    ByteList result(number_of_bytes);
    boost::asio::read(*tcp_socket, boost::asio::buffer(result));

    return result;
}

std::uint8_t TcpBytestream::get_byte() {
    return get_bytes(1).at(0);
}

