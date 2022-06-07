#include <iostream>
#include <exception>

#include "server_options.hpp"
#include "server_manager.hpp"

int main(int argc, char** argv) {
    ServerOptions server_options;

    try {
        server_options.parse_options(argc, argv);
    }
    catch (const ServerOptionsException &e) {
        std::cerr << e.what() << "\n";
        std::cerr << server_options.get_help_message();
        return 1;
    }
    catch (const ServerOptionsGetHelp &e) {
        std::cout << server_options.get_help_message();
        return 0;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        std::cerr << server_options.get_help_message();
        return 1;
    }

    std::cout << server_options.to_string();

    try {
        ServerManager server_manager(server_options);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        std::cerr << server_options.get_help_message();
        return 1;
    }

    return 0;
}
