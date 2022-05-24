#include "client_options.hpp"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

const std::uint16_t NUMBER_OF_REQUIRED_OPTIONS = 4;

void ClientOptions::parse_options(int argc, char** argv) {
    po::variables_map vm;
    po::options_description description("Usage");
    description.add_options()
            ("gui-address,d", po::value<std::string>(),
             "<(host name):(port) lub (IPv4):(port) or (IPv6):(port)>")
            ("help,h", "Displays help message")
            ("player-name,n", po::value<std::string>(), "<String>")
            ("port,p", po::value<std::int32_t>(), "<u16>")
            ("server-address,s", po::value<std::string>(),
             "<(host name):(port) lub (IPv4):(port) or (IPv6):(port)>");

    std::stringstream string_stream;
    string_stream << description;
    help_message = string_stream.str();

    try {
        po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
        po::notify(vm);
    }
    catch (const po::invalid_option_value &e) {
        throw ClientOptionsException();
    }

    if (vm.count("help")) throw ClientOptionsGetHelp();
    if (vm.size() < NUMBER_OF_REQUIRED_OPTIONS) throw ClientOptionsException();
    if (vm["port"].as<std::int32_t>() < std::numeric_limits<std::uint16_t>::min() ||
            vm["port"].as<std::int32_t>() > std::numeric_limits<std::uint16_t>::max()) {
        throw ClientOptionsException();
    }

    port = vm["port"].as<std::int32_t>();
    gui_address = vm["gui-address"].as<std::string>();
    player_name = vm["player-name"].as<std::string>();
    server_address = vm["server-address"].as<std::string>();
}

const std::string &ClientOptions::get_help_message() const {
    return help_message;
}

const std::string &ClientOptions::get_gui_address() const {
    return gui_address;
}

const std::string &ClientOptions::get_player_name() const {
    return player_name;
}

std::uint16_t ClientOptions::get_port() const {
    return port;
}

const std::string &ClientOptions::get_server_address() const {
    return server_address;
}
