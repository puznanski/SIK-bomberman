#ifndef ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
#define ROBOTS_CLIENT_CLIENT_MESSAGE_HPP

#include <string>
#include <utility>
#include <vector>
#include <cstdint>

#include "../types.hpp"
#include "../networking/tcp_bytestream.hpp"

class ClientMessage {
public:
    virtual ~ClientMessage() = default;
    explicit ClientMessage(ClientMessageType client_message_type) : client_message_type(client_message_type) {}

    ClientMessageType client_message_type{};

    virtual ByteList serialize();
};

class ClientMessageJoin : public ClientMessage {
public:
    explicit ClientMessageJoin(std::string name) : ClientMessage(ClientMessageType::Join), name(std::move(name)) {}
    explicit ClientMessageJoin(TcpBytestream& bytestream);

    std::string name;

    ByteList serialize() override;
};

class ClientMessageMove : public ClientMessage {
public:
    explicit ClientMessageMove(Direction direction) : ClientMessage(ClientMessageType::Move), direction(direction) {}
    explicit ClientMessageMove(TcpBytestream& bytestream);

    Direction direction;

    ByteList serialize() override;
};

#endif //ROBOTS_CLIENT_CLIENT_MESSAGE_HPP
