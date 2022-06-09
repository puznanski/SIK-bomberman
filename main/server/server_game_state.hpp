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
#include "../common/models/server_message.hpp"
#include "../common/models/client_message.hpp"
#include "../common/models/player_movement.hpp"

class ServerGameState {
public:
    explicit ServerGameState(ServerOptions server_options);

private:
    ServerOptions server_options;
    std::map<PlayerId, Player> players;
    std::minstd_rand random_generator;

    bool is_game_started = false;
    Turn current_turn;
    std::vector<Turn> turns;
    std::map<PlayerId, Position> player_positions;
    std::map<PlayerId, Score> scores;
    std::map<BombId, Bomb> bombs;
    BombId next_bomb_id;
    std::set<Position> blocks;

    std::vector<PlayerId> robots_destroyed;
    std::vector<Position> blocks_destroyed;

    Position get_random_position();
    bool handle_explosion(Position position);
    // bool value tells us if the movement occurred
    std::pair<bool, Position> handle_move(const Position &old_position, Direction direction);

public:
    ServerMessageTurn initialize_game(std::map<PlayerId, Player> players_map);
    ServerMessageTurn handle_turn(std::map<PlayerId, PlayerMovement> player_movements);
};

#endif //ROBOTS_SERVER_GAME_STATE_HPP