#ifndef ROBOTS_CLIENT_SERVER_MESSAGE_HPP
#define ROBOTS_CLIENT_SERVER_MESSAGE_HPP

#include <string>
#include <cstdint>
#include <map>
#include <vector>

#include "misc.hpp"
#include "event.hpp"
#include "../types.hpp"
#include "../networking/tcp_bytestream.hpp"

class ServerMessage {
public:
    virtual ~ServerMessage() = default;
    explicit ServerMessage(ServerMessageType server_message_type) : server_message_type(server_message_type) {}

    ServerMessageType server_message_type;

    virtual ByteList serialize() = 0;
    virtual std::string to_string() = 0;
};

class ServerMessageHello : public ServerMessage {
public:
    ServerMessageHello(std::string server_name, std::uint8_t players_count,
                       std::uint16_t size_x, std::uint16_t size_y, std::uint16_t game_length,
                       std::uint16_t explosion_radius, std::uint16_t bomb_tier);
    explicit ServerMessageHello(TcpBytestream &bytestream);

    std::string server_name;
    std::uint8_t players_count;
    std::uint16_t size_x;
    std::uint16_t size_y;
    std::uint16_t game_length;
    std::uint16_t explosion_radius;
    std::uint16_t bomb_tier;

    ByteList serialize() override;
    std::string to_string() override;
};

class ServerMessageAcceptedPlayer : public ServerMessage {
public:
    ServerMessageAcceptedPlayer(PlayerId id, Player player);
    explicit ServerMessageAcceptedPlayer(TcpBytestream &bytestream);

    PlayerId id;
    Player player;

    ByteList serialize() override;
    std::string to_string() override;
};

class ServerMessageGameStarted : public ServerMessage {
public:
    explicit ServerMessageGameStarted(std::map<PlayerId, Player> players);
    explicit ServerMessageGameStarted(TcpBytestream &bytestream);

    std::map<PlayerId, Player> players;

    ByteList serialize() override;
    std::string to_string() override;
};

class ServerMessageTurn : public ServerMessage {
public:
    ServerMessageTurn(std::uint16_t turn, std::vector<std::shared_ptr<Event>> events);
    explicit ServerMessageTurn(TcpBytestream &bytestream);

    std::uint16_t turn;
    std::vector<std::shared_ptr<Event>> events;

    ByteList serialize() override;
    std::string to_string() override;
};

class ServerMessageGameEnded : public ServerMessage {
public:
    explicit ServerMessageGameEnded(std::map<PlayerId, Score> scores);
    explicit ServerMessageGameEnded(TcpBytestream &bytestream);

    std::map<PlayerId, Score> scores;

    ByteList serialize() override;
    std::string to_string() override;
};

#endif //ROBOTS_CLIENT_SERVER_MESSAGE_HPP
