#include "debug_functions.hpp"

#include <iostream>
#include <algorithm>

void print_debug_msg_beg(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);
    std::cout << "DEBUG - " << name << "\n";
}

void print_debug_msg_end() {
    std::cout << "================================================================================\n";
};

void print_serialized(const ByteList& x) {
    print_debug_msg_beg("serialization");
    std::cout << "[";
    size_t len = x.size();
    size_t i = 0;
    for (auto a : x) {
        std::cout << a - 0;
        if (i != len - 1) std::cout << ", ";
        i++;
    }
    std::cout << "]\n";
    print_debug_msg_end();
}

void print_client_params(const ClientOptions& options) {
    print_debug_msg_beg("params");
    std::cout << "GUI address: " << options.get_gui_address() << "\n";
    std::cout << "Player name: " << options.get_player_name() << "\n";
    std::cout << "Port: " << options.get_port() << "\n";
    std::cout << "Server address: " << options.get_server_address() << "\n";
    print_debug_msg_end();
}