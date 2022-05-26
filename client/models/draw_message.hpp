#ifndef ROBOTS_CLIENT_DRAW_MESSAGE_HPP
#define ROBOTS_CLIENT_DRAW_MESSAGE_HPP

#include <string>
#include <cstdint>
#include <map>
#include <vector>

#include "../types.hpp"
#include "misc.hpp"
#include "../networking/udp_bytestream.hpp"

class DrawMessage {
public:
    virtual ~DrawMessage() = default;
    explicit DrawMessage(DrawMessageType gui_message_type) : gui_message_type(gui_message_type) {}

    DrawMessageType gui_message_type;

    virtual ByteList serialize() = 0;
};

class DrawMessageLobby : public DrawMessage {
public:
    DrawMessageLobby(std::string server_name, std::uint8_t players_count,
                     std::uint16_t size_x, std::uint16_t size_y, std::uint16_t game_length,
                     std::uint16_t explosion_radius, std::uint16_t bomb_timer,
                     std::map<PlayerId, Player> players);
    explicit DrawMessageLobby(UdpBytestream &bytestream);

    std::string server_name;
    std::uint8_t players_count;
    std::uint16_t size_x;
    std::uint16_t size_y;
    std::uint16_t game_length;
    std::uint16_t explosion_radius;
    std::uint16_t bomb_timer;
    std::map<PlayerId, Player> players;

    ByteList serialize() override;
};

class DrawMessageGame : public DrawMessage {
public:
    DrawMessageGame(std::string server_name, std::uint16_t size_x, std::uint16_t size_y,
                    std::uint16_t game_length, std::uint16_t turn, std::map<PlayerId, Player> players,
                    std::map<PlayerId, Position> player_positions, std::vector<Position> blocks,
                    std::vector<Bomb> bombs, std::vector<Position> explosions,
                    std::map<PlayerId, Score> scores);
    explicit DrawMessageGame(UdpBytestream &bytestream);

    std::string server_name;
    std::uint16_t size_x;
    std::uint16_t size_y;
    std::uint16_t game_length;
    std::uint16_t turn;
    std::map<PlayerId, Player> players;
    std::map<PlayerId, Position> player_positions;
    std::vector<Position> blocks;
    std::vector<Bomb> bombs;
    std::vector<Position> explosions;
    std::map<PlayerId, Score> scores;

    ByteList serialize() override;
};

#endif //ROBOTS_CLIENT_DRAW_MESSAGE_HPP
