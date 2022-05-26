#include "client_game_state.hpp"

#include "../models/server_message.hpp"

bool ClientGameState::is_game_started() {
    std::scoped_lock lock(mutex);
    return game_state == GameState::InGame;
}

bool ClientGameState::is_in_lobby() {
    std::scoped_lock lock(mutex);
    return game_state == GameState::InLobby;
}

bool ClientGameState::is_join_message_sent() const {
    return join_message_sent;
}

void ClientGameState::set_join_message_sent(bool value) {
    join_message_sent = value;
}

void ClientGameState::set_from_hello_message(ServerMessageHello &server_message_hello) {
    server_name = server_message_hello.server_name;
    players_count = server_message_hello.players_count;
    size_x = server_message_hello.size_x;
    size_y = server_message_hello.size_y;
    game_length = server_message_hello.game_length;
    explosion_radius = server_message_hello.explosion_radius;
    bomb_timer = server_message_hello.bomb_timer;
}

std::shared_ptr<DrawMessage> ClientGameState::get_lobby_draw_message() {
    return std::make_shared<DrawMessageLobby>(server_name, players_count, size_x,
                                              size_y, game_length, explosion_radius, bomb_timer, players);
}

void ClientGameState::add_player(PlayerId id, const Player& player) {
    players.insert({id, player});
}

std::shared_ptr<DrawMessage> ClientGameState::get_game_draw_message() {
    std::vector<Bomb> bombs_vector;
    bombs_vector.reserve(bombs.size());

    for (const auto& bomb : bombs) {
        bombs_vector.push_back(bomb.second);
    }

    std::vector<Position> blocks_vector = {blocks.begin(), blocks.end()};
    std::vector<Position> explosions_vector = {explosions.begin(), explosions.end()};

    return std::make_shared<DrawMessageGame>(server_name, size_x, size_y, game_length, turn, players,
                                             player_positions, blocks_vector, bombs_vector,
                                             explosions_vector, scores);
}

void ClientGameState::set_players_map(const std::map<PlayerId, Player> &value) {
    players = value;
    
    for (const auto& player : players) {
        scores.insert({player.first, 0});
    }
}

void ClientGameState::add_bomb(BombId id, const Position &position) {
    bombs.insert({id, {position, bomb_timer}});
}

void ClientGameState::add_explosion_event(BombId bomb_id, std::vector<PlayerId> player_ids,
                                          std::vector<Position> block_positions) {
    bombs_exploded.insert(bomb_id);
    robots_destroyed.insert(player_ids.begin(), player_ids.end());
    blocks_destroyed.insert(block_positions.begin(), block_positions.end());
}

void ClientGameState::change_player_position(PlayerId id, const Position &new_position) {
    player_positions[id] = new_position;
}

void ClientGameState::set_scores(const std::map<PlayerId, Score> &end_scores) {
    scores = end_scores;
}

void ClientGameState::add_block(const Position &block_position) {
    blocks.insert(block_position);
}

void ClientGameState::set_turn(std::uint16_t value) {
    turn = value;

    for (auto &bomb : bombs) {
        bomb.second.timer--;
    }
}

void ClientGameState::set_game_state(GameState value) {
    std::scoped_lock lock(mutex);
    game_state = value;
}

void ClientGameState::clear_variables() {
    set_game_state(GameState::InLobby);
    join_message_sent = false;
    players.clear();
    turn = 0;
    player_positions.clear();
    blocks.clear();
    bombs.clear();
    explosions.clear();
    scores.clear();
    bombs_exploded.clear();
    robots_destroyed.clear();
    blocks_destroyed.clear();
}

void ClientGameState::handle_explosions() {
    explosions.clear();

    for (auto bomb : bombs_exploded) {
        auto bomb_position = bombs.at(bomb).position;
        explosions.insert(bomb_position);

        auto range = static_cast<std::int32_t>(std::max(0, bomb_position.x - explosion_radius));

        for (std::int32_t x = bomb_position.x; x >= range; x--) {
            Position current_position = {static_cast<std::uint16_t>(x), bomb_position.y};
            explosions.insert(current_position);
            if (blocks.contains(current_position)) break;
        }

        range = static_cast<std::int32_t>(std::min(static_cast<std::int32_t>(size_x - 1), bomb_position.x + explosion_radius));

        for (std::int32_t x = bomb_position.x; x <= range; x++) {
            Position current_position = {static_cast<std::uint16_t>(x), bomb_position.y};
            explosions.insert(current_position);
            if (blocks.contains(current_position)) break;
        }

        range = static_cast<std::int32_t>(std::max(0, bomb_position.y - explosion_radius));

        for (std::int32_t y = bomb_position.y; y >= range; y--) {
            Position current_position = {bomb_position.x, static_cast<std::uint16_t>(y)};
            explosions.insert(current_position);
            if (blocks.contains(current_position)) break;
        }

        range = static_cast<std::int32_t>(std::min(static_cast<std::int32_t>(size_y - 1), bomb_position.y + explosion_radius));

        for (std::int32_t y = bomb_position.y; y <= range; y++) {
            Position current_position = {bomb_position.x, static_cast<std::uint16_t>(y)};
            explosions.insert(current_position);
            if (blocks.contains(current_position)) break;
        }

        bombs.erase(bomb);
    }

    for (const auto& player : robots_destroyed) {
        scores.at(player)++; 
    }

    for (auto block : blocks_destroyed) {
        blocks.erase(block);
    }

    bombs_exploded.clear();
    robots_destroyed.clear();
    blocks_destroyed.clear();
}
