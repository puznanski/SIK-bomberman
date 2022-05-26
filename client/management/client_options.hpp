#ifndef ROBOTS_CLIENT_CLIENT_OPTIONS_HPP
#define ROBOTS_CLIENT_CLIENT_OPTIONS_HPP

#include <string>
#include <cstdint>

class ClientOptionsException : public std::exception {
public:
    [[nodiscard]] const char* what() const noexcept override {
        return "Wrong parameters passed\n";
    }
};

class ClientOptionsGetHelp : public std::exception {};

class ClientOptions {
public:
    ClientOptions() = default;

private:
    std::string help_message;
    std::string player_name;
    std::uint16_t port{};
    std::string server_address_host_name;
    std::string server_address_port;
    std::string gui_address_host_name;
    std::string gui_address_port;

public:
    void parse_options(int argc, char** argv);

    [[nodiscard]] const std::string &get_help_message() const;
    [[nodiscard]] const std::string &get_player_name() const;
    [[nodiscard]] std::uint16_t get_port() const;
    [[nodiscard]] const std::string &get_server_address_host_name() const;
    [[nodiscard]] const std::string &get_server_address_port() const;
    [[nodiscard]] const std::string &get_gui_address_host_name() const;
    [[nodiscard]] const std::string &get_gui_address_port() const;
};

#endif //ROBOTS_CLIENT_CLIENT_OPTIONS_HPP
