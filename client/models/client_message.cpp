#include "client_message.hpp"

#include <iostream>
#include <sstream>

#include "../serializers/std_types_serializers.hpp"

ByteList ClientMessage::serialize() {
    ByteList result;
    result.reserve(1);
    result.emplace_back(client_message_type);
    return result;
}

ClientMessageJoin::ClientMessageJoin(TcpBytestream& bytestream) : ClientMessage(ClientMessageType::Join) {
    name = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
}

ByteList ClientMessageJoin::serialize() {
    ByteList result;
    auto serialized_str = serialize_string(name);
    result.reserve(1 + serialized_str.size());
    result.emplace_back(client_message_type);

    for (const auto &a : serialized_str) {
        result.emplace_back(a);
    }

    return result;
}

ClientMessageMove::ClientMessageMove(TcpBytestream& bytestream) : ClientMessage(ClientMessageType::Move) {
    direction = static_cast<Direction>(bytestream.get_byte());
}

ByteList ClientMessageMove::serialize() {
    ByteList result;
    result.reserve(2);
    result.emplace_back(client_message_type);
    result.emplace_back(direction);
    return result;
}

/* DEBUG */
std::string ClientMessage::to_string() {
    std::stringstream ss;
    ss << "ClientMessage: " << client_message_type << "\n";
    return ss.str();
}

std::string ClientMessageJoin::to_string() {
    std::stringstream ss;
    ss << "ClientMessage: " << client_message_type << " { name: " << name << " }\n";
    return ss.str();
}

std::string ClientMessageMove::to_string() {
    std::stringstream ss;
    ss << "ClientMessage: " << client_message_type << " { direction: " << direction << " }\n";
    return ss.str();
}
