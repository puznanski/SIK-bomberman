#ifndef ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
#define ROBOTS_CLIENT_CLIENT_MESSAGE_HPP

#include <string>
#include <utility>
#include <vector>
#include <cstdint>

#include "../types.hpp"

class ClientMessage {
public:
    ClientMessage() = default;
    explicit ClientMessage(ClientMessageType client_message_type) : client_message_type(client_message_type) {}
    explicit ClientMessage(const ByteList& message);

    ClientMessageType client_message_type{};

    virtual ByteList serialize();

    /* DEBUG */
    virtual std::string to_string();
};

class ClientMessageJoin : public ClientMessage {
public:
    explicit ClientMessageJoin(std::string name) : ClientMessage(ClientMessageType::Join), name(std::move(name)) {}
    explicit ClientMessageJoin(const ByteList& message);

    std::string name;

    ByteList serialize() override;

    /* DEBUG */
    std::string to_string() override;
};

class ClientMessageMove : public ClientMessage {
public:
    explicit ClientMessageMove(Direction direction) : ClientMessage(ClientMessageType::Move), direction(direction) {}
    explicit ClientMessageMove(const ByteList& message);

    Direction direction;

    ByteList serialize() override;

    /* DEBUG */
    std::string to_string() override;
};

#endif //ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
