#include "client_message.hpp"

#include <iostream>

#include "../serializers/std_types_serializers.hpp"

ByteList ClientMessage::serialize() {
    ByteList result;

    if (client_message_type == ClientMessageType::Join) {
        auto serialized_str = serialize_string(join.name);
        result.reserve(1 + serialized_str.size());
        result.emplace_back(client_message_type);

        for (const auto &a : serialized_str) {
            result.emplace_back(a);
        }
    }
    else if (client_message_type == ClientMessageType::Move) {
        result.reserve(2);
        result.emplace_back(client_message_type);
        result.emplace_back(move.direction);
    }
    else {
        result.reserve(1);
        result.emplace_back(client_message_type);
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const ClientMessage &client_message) {
    switch (client_message.client_message_type) {
        case Join:
            return os << "ClientMessage: " << client_message.client_message_type <<
                      " { name: " << client_message.join.name << " }\n";
        case Move:
            return os << "ClientMessage: " << client_message.client_message_type <<
                      " { direction: " << client_message.move.direction << " }\n";
        default:
            return os << "ClientMessage: " << client_message.client_message_type << "\n";
    }
}
