#ifndef ROBOTS_CLIENT_CLIENT_OPTIONS_HPP
#define ROBOTS_CLIENT_CLIENT_OPTIONS_HPP

#include <string>
#include <cstdint>

class ClientOptionsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Wrong parameters passed\n";
    }
};

class ClientOptionsGetHelp : public std::exception {};

class ClientOptions {
public:
    ClientOptions() = default;

private:
    std::string help_message;
    std::string gui_address;
    std::string player_name;
    std::uint16_t port{};
    std::string server_address;

public:
    void parse_options(int argc, char** argv);

    const std::string &get_help_message() const;
    const std::string &get_gui_address() const;
    const std::string &get_player_name() const;
    std::uint16_t get_port() const;
    const std::string &get_server_address() const;
};

#endif //ROBOTS_CLIENT_CLIENT_OPTIONS_HPP
