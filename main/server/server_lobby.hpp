#ifndef ROBOTS_SERVER_LOBBY_HPP
#define ROBOTS_SERVER_LOBBY_HPP

#include <optional>
#include <queue>
#include <mutex>
#include <utility>
#include <map>
#include <condition_variable>
#include <cstdint>

#include "../common/types.hpp"
#include "../common/models/misc.hpp"

class ServerLobby {
public:
    explicit ServerLobby(std::uint8_t players_count) : players_count(players_count) {};

private:
    std::uint8_t players_count;
    std::queue<std::pair<PlayerId, Player>> queue;
    mutable std::mutex mutex;
    std::condition_variable condition_variable;
    PlayerId next_id{};
    std::map<PlayerId, Player> players;

    void enqueue(const std::pair<PlayerId, Player>& new_element);

public:
    std::pair<PlayerId, Player> dequeue();
    std::optional<PlayerId> add_player(const Player& player);
    std::map<PlayerId, Player> get_players();
    void clear();
};

#endif //ROBOTS_SERVER_LOBBY_HPP
