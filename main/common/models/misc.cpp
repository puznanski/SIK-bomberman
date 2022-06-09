#include "misc.hpp"

#include <utility>

#include "../serializers/std_types_serializers.hpp"

Position::Position() : x(0), y(0) {}

Position::Position(std::uint16_t x, std::uint16_t y) : x(x), y(y) {}

Position::Position(TcpBytestream& bytestream) {
    x = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    y = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
}

Position::Position(UdpBytestream& bytestream) {
    x = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    y = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
}

ByteList Position::serialize() const {
    ByteList result = serialize_uint16(x);
    ByteList serialized_y = serialize_uint16(y);
    result.insert(result.end(), serialized_y.begin(), serialized_y.end());
    return result;
}

bool Position::operator<(const Position &rhs) const {
    if (x < rhs.x)
        return true;
    if (rhs.x < x)
        return false;
    return y < rhs.y;
}

bool Position::operator==(const Position &rhs) const {
    return x == rhs.x &&
           y == rhs.y;
}

bool Position::operator!=(const Position &rhs) const {
    return !(rhs == *this);
}

Bomb::Bomb(Position position, std::uint16_t timer) : position(position), timer(timer) {}

Bomb::Bomb(TcpBytestream &bytestream) {
    position = Position(bytestream);
    timer = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
}

Bomb::Bomb(UdpBytestream &bytestream) {
    position = Position(bytestream);
    timer = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
}

ByteList Bomb::serialize() const {
    ByteList result = position.serialize();
    ByteList serialized_timer = serialize_uint16(timer);
    result.insert(result.end(), serialized_timer.begin(), serialized_timer.end());
    return result;
}

Player::Player() : name(), address() {}

Player::Player(std::string name, std::string address) : name(std::move(name)), address(std::move(address)) {}

Player::Player(TcpBytestream &bytestream) {
    name = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
    address = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
}

Player::Player(UdpBytestream &bytestream) {
    name = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
    address = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
}

ByteList Player::serialize() const {
    ByteList result = serialize_string(name);
    ByteList serialized_address = serialize_string(address);
    result.insert(result.end(), serialized_address.begin(), serialized_address.end());
    return result;
}

void append_to_vector(ByteList& dest, const ByteList& src) {
    dest.insert(dest.end(), src.begin(), src.end());
}
