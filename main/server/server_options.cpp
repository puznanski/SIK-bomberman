#include "server_options.hpp"

#include <boost/program_options.hpp>
#include <sstream>
#include <chrono>
#include <limits>
#include <algorithm>

namespace po = boost::program_options;

const std::uint16_t NUMBER_OF_REQUIRED_OPTIONS = 10;

template<typename T1, typename T2>
T1 parse_numeric_value(const po::variables_map& vm, const std::string& name) {
    T2 value = vm[name].as<T2>();

    if (value < std::numeric_limits<T1>::min() || value > std::numeric_limits<T1>::max()) {
        std::stringstream ss;
        ss << name << " out of range";
        throw ServerOptionsException(ss.str());
    }

    return static_cast<T1>(value);
}

bool is_string_numeric(const std::string& str) {
    return std::ranges::all_of(str.begin(), str.end(), [](auto const& c){return std::isdigit(c) != 0;});
}

std::uint64_t parse_turn_duration(const std::string& str) {
    if (!is_string_numeric(str)) throw ServerOptionsException("turn-duration is not valid");
    std::uint64_t value;
    std::istringstream iss(str);
    iss.exceptions(std::ios::failbit | std::ios::badbit);

    try {
        iss >> value;
    }
    catch (const std::exception& e) {
        throw ServerOptionsException("turn-duration out of range");
    }

    return value;
}

void ServerOptions::parse_options(int argc, char **argv) {
    po::variables_map vm;
    po::options_description description("Usage");
    description.add_options()
            ("bomb-timer,b", po::value<std::int32_t>(),"<u16>")
            ("players-count,c", po::value<std::int16_t>(), "<u8>")
            ("turn-duration,d", po::value<std::string>(), "<u64, milliseconds>")
            ("explosion-radius,e", po::value<std::int32_t>(), "<u16>")
            ("help,h", "Displays help message")
            ("initial-blocks,k", po::value<std::int32_t>(), "<u16>")
            ("game-length,l", po::value<std::int32_t>(), "<u16>")
            ("server-name,n", po::value<std::string>(), "<String>")
            ("port,p", po::value<std::int32_t>(), "<u16>")
            ("seed,s", po::value<std::int64_t>(), "<u32, optional>")
            ("size-x,x", po::value<std::int32_t>(), "<u16>")
            ("size-y,y", po::value<std::int32_t>(), "<u16>");

    std::stringstream string_stream;
    string_stream << description;
    help_message = string_stream.str();

    try {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);
    }
    catch (const po::invalid_option_value &e) {
        throw ServerOptionsException(e.what());
    }

    if (vm.count("help")) throw ServerOptionsGetHelp();
    if (vm.size() < NUMBER_OF_REQUIRED_OPTIONS) throw ServerOptionsException("Not enough arguments");
    if (vm.count("seed") && vm.size() == NUMBER_OF_REQUIRED_OPTIONS) throw ServerOptionsException("Not enough arguments");

    if (vm.count("seed")) {
        seed = vm["seed"].as<std::uint32_t>();
    }
    else {
        seed = static_cast<std::uint32_t>(std::chrono::system_clock::now().time_since_epoch().count());
    }

    bomb_timer = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "bomb-timer");
    players_count = parse_numeric_value<std::uint8_t, std::int16_t>(vm, "players-count");
    turn_duration = parse_turn_duration(vm["turn-duration"].as<std::string>());
    explosion_radius = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "explosion-radius");
    initial_blocks = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "initial-blocks");
    game_length = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "game-length");
    server_name = vm["server-name"].as<std::string>();
    port = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "port");
    size_x = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "size-x");
    size_y = parse_numeric_value<std::uint16_t, std::int32_t>(vm, "size-y");
}

const std::string &ServerOptions::get_help_message() const {
    return help_message;
}

std::uint16_t ServerOptions::get_bomb_timer() const {
    return bomb_timer;
}

std::uint8_t ServerOptions::get_players_count() const {
    return players_count;
}

std::uint64_t ServerOptions::get_turn_duration() const {
    return turn_duration;
}

std::uint16_t ServerOptions::get_explosion_radius() const {
    return explosion_radius;
}

std::uint16_t ServerOptions::get_initial_blocks() const {
    return initial_blocks;
}

std::uint16_t ServerOptions::get_game_length() const {
    return game_length;
}

const std::string &ServerOptions::get_server_name() const {
    return server_name;
}

std::uint16_t ServerOptions::get_port() const {
    return port;
}

std::uint32_t ServerOptions::get_seed() const {
    return seed;
}

std::uint16_t ServerOptions::get_size_x() const {
    return size_x;
}

std::uint16_t ServerOptions::get_size_y() const {
    return size_y;
}

std::string ServerOptions::to_string() const {
    std::stringstream ss;

    ss << "Server options:\n";
    ss << "--bomb-timer: " << bomb_timer << "\n";
    ss << "--players-count: " << players_count + 0 << "\n";
    ss << "--turn-duration: " << turn_duration << "\n";
    ss << "--explosion-radius: " << explosion_radius << "\n";
    ss << "--initial-blocks: " << initial_blocks << "\n";
    ss << "--game-length: " << game_length << "\n";
    ss << "--server-name: " << server_name << "\n";
    ss << "--port: " << port << "\n";
    ss << "--seed: " << seed << "\n";
    ss << "--size-x: " << size_x << "\n";
    ss << "--size-y: " << size_y << "\n";

    return ss.str();
}
