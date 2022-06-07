#include "udp_bytestream.hpp"

#include <utility>

UdpBytestream::UdpBytestream(std::shared_ptr<boost::asio::ip::udp::socket> udp_socket) : udp_socket(std::move(udp_socket)), buffer(MAX_UDP_DATAGRAM_SIZE) {}

void UdpBytestream::get_message() {
    boost::asio::ip::udp::endpoint endpoint;
    bytes_received = udp_socket->receive_from(boost::asio::buffer(buffer), endpoint);
    buffer_iterator = 0;
}

ByteList UdpBytestream::get_bytes(size_t number_of_bytes) {
    ByteList result(number_of_bytes);

    if (number_of_bytes > bytes_received - buffer_iterator) throw UdpWrongMessageException();

    for (size_t i = 0; i < number_of_bytes; i++) {
        result[i] = buffer[i + buffer_iterator];
    }

    buffer_iterator += number_of_bytes;

    return result;
}

std::uint8_t UdpBytestream::get_byte() {
    return get_bytes(1).at(0);
}

bool UdpBytestream::is_buffer_empty() const {
    return bytes_received <= buffer_iterator;
}
