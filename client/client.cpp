#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "debug_functions.hpp"
#include "models/client_message.hpp"
#include "models/server_message.hpp"
#include "models/event.hpp"
#include "serializers/std_types_serializers.hpp"
#include "management/client_options.hpp"

namespace po = boost::program_options;

int main(int argc, char** argv) {
    ClientOptions client_options;

    try {
        client_options.parse_options(argc, argv);
    }
    catch (const ClientOptionsException &e) {
        std::cerr << e.what();
        std::cout << client_options.get_help_message();
        return 0;
    }
    catch (const ClientOptionsGetHelp &e) {
        std::cout << client_options.get_help_message();
        return 0;
    }

    print_client_params(client_options);

    //connection
    boost::asio::io_context io_context;

    std::string str("Żółć!");

    auto test = serialize_string(str);
    print_serialized(test);

    ClientMessageJoin test2("Ąćźśż");
    std::cout << test2.to_string();
    print_serialized(test2.serialize());

    ClientMessageMove test3(Direction::Right);
    std::cout << test3.to_string();
    print_serialized(test3.serialize());

    ClientMessage test4(ClientMessageType::PlaceBlock);
    std::cout << test4.to_string();
    print_serialized(test4.serialize());

    std::shared_ptr<ClientMessage> x = std::make_shared<ClientMessageJoin>(test2);
    std::cout << x->to_string();
    print_serialized(x->serialize());

    std::vector<Position> positions;
    positions.emplace_back(10, 20);
    positions.emplace_back(0, 0);
    positions.emplace_back(5, 200);
    positions.emplace_back(101, 2);

    std::vector<PlayerId> playersId;

    EventBombPlaced eventBombPlaced(24895792, {10, 20});
    EventBombExploded eventBombExploded(4295693, playersId, positions);
    EventPlayerMoved eventPlayerMoved(0, positions[3]);
    EventBlockPlaced eventBlockPlaced(positions[2]);

    std::vector<std::shared_ptr<Event>> events;
    events.push_back(std::make_shared<EventBlockPlaced>(eventBlockPlaced));
    events.push_back(std::make_shared<EventBombExploded>(eventBombExploded));
    events.push_back(std::make_shared<EventBombPlaced>(eventBombPlaced));
    events.push_back(std::make_shared<EventPlayerMoved>(eventPlayerMoved));

    for (const auto& event : events) {
        std::cout << event->to_string();
        print_serialized(event->serialize());
    }

    ServerMessageHello serverMessageHello(client_options.get_server_address(), 10, 4, 2, 69, 2137, 8);
    std::cout << serverMessageHello.to_string();
    print_serialized(serverMessageHello.serialize());

    ServerMessageAcceptedPlayer serverMessageAcceptedPlayer(3, {client_options.get_player_name(), client_options.get_gui_address()});
    std::cout << serverMessageAcceptedPlayer.to_string();
    print_serialized(serverMessageAcceptedPlayer.serialize());

    std::map<PlayerId, Player> players;
    players.insert({5, {"adam", "xd"}});
    players.insert({2, {"dawid", "lol"}});

    ServerMessageGameStarted serverMessageGameStarted(players);
    std::cout << serverMessageGameStarted.to_string();
    print_serialized(serverMessageGameStarted.serialize());

    ServerMessageTurn serverMessageTurn(123, events);
    std::cout << serverMessageTurn.to_string();
    print_serialized(serverMessageTurn.serialize());

    std::map<PlayerId, Score> scores;
    scores.insert({6, 40000000});
    scores.insert({2, 0});

    ServerMessageGameEnded serverMessageGameEnded(scores);
    std::cout << serverMessageGameEnded.to_string();
    print_serialized(serverMessageGameEnded.serialize());

    auto test5 = serverMessageGameEnded.serialize();
    std::cout << deserialize_uint32(test5, 11) << "\n";
    std::cout << deserialize_string(test, 1, test[0]) << "\n";
    print_debug_msg_end();

    auto test2_des = ClientMessageJoin(test2);
    auto test3_des = ClientMessageMove(test3);
    auto test4_des = ClientMessage(test4);
    print_deserialized(test2_des.to_string());
    print_deserialized(test3_des.to_string());
    print_deserialized(test4_des.to_string());

    return 0;
}
