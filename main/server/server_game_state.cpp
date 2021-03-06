#include "server_game_state.hpp"

#include <memory>

ServerGameState::ServerGameState(ServerOptions server_options) : server_options(std::move(server_options)) {
    random_generator = std::minstd_rand(this->server_options.get_seed());
}

Turn ServerGameState::initialize_game(const std::vector<PlayerId>& players) {
    turns.clear();
    player_positions.clear();
    scores.clear();
    bombs.clear();
    next_bomb_id = 0;
    blocks.clear();

    std::vector<std::shared_ptr<Event>> events;

    for (auto const& player : players) {
        Position position = get_random_position();
        player_positions.insert({player, position});
        scores.insert({player, 0});
        events.push_back(std::make_shared<EventPlayerMoved>(player, position));
    }

    for (std::uint16_t i = 0; i < server_options.get_initial_blocks(); i++) {
        Position position = get_random_position();
        auto x = blocks.insert(position);
        if (x.second) events.push_back(std::make_shared<EventBlockPlaced>(position));
    }

    current_turn = Turn(0, events);
    turns.push_back(current_turn);
    return current_turn;
}

Turn ServerGameState::handle_turn(std::map<PlayerId, std::optional<PlayerMovement>> player_movements) {
    robots_destroyed.clear();
    blocks_destroyed.clear();
    std::vector<std::shared_ptr<Event>> events;
    std::set<PlayerId> all_destroyed_robots;
    std::set<Position> all_destroyed_blocks;
    auto explosion_radius = server_options.get_explosion_radius();
    auto bomb_it = bombs.begin();

    while (bomb_it != bombs.end()) {
        bomb_it->second.timer -= 1;

        std::uint16_t timer = bomb_it->second.timer;
        if (timer == 0) {
            if (blocks.contains(bomb_it->second.position)) {
                handle_explosion(bomb_it->second.position);
            }
            else {
                handle_explosion(bomb_it->second.position);

                auto range = static_cast<std::int32_t>(std::max(0, bomb_it->second.position.x - explosion_radius));

                for (std::int32_t x = bomb_it->second.position.x - 1; x >= range; x--) {
                    Position current_position = {static_cast<std::uint16_t>(x), bomb_it->second.position.y};
                    if (handle_explosion(current_position)) break;
                }

                range = static_cast<std::int32_t>(std::min(static_cast<std::int32_t>(server_options.get_size_x() - 1),
                                                           bomb_it->second.position.x + explosion_radius));

                for (std::int32_t x = bomb_it->second.position.x + 1; x <= range; x++) {
                    Position current_position = {static_cast<std::uint16_t>(x), bomb_it->second.position.y};
                    if (handle_explosion(current_position)) break;
                }

                range = static_cast<std::int32_t>(std::max(0, bomb_it->second.position.y - explosion_radius));

                for (std::int32_t y = bomb_it->second.position.y - 1; y >= range; y--) {
                    Position current_position = {bomb_it->second.position.x, static_cast<std::uint16_t>(y)};
                    if (handle_explosion(current_position)) break;
                }

                range = static_cast<std::int32_t>(std::min(static_cast<std::int32_t>(server_options.get_size_y() - 1),
                                                           bomb_it->second.position.y + explosion_radius));

                for (std::int32_t y = bomb_it->second.position.y + 1; y <= range; y++) {
                    Position current_position = {bomb_it->second.position.x, static_cast<std::uint16_t>(y)};
                    if (handle_explosion(current_position)) break;
                }
            }

            all_destroyed_robots.insert(robots_destroyed.begin(), robots_destroyed.end());
            all_destroyed_blocks.insert(blocks_destroyed.begin(), blocks_destroyed.end());
            events.push_back(std::make_shared<EventBombExploded>(bomb_it->first, robots_destroyed, blocks_destroyed));
            bomb_it = bombs.erase(bomb_it);
            robots_destroyed.clear();
            blocks_destroyed.clear();
        }
        else {
            bomb_it++;
        }
    }

    for (auto const& block : all_destroyed_blocks) {
        blocks.erase(block);
    }

    for (auto& player : player_positions) {
        if (all_destroyed_robots.contains(player.first)) {
            Position new_position = get_random_position();
            player.second = new_position;
            scores.at(player.first)++;
            events.push_back(std::make_shared<EventPlayerMoved>(player.first, new_position));
        }
        else {
            auto movement = player_movements.at(player.first);
            if (movement) {
                if (movement.value().movement_type == ClientMessageType::PlaceBomb) {
                    Bomb new_bomb(player.second, server_options.get_bomb_timer());
                    bombs.insert({next_bomb_id, new_bomb});
                    events.push_back(std::make_shared<EventBombPlaced>(next_bomb_id++, player.second));
                }
                else if (movement.value().movement_type == ClientMessageType::PlaceBlock) {
                    if (blocks.insert(player.second).second) {
                        events.push_back(std::make_shared<EventBlockPlaced>(player.second));
                    }
                }
                else if (movement.value().movement_type == ClientMessageType::Move) {
                    auto move = handle_move(player.second, movement.value().direction.value());
                    player_positions.at(player.first) = move.second;
                    if (move.first) events.push_back(std::make_shared<EventPlayerMoved>(player.first, move.second));
                }
            }
        }
    }

    current_turn = Turn(current_turn.turn_number + 1, events);
    turns.push_back(current_turn);
    return current_turn;
}

Position ServerGameState::get_random_position() {
    auto x = static_cast<std::uint16_t>(random_generator() % server_options.get_size_x());
    auto y = static_cast<std::uint16_t>(random_generator() % server_options.get_size_y());

    return {x, y};
}

bool ServerGameState::handle_explosion(Position position) {
    for (auto& player : player_positions) {
        if (player.second == position) {
            robots_destroyed.push_back(player.first);
        }
    }

    if (blocks.contains(position)) {
        blocks_destroyed.push_back(position);
        return true;
    }

    return false;
}

std::pair<bool, Position> ServerGameState::handle_move(const Position &old_position, Direction direction) {
    bool has_moved = false;
    bool out_of_range = false;
    Position position = old_position;

    switch (direction) {
        case Up: {
            if (old_position.y != server_options.get_size_y() - 1) {
                position = Position(old_position.x, old_position.y + 1);
            }
            else {
                out_of_range = true;
            }
            break;
        }
        case Right: {
            if (old_position.x != server_options.get_size_x() - 1) {
                position = Position(old_position.x + 1, old_position.y);
            }
            else {
                out_of_range = true;
            }
            break;
        }
        case Down: {
            if (old_position.y != 0) {
                position = Position(old_position.x, old_position.y - 1);
            }
            else {
                out_of_range = true;
            }
            break;
        }
        case Left: {
            if (old_position.x != 0) {
                position = Position(old_position.x - 1, old_position.y);
            }
            else {
                out_of_range = true;
            }
            break;
        }
    }

    if (blocks.contains(position)) {
        position = old_position;
    }
    else if (!out_of_range) {
        has_moved = true;
    }

    return {has_moved, position};
}

std::vector<Turn> ServerGameState::get_turns() const {
    return turns;
}

std::map<PlayerId, Score> ServerGameState::get_scores() const {
    return scores;
}

bool ServerGameState::is_game_finished() const {
    return current_turn.turn_number >= server_options.get_game_length();
}
