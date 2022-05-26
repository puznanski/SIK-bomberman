#include "client_options.hpp"

#include <boost/program_options.hpp>
#include <sstream>

namespace po = boost::program_options;

const std::uint16_t NUMBER_OF_REQUIRED_OPTIONS = 4;
const char SEPARATOR = ':';

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

    port = static_cast<uint16_t>(vm["port"].as<std::int32_t>());
    player_name = vm["player-name"].as<std::string>();

    auto gui_address = vm["gui-address"].as<std::string>();
    auto server_address = vm["server-address"].as<std::string>();

    auto x = gui_address.rfind(SEPARATOR);
    if (x == std::string::npos || x == gui_address.size() - 1) throw ClientOptionsException();
    gui_address_host_name = gui_address.substr(0, x);
    gui_address_port = gui_address.substr(x + 1);

    x = server_address.rfind(SEPARATOR);
    if (x == std::string::npos || x == server_address.size() - 1) throw ClientOptionsException();
    server_address_host_name = server_address.substr(0, x);
    server_address_port = server_address.substr(x + 1);
}

const std::string &ClientOptions::get_help_message() const {
    return help_message;
}

const std::string &ClientOptions::get_player_name() const {
    return player_name;
}

std::uint16_t ClientOptions::get_port() const {
    return port;
}

const std::string &ClientOptions::get_server_address_host_name() const {
    return server_address_host_name;
}

const std::string &ClientOptions::get_server_address_port() const {
    return server_address_port;
}

const std::string &ClientOptions::get_gui_address_host_name() const {
    return gui_address_host_name;
}

const std::string &ClientOptions::get_gui_address_port() const {
    return gui_address_port;
}
