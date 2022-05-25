#include <iostream>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "debug_functions.hpp"
#include "models/client_message.hpp"
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

    ClientMessageJoin test2("😍💕");
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

    return 0;
}
