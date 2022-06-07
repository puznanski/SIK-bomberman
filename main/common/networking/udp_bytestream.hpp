#ifndef ROBOTS_CLIENT_UDP_BYTESTREAM_HPP
#define ROBOTS_CLIENT_UDP_BYTESTREAM_HPP

#include <boost/asio.hpp>
#include <cstdint>

#include "../types.hpp"

const size_t MAX_UDP_DATAGRAM_SIZE = 65507;

class UdpBytestream {
public:
    explicit UdpBytestream(std::shared_ptr<boost::asio::ip::udp::socket> udp_socket);

private:
    std::shared_ptr<boost::asio::ip::udp::socket> udp_socket;
    ByteList buffer;
    size_t bytes_received{};
    size_t buffer_iterator{};

public:
    void get_message();
    ByteList get_bytes(size_t number_of_bytes);
    std::uint8_t get_byte();
    bool is_buffer_empty() const;
};

#endif //ROBOTS_CLIENT_UDP_BYTESTREAM_HPP
