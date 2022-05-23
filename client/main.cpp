#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

namespace po = boost::program_options;

const uint16_t NUMBER_OF_REQUIRED_OPTIONS = 4;

std::vector<uint8_t> serialize_string(const std::string& str);

/* ClientMessage */
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

    std::vector<uint8_t> serialize() {
        std::vector<uint8_t> result;

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

    /* DEBUG */
    friend std::ostream &operator<<(std::ostream &os, const ClientMessage &client_message);
};

/* ServerMessage */
enum ServerMessageType {
    Hello = 0,
    AcceptedPlayer = 1,
    GameStarted = 2,
    Turn = 3,
    GameEnded = 4
};

using BombId = uint32_t;
using PlayerId = uint8_t;
using Score = uint32_t;

struct Position {
    uint16_t x;
    uint16_t y;
};

struct Bomb {
    Position position;
    uint16_t timer;
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
    uint8_t players_count;
    uint16_t size_x;
    uint16_t size_y;
    uint16_t game_length;
    uint16_t explosion_radius;
    uint16_t bomb_tier;
};

struct AcceptedPlayerMsg {
    PlayerId id;
    Player player;
};

struct GameStartedMsg {
    std::unordered_map<PlayerId, Player> players;
};

struct TurnMsg {
    uint16_t turn;
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

struct ClientParams {
    ClientParams(std::string gui_address, std::string player_name, uint16_t port, std::string server_address) :
        gui_address(std::move(gui_address)),
        player_name(std::move(player_name)),
        port(port),
        server_address(std::move(server_address)) {}

    std::string gui_address;
    std::string player_name;
    uint16_t port;
    std::string server_address;
};

std::vector<uint8_t> serialize_string(const std::string& str) {
    std::vector<uint8_t> result;
    result.reserve(str.size() + 1);
    result.emplace_back(str.size());
    for (uint8_t c : str) {
        result.emplace_back(c);
    }

    return result;
}


/* DEBUG */
#include <algorithm>

void print_debug_msg_beg(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    std::cout << "DEBUG - " << name << "\n";
}

void print_debug_msg_end() {
    std::cout << "================================================================================\n";
};

void print_client_params(const ClientParams& params) {
    print_debug_msg_beg("params");
    std::cout << "GUI address: " << params.gui_address << "\n";
    std::cout << "Player name: " << params.player_name << "\n";
    std::cout << "Port: " << params.port << "\n";
    std::cout << "Server address: " << params.server_address << "\n";
    print_debug_msg_end();
}

void print_serialized(const std::vector<uint8_t>& x) {
    print_debug_msg_beg("serialization");
    std::cout << "[";
    size_t len = x.size();
    size_t i = 0;
    for (auto a : x) {
        std::cout << a - 0;
        if (i != len - 1) std::cout << ", ";
        i++;
    }
    std::cout << "]\n";
    print_debug_msg_end();
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

int main(int argc, char** argv) {
    //TODO co zrobic jak zle parametry??
    po::options_description description("Usage");
    description.add_options()
            ("gui-address,d", po::value<std::string>(),
                "<(host name):(port) lub (IPv4):(port) or (IPv6):(port)>")
            ("help,h", "Displays help message")
            ("player-name,n", po::value<std::string>(), "<String>")
            ("port,p", po::value<uint16_t>(), "<u16>")
            ("server-address,s", po::value<std::string>(),
                    "<(host name):(port) lub (IPv4):(port) or (IPv6):(port)>");

    po::variables_map vm;

    try {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);
    }
    catch (const po::invalid_option_value &e) {
        std::cout << description;
        return 0;
    }

    if (vm.count("help")) {
        std::cout << description;
        if (vm.size() < NUMBER_OF_REQUIRED_OPTIONS + 1) return 0;
    }

    if (vm.size() < NUMBER_OF_REQUIRED_OPTIONS) {
        std::cout << description;
        return 0;
    }

    ClientParams client_params(vm["gui-address"].as<std::string>(), vm["player-name"].as<std::string>(),
                               vm["port"].as<uint16_t>(), vm["server-address"].as<std::string>());
    print_client_params(client_params);

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
