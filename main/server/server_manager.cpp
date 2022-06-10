#include "server_manager.hpp"

#include <utility>

ServerManager::ServerManager(ServerOptions server_options) : server_options(std::move(server_options)),
    server_game_state(server_options),
    lobby(server_options.get_players_count()),
    acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), server_options.get_port())) {}

void ServerManager::listen_for_clients() {
    while (true) {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
        boost::asio::ip::tcp::endpoint endpoint;
        acceptor.accept(*socket, endpoint);

        std::lock_guard lock(manager_mutex);

        GameClient& game_client = game_clients.emplace_back(socket, endpoint, server_options, manager_mutex, lobby);
        std::thread thread_game_client([&game_client](){game_client.start_communication();});
        game_client.set_thread(std::move(thread_game_client));

        game_client.send_hello_message();

        if (!game_started) {
            for (auto const& player : players) {
                game_client.enqueue_message(std::make_shared<ServerMessageAcceptedPlayer>(player.first, player.second));
            }
        }
        else {
            auto server_message_game_started = std::make_shared<ServerMessageGameStarted>(players);
            game_client.enqueue_message(server_message_game_started);

            for (auto const& turn : server_game_state.get_turns()) {
                game_client.enqueue_message(std::make_shared<ServerMessageTurn>(turn));
            }
        }

        auto it = game_clients.begin();
        while (it != game_clients.end()) {
            if (it->get_error()) {
                it = game_clients.erase(it);
            }
            else {
                it++;
            }
        }
    }
}

void ServerManager::handle() {
    while (true) {
        while (players.size() != server_options.get_players_count()) {
            auto player = lobby.dequeue();
            players.insert(player);
            send_to_clients(std::make_shared<ServerMessageAcceptedPlayer>(player.first, player.second));
        }

        start_game();

        while (!server_game_state.is_game_finished()) {
            clear_events();

            std::this_thread::sleep_for(std::chrono::milliseconds(server_options.get_turn_duration()));
            std::lock_guard lock(manager_mutex);
            std::map<PlayerId, std::optional<PlayerMovement>> player_movements;

            for (auto const& client : game_clients) {
                if (auto id = client.get_player_id()) {
                    player_movements.insert({id.value(), client.get_movement()});
                }
            }

            auto turn = server_game_state.handle_turn(player_movements);
            send_to_clients(std::make_shared<ServerMessageTurn>(turn));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(server_options.get_turn_duration()));
        end_game();
    }
}

void ServerManager::start_game() {
    std::lock_guard lock(manager_mutex);
    send_to_clients(std::make_shared<ServerMessageGameStarted>(players));
    game_started = true;

    std::vector<PlayerId> ids;
    for (auto const& player : players) {
        ids.push_back(player.first);
    }

    auto turn_zero = server_game_state.initialize_game(ids);
    send_to_clients(std::make_shared<ServerMessageTurn>(turn_zero));
}

void ServerManager::end_game() {
    std::lock_guard lock(manager_mutex);
    send_to_clients(std::make_shared<ServerMessageGameEnded>(server_game_state.get_scores()));
    game_started = false;

    for (auto& client : game_clients) {
        client.end_game();
    }

    players.clear();
    lobby.clear();
}

void ServerManager::send_to_clients(std::shared_ptr<ServerMessage> msg) {
    for (auto& client : game_clients) {
        if (!client.get_error()) {
            client.enqueue_message(msg);
        }
    }
}

void ServerManager::clear_events() {
    std::lock_guard lock(manager_mutex);

    for (auto& client : game_clients) {
        client.clear_movement();
    }
}

void ServerManager::start() {
    std::thread main_game_thread(&ServerManager::handle, this);

    try {
        listen_for_clients();
    }
    catch (const std::exception &e) {
        main_game_thread.join();
    }
}

