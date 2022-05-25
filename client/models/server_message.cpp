#include "server_message.hpp"

#include <utility>
#include <sstream>

#include "../serializers/std_types_serializers.hpp"

ServerMessageHello::ServerMessageHello(std::string server_name, std::uint8_t players_count,
                                       std::uint16_t size_x, std::uint16_t size_y,
                                       std::uint16_t game_length, std::uint16_t explosion_radius,
                                       std::uint16_t bomb_tier) : ServerMessage(ServerMessageType::Hello),
                                                                  server_name(std::move(server_name)),
                                                                  players_count(players_count),
                                                                  size_x(size_x), size_y(size_y),
                                                                  game_length(game_length),
                                                                  explosion_radius(explosion_radius),
                                                                  bomb_tier(bomb_tier) {}

ByteList ServerMessageHello::serialize() {
    ByteList result;
    result.push_back(server_message_type);

    ByteList serialized_server_name = serialize_string(server_name);
    append_to_vector(result, serialized_server_name);
    result.push_back(players_count);

    ByteList serialized_size_x = serialize_uint16(size_x);
    append_to_vector(result, serialized_size_x);

    ByteList serialized_size_y = serialize_uint16(size_y);
    append_to_vector(result, serialized_size_y);

    ByteList serialized_game_length = serialize_uint16(game_length);
    append_to_vector(result, serialized_game_length);

    ByteList serialized_explosion_radius = serialize_uint16(explosion_radius);
    append_to_vector(result, serialized_explosion_radius);

    ByteList serialized_bomb_tier = serialize_uint16(bomb_tier);
    append_to_vector(result, serialized_bomb_tier);

    return result;
}

std::string ServerMessageHello::to_string() {
    std::stringstream ss;
    ss << "ServerMessage (Hello): { server_name: " << server_name << ", " <<
                                "players_count: " << players_count + 0 << ", " <<
                                "size_x: " << size_x << ", " <<
                                "size_y: " << size_y << ", " <<
                                "game_length: " << game_length << ", " <<
                                "explosion_radius: " << explosion_radius << ", " <<
                                "bomb_tier: " << bomb_tier << " }\n";

    return ss.str();
}

ServerMessageAcceptedPlayer::ServerMessageAcceptedPlayer(PlayerId id, Player player) : ServerMessage(ServerMessageType::AcceptedPlayer), id(id),
                                                                                       player(std::move(player)) {}

ByteList ServerMessageAcceptedPlayer::serialize() {
    ByteList result;
    result.push_back(server_message_type);
    result.push_back(id);

    ByteList serialized_player = player.serialize();
    append_to_vector(result, serialized_player);

    return result;
}

std::string ServerMessageAcceptedPlayer::to_string() {
    std::stringstream ss;
    ss << "ServerMessage (AcceptedPlayer): { player_id: " << id + 0 << ", player: { name: " << player.name << ", address: " << player.address << " } }\n";

    return ss.str();
}

ServerMessageGameStarted::ServerMessageGameStarted(std::map<PlayerId, Player> players) :
    ServerMessage(ServerMessageType::GameStarted), players(std::move(players)) {}

ByteList ServerMessageGameStarted::serialize() {
    ByteList result;
    result.push_back(server_message_type);

    ByteList serialized_map_length = serialize_uint32(players.size());
    append_to_vector(result, serialized_map_length);

    for (const auto &player : players) {
        result.push_back(player.first);
        append_to_vector(result, player.second.serialize());
    }

    return result;
}

std::string ServerMessageGameStarted::to_string() {
    std::stringstream ss;
    ss << "ServerMessage (GameStarted): { players_map: [";

    for (const auto &player : players) {
        ss << " {" << player.first + 0 << " : {" << player.second.name << ", " << player.second.address << "}} ";
    }

    ss << "] }\n";

    return ss.str();
}

ServerMessageTurn::ServerMessageTurn(std::uint16_t turn, std::vector<std::shared_ptr<Event>> events) : ServerMessage(ServerMessageType::Turn),
                                                                                        turn(turn), events(std::move(events)) {}

ByteList ServerMessageTurn::serialize() {
    ByteList result;
    result.push_back(server_message_type);
    append_to_vector(result, serialize_uint16(turn));
    append_to_vector(result, serialize_uint32(events.size()));

    for (const auto& event : events) {
        append_to_vector(result, event->serialize());
    }

    return result;
}

std::string ServerMessageTurn::to_string() {
    std::stringstream ss;
    ss << "ServerMessage (Turn): { turn: " << turn << ", events: [\n";

    for (const auto& event : events) {
        ss << "\t\t\t" << event->to_string();
    }

    ss << "] }\n";

    return ss.str();
}

ServerMessageGameEnded::ServerMessageGameEnded(std::map<PlayerId, Score> scores) : ServerMessage(ServerMessageType::GameEnded),
                                                                                    scores(std::move(scores)) {}

ByteList ServerMessageGameEnded::serialize() {
    ByteList result;
    result.push_back(server_message_type);
    append_to_vector(result, serialize_uint32(scores.size()));

    for (const auto& score : scores) {
        result.push_back(score.first);
        append_to_vector(result, serialize_uint32(score.second));
    }

    return result;
}

std::string ServerMessageGameEnded::to_string() {
    std::stringstream ss;
    ss << "ServerMessage (GameEnded): { scores_map: [";

    for (const auto& score : scores) {
        ss << " {" << score.first + 0 << " : " << score.second << "} ";
    }

    ss << "] }\n";

    return ss.str();
}
