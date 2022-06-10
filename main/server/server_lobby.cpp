#include "server_lobby.hpp"

void ServerLobby::enqueue(const std::pair<PlayerId, Player>& new_element) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(new_element);
    condition_variable.notify_one();
}

std::pair<PlayerId, Player> ServerLobby::dequeue() {
    std::unique_lock<std::mutex> lock(mutex);

    while(queue.empty()) {
        condition_variable.wait(lock);
    }

    std::pair<PlayerId, Player> element = queue.front();
    queue.pop();

    return element;
}

std::optional<PlayerId> ServerLobby::add_player(const Player& player) {
    if (next_id < players_count) {
        std::pair<PlayerId, Player> new_player(next_id, player);
        enqueue(new_player);
        players.insert(new_player);

        return next_id++;
    }

    return std::nullopt;
}

std::map<PlayerId, Player> ServerLobby::get_players() {
    return players;
}

void ServerLobby::clear() {
    next_id = 0;
}
