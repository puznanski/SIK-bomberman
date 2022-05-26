#ifndef ROBOTS_CLIENT_CLIENT_GAME_STATE_HPP
#define ROBOTS_CLIENT_CLIENT_GAME_STATE_HPP

#include <mutex>
#include <memory>
#include <map>
#include <vector>
#include <set>

#include "../models/draw_message.hpp"

class ServerMessageHello;

class ClientGameState {
public:
    ClientGameState() = default;

private:
    GameState game_state = GameState::InLobby;
    bool join_message_sent = false;
    std::mutex mutex;

    std::string server_name;
    std::uint8_t players_count{};
    std::uint16_t size_x{};
    std::uint16_t size_y{};
    std::uint16_t game_length{};
    std::uint16_t explosion_radius{};
    std::uint16_t bomb_timer{};
    std::map<PlayerId, Player> players{};
    std::uint16_t turn{};
    std::map<PlayerId, Position> player_positions;
    std::set<Position> blocks;
    std::map<BombId, Bomb> bombs;
    std::set<Position> explosions;
    std::map<PlayerId, Score> scores;
    std::set<BombId> bombs_exploded;
    std::set<PlayerId> robots_destroyed;
    std::set<Position> blocks_destroyed;

public:
    bool is_game_started();
    [[nodiscard]] bool is_join_message_sent() const;

    void set_game_state(GameState value);
    void set_join_message_sent(bool value);
    void set_from_hello_message(ServerMessageHello &server_message_hello);
    void set_players_map(const std::map<PlayerId, Player> &value);
    void set_scores(const std::map<PlayerId, Score> &end_scores);
    void set_turn(std::uint16_t value);

    std::shared_ptr<DrawMessage> get_lobby_draw_message();
    std::shared_ptr<DrawMessage> get_game_draw_message();
    void add_player(PlayerId id, const Player& player);
    void add_bomb(BombId id, const Position& position);
    void add_explosion_event(BombId bomb_id, std::vector<PlayerId> player_ids, std::vector<Position> block_positions);
    void change_player_position(PlayerId id, const Position& new_position);
    void add_block(const Position& block_position);
    void handle_explosions();
    void clear_variables();
};

#endif //ROBOTS_CLIENT_CLIENT_GAME_STATE_HPP
