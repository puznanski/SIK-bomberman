#include "gui_message.hpp"

#include <utility>

#include "../serializers/std_types_serializers.hpp"

GuiMessageLobby::GuiMessageLobby(std::string server_name, std::uint8_t players_count,
                                 std::uint16_t size_x, std::uint16_t size_y, std::uint16_t game_length,
                                 std::uint16_t explosion_radius, std::uint16_t bomb_tier,
                                 std::map<PlayerId, Player> players) :
        GuiMessage(GuiMessageType::Lobby), server_name(std::move(server_name)), players_count(players_count),
        size_x(size_x), size_y(size_y), game_length(game_length), explosion_radius(explosion_radius), bomb_tier(bomb_tier),
        players(std::move(players)) {}

GuiMessageLobby::GuiMessageLobby(UdpBytestream &bytestream) : GuiMessage(GuiMessageType::Lobby) {
    server_name = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
    players_count = bytestream.get_byte();
    size_x = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    size_y = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    game_length = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    explosion_radius = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    bomb_tier = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));

    size_t players_map_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));

    for (size_t i = 0; i < players_map_size; i++) {
        players.insert({bytestream.get_byte(), Player(bytestream)});
    }
}

ByteList GuiMessageLobby::serialize() {
    ByteList result;
    result.push_back(gui_message_type);
    append_to_vector(result, serialize_string(server_name));
    result.push_back(players_count);
    append_to_vector(result, serialize_uint16(size_x));
    append_to_vector(result, serialize_uint16(size_y));
    append_to_vector(result, serialize_uint16(game_length));
    append_to_vector(result, serialize_uint16(explosion_radius));
    append_to_vector(result, serialize_uint16(bomb_tier));
    append_to_vector(result, serialize_uint32(players.size()));

    for (const auto& player : players) {
        result.push_back(player.first);
        append_to_vector(result, player.second.serialize());
    }

    return result;
}

GuiMessageGame::GuiMessageGame(std::string server_name, std::uint16_t size_x, std::uint16_t size_y,
                               std::uint16_t game_length, std::uint16_t turn, std::map<PlayerId, Player> players,
                               std::map<PlayerId, Position> player_positions, std::vector<Position> blocks,
                               std::vector<Bomb> bombs, std::vector<Position> explosions,
                               std::map<PlayerId, Score> scores) :
        GuiMessage(GuiMessageType::Game), server_name(std::move(server_name)), size_x(size_x), size_y(size_y),
        game_length(game_length), turn(turn), players(std::move(players)), player_positions(std::move(player_positions)),
        blocks(std::move(blocks)), bombs(std::move(bombs)), explosions(std::move(explosions)), scores(std::move(scores)) {}

GuiMessageGame::GuiMessageGame(UdpBytestream &bytestream) : GuiMessage(GuiMessageType::Game) {
    server_name = deserialize_string(bytestream.get_bytes(bytestream.get_byte()));
    size_x = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    size_y = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    game_length = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    turn = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));

    size_t players_map_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));

    for (size_t i = 0; i < players_map_size; i++) {
        players.insert({bytestream.get_byte(), Player(bytestream)});
    }

    size_t player_positions_map_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));

    for (size_t i = 0; i < player_positions_map_size; i++) {
        player_positions.insert({bytestream.get_byte(), Position(bytestream)});
    }

    size_t blocks_vector_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    blocks.reserve(blocks_vector_size);

    for (size_t i = 0; i < blocks_vector_size; i++) {
        blocks.emplace_back(bytestream);
    }

    size_t bombs_vector_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    bombs.reserve(blocks_vector_size);

    for (size_t i = 0; i < bombs_vector_size; i++) {
        bombs.emplace_back(bytestream);
    }

    size_t explosions_vector_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    explosions.reserve(explosions_vector_size);

    for (size_t i = 0; i < explosions_vector_size; i++) {
        explosions.emplace_back(bytestream);
    }

    size_t scores_map_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));

    for (size_t i = 0; i < scores_map_size; i++) {
        scores.insert({bytestream.get_byte(), deserialize_uint32(bytestream.get_bytes(UINT32_SIZE))});
    }
}

ByteList GuiMessageGame::serialize() {
    ByteList result;
    result.push_back(gui_message_type);
    append_to_vector(result, serialize_string(server_name));
    append_to_vector(result, serialize_uint16(size_x));
    append_to_vector(result, serialize_uint16(size_y));
    append_to_vector(result, serialize_uint16(game_length));
    append_to_vector(result, serialize_uint16(turn));

    append_to_vector(result, serialize_uint32(players.size()));

    for (const auto& player : players) {
        result.push_back(player.first);
        append_to_vector(result, player.second.serialize());
    }

    append_to_vector(result, serialize_uint32(player_positions.size()));

    for (const auto& player_position: player_positions) {
        result.push_back(player_position.first);
        append_to_vector(result, player_position.second.serialize());
    }

    append_to_vector(result, serialize_uint32(blocks.size()));

    for (const auto& block : blocks) {
        append_to_vector(result, block.serialize());
    }

    append_to_vector(result, serialize_uint32(bombs.size()));

    for (const auto& bomb : bombs) {
        append_to_vector(result, bomb.serialize());
    }

    append_to_vector(result, serialize_uint32(explosions.size()));

    for (const auto& explosion : explosions) {
        append_to_vector(result, explosion.serialize());
    }

    append_to_vector(result, serialize_uint32(scores.size()));

    for (const auto& score: scores) {
        result.push_back(score.first);
        append_to_vector(result, serialize_uint32(score.second));
    }

    return result;
}
