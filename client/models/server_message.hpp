#ifndef ROBOTS_CLIENT_SERVER_MESSAGE_HPP
#define ROBOTS_CLIENT_SERVER_MESSAGE_HPP

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "misc.hpp"
#include "event.hpp"
#include "../types.hpp"

struct HelloMsg {
    std::string server_name;
    std::uint8_t players_count;
    std::uint16_t size_x;
    std::uint16_t size_y;
    std::uint16_t game_length;
    std::uint16_t explosion_radius;
    std::uint16_t bomb_tier;
};

struct AcceptedPlayerMsg {
    PlayerId id;
    Player player;
};

struct GameStartedMsg {
    std::unordered_map<PlayerId, Player> players;
};

struct TurnMsg {
    std::uint16_t turn;
    std::vector<Event> events;
};

struct GameEndedMsg {
    std::unordered_map<PlayerId, Score> scores;
};

struct ServerMessage {
    ServerMessageType server_message_type;
    union server_message_args {
        explicit server_message_args(HelloMsg msg) : hello(std::move(msg)) {}
        explicit server_message_args(AcceptedPlayerMsg msg) : accepted_player(std::move(msg)) {}
        explicit server_message_args(GameStartedMsg msg) : game_started(std::move(msg)) {}
        explicit server_message_args(TurnMsg msg) : turn(std::move(msg)) {}
        explicit server_message_args(GameEndedMsg msg) : game_ended(std::move(msg)) {}

        HelloMsg hello;
        AcceptedPlayerMsg accepted_player;
        GameStartedMsg game_started;
        TurnMsg turn;
        GameEndedMsg game_ended;
    };
};

#endif //ROBOTS_CLIENT_SERVER_MESSAGE_HPP
