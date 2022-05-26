#include "misc.hpp"

#include <utility>

#include "../serializers/std_types_serializers.hpp"

Position::Position(std::uint16_t x, std::uint16_t y) : x(x), y(y) {}

Position::Position(const ByteList& message, size_t beg) {
    x = deserialize_uint16(message, beg);
    y = deserialize_uint16(message, beg + UINT16_SIZE);
}

ByteList Position::serialize() const {
    ByteList result = serialize_uint16(x);
    ByteList serialized_y = serialize_uint16(y);
    result.insert(result.end(), serialized_y.begin(), serialized_y.end());
    return result;
}

Bomb::Bomb(Position position, std::uint16_t timer) : position(position), timer(timer) {}

ByteList Bomb::serialize() const {
    ByteList result = position.serialize();
    ByteList serialized_timer = serialize_uint16(timer);
    result.insert(result.end(), serialized_timer.begin(), serialized_timer.end());
    return result;
}

Player::Player(std::string name, std::string address) : name(std::move(name)), address(std::move(address)) {}

ByteList Player::serialize() const {
    ByteList result = serialize_string(name);
    ByteList serialized_address = serialize_string(address);
    result.insert(result.end(), serialized_address.begin(), serialized_address.end());
    return result;
}

void append_to_vector(ByteList& dest, const ByteList& src) {
    dest.insert(dest.end(), src.begin(), src.end());
}
