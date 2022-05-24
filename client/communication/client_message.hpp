#ifndef ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
#define ROBOTS_CLIENT_CLIENT_MESSAGE_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "../types.hpp"

enum ClientMessageType {
    Join = 0,
    PlaceBomb = 1,
    PlaceBlock = 2,
    Move = 3
};

enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

struct JoinMsg {
    std::string name;
};

struct MoveMsg {
    Direction direction;
};

class ClientMessage {
public:
    explicit ClientMessage(ClientMessageType type) : client_message_type(type) {}
    ClientMessage(ClientMessageType type, JoinMsg join_msg) : client_message_type(type), join(std::move(join_msg)) {}
    ClientMessage(ClientMessageType type, MoveMsg move_msg) : client_message_type(type), move(move_msg) {}
    ~ClientMessage() {}

    ClientMessageType client_message_type;
    union {
        JoinMsg join;
        MoveMsg move;
    };

    ByteList serialize();
    friend std::ostream &operator<<(std::ostream &os, const ClientMessage &client_message);
};

#endif //ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
