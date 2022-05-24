#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "debug_functions.hpp"
#include "communication/client_message.hpp"
#include "serializers/std_types_serializers.hpp"
#include "management/client_options.hpp"

namespace po = boost::program_options;

/* ServerMessage */
enum ServerMessageType {
    Hello = 0,
    AcceptedPlayer = 1,
    GameStarted = 2,
    Turn = 3,
    GameEnded = 4
};

using BombId = std::uint32_t;
using PlayerId = std::uint8_t;
using Score = std::uint32_t;

struct Position {
    std::uint16_t x;
    std::uint16_t y;
};

struct Bomb {
    Position position;
    std::uint16_t timer;
};

struct Player {
    std::string name;
    std::string address;
};

enum EventType {
    BombPlaced = 0,
    BombExploded = 1,
    PlayerMoved = 2,
    BlockPlaced = 3
};

struct BombPlacedEvent {
    BombId id;
    Position position;
};

struct BombExplodedEvent {
    BombId id;
    std::vector<PlayerId> robots_destroyed;
};

struct PlayerMovedEvent {
    PlayerId id;
    Position position;
};

struct BlockPlacedEvent {
    Position position;
};

struct Event {
    EventType event_type;
    union event_args {
        explicit event_args(const BombPlacedEvent &event) : bomb_placed(event) {}
        explicit event_args(BombExplodedEvent event) : bomb_exploded(std::move(event)) {}
        explicit event_args(const PlayerMovedEvent &event) : player_moved(event) {}
        explicit event_args(const BlockPlacedEvent &event) : block_placed(event) {}

        BombPlacedEvent bomb_placed;
        BombExplodedEvent bomb_exploded;
        PlayerMovedEvent player_moved;
        BlockPlacedEvent block_placed;
    };
};

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

int main(int argc, char** argv) {
    ClientOptions client_options;

    try {
        client_options.parse_options(argc, argv);
    }
    catch (const ClientOptionsException &e) {
        std::cerr << e.what();
        std::cout << client_options.get_help_message();
        return 0;
    }
    catch (const ClientOptionsGetHelp &e) {
        std::cout << client_options.get_help_message();
        return 0;
    }

    print_client_params(client_options);

    //connection
    boost::asio::io_context io_context;

    std::string str("Żółć!");

    auto test = serialize_string(str);
    print_serialized(test);


    ClientMessage test2(ClientMessageType::Join, {"😍💕"});
    std::cout << test2 << "\n";
    print_serialized(test2.serialize());

    ClientMessage test3(ClientMessageType::Move, {Direction::Right});
    std::cout << test3 << "\n";
    print_serialized(test3.serialize());


    return 0;
}
