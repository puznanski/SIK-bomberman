#ifndef ROBOTS_CLIENT_GUI_MESSAGE_HPP
#define ROBOTS_CLIENT_GUI_MESSAGE_HPP

#include <string>
#include <cstdint>
#include <map>
#include <vector>

#include "../types.hpp"
#include "misc.hpp"
#include "../networking/udp_bytestream.hpp"

class GuiMessage {
public:
    virtual ~GuiMessage() = default;
    explicit GuiMessage(GuiMessageType gui_message_type) : gui_message_type(gui_message_type) {}

    GuiMessageType gui_message_type;

    virtual ByteList serialize() = 0;
};

class GuiMessageLobby : public GuiMessage {
public:
    GuiMessageLobby(std::string server_name, std::uint8_t players_count,
                    std::uint16_t size_x, std::uint16_t size_y, std::uint16_t game_length,
                    std::uint16_t explosion_radius, std::uint16_t bomb_tier,
                    std::map<PlayerId, Player> players);
    explicit GuiMessageLobby(UdpBytestream &bytestream);

    std::string server_name;
    std::uint8_t players_count;
    std::uint16_t size_x;
    std::uint16_t size_y;
    std::uint16_t game_length;
    std::uint16_t explosion_radius;
    std::uint16_t bomb_tier;
    std::map<PlayerId, Player> players;

    ByteList serialize() override;
};

class GuiMessageGame : public GuiMessage {
public:
    GuiMessageGame(std::string server_name, std::uint16_t size_x, std::uint16_t size_y,
                   std::uint16_t game_length, std::uint16_t turn, std::map<PlayerId, Player> players,
                   std::map<PlayerId, Position> player_positions, std::vector<Position> blocks,
                   std::vector<Bomb> bombs, std::vector<Position> explosions,
                   std::map<PlayerId, Score> scores);
    explicit GuiMessageGame(UdpBytestream &bytestream);

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

#endif //ROBOTS_CLIENT_GUI_MESSAGE_HPP
