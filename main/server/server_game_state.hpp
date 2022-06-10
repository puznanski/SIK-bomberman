#ifndef ROBOTS_SERVER_GAME_STATE_HPP
#define ROBOTS_SERVER_GAME_STATE_HPP

#include <cstdint>
#include <random>
#include <vector>
#include <map>
#include <utility>
#include <set>

#include "server_options.hpp"
#include "../common/models/misc.hpp"
#include "../common/models/turn.hpp"
#include "../common/models/player_movement.hpp"

class ServerGameState {
public:
    explicit ServerGameState(ServerOptions server_options);

private:
    ServerOptions server_options;
    std::minstd_rand random_generator;

    Turn current_turn;
    std::vector<Turn> turns;
    std::map<PlayerId, Position> player_positions;
    std::map<PlayerId, Score> scores;
    std::map<BombId, Bomb> bombs;
    BombId next_bomb_id{};
    std::set<Position> blocks;

    std::vector<PlayerId> robots_destroyed;
    std::vector<Position> blocks_destroyed;

    Position get_random_position();
    bool handle_explosion(Position position);
    // bool value tells us if the movement occurred
    std::pair<bool, Position> handle_move(const Position &old_position, Direction direction);

public:
    Turn initialize_game(const std::vector<PlayerId>& players);
    Turn handle_turn(std::map<PlayerId, PlayerMovement> player_movements);
    std::vector<Turn> get_turns();
};

#endif //ROBOTS_SERVER_GAME_STATE_HPP
