#include <iostream>
#include <string>

#include "client_options.hpp"
#include "client_manager.hpp"

int main(int argc, char** argv) {
    ClientOptions client_options;

    try {
        client_options.parse_options(argc, argv);
    }
    catch (const ClientOptionsException &e) {
        std::cerr << e.what() << "\n";
        std::cerr << client_options.get_help_message();
        return 1;
    }
    catch (const ClientOptionsGetHelp &e) {
        std::cout << client_options.get_help_message();
        return 0;
    }

    try {
        ClientManager client_manager(client_options);
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        std::cerr << client_options.get_help_message();
        return 1;
    }

    return 0;
}
