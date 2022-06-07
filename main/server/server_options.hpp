#ifndef ROBOTS_SERVER_OPTIONS_HPP
#define ROBOTS_SERVER_OPTIONS_HPP

#include <exception>
#include <string>
#include <cstdint>
#include <utility>

class ServerOptionsException : public std::exception {
public:
    ServerOptionsException() : message("Wrong parameters passed") {};
    explicit ServerOptionsException(std::string message) : message(std::move(message)) {};

private:
    std::string message;

public:
    [[nodiscard]] const char *what() const noexcept override {
        return message.c_str();
    }
};

class ServerOptionsGetHelp : public std::exception {};

class ServerOptions {
public:
    ServerOptions() = default;

private:
    std::string help_message;
    std::uint16_t bomb_timer{};
    std::uint8_t players_count{};
    std::uint64_t turn_duration{};
    std::uint16_t explosion_radius{};
    std::uint16_t initial_blocks{};
    std::uint16_t game_length{};
    std::string server_name;
    std::uint16_t port{};
    std::uint32_t seed{};
    std::uint16_t size_x{};
    std::uint16_t size_y{};

public:
    void parse_options(int argc, char** argv);

    [[nodiscard]] const std::string &get_help_message() const;
    [[nodiscard]] std::uint16_t get_bomb_timer() const;
    [[nodiscard]] std::uint8_t get_players_count() const;
    [[nodiscard]] std::uint64_t get_turn_duration() const;
    [[nodiscard]] std::uint16_t get_explosion_radius() const;
    [[nodiscard]] std::uint16_t get_initial_blocks() const;
    [[nodiscard]] std::uint16_t get_game_length() const;
    [[nodiscard]] const std::string &get_server_name() const;
    [[nodiscard]] std::uint16_t get_port() const;
    [[nodiscard]] std::uint32_t get_seed() const;
    [[nodiscard]] std::uint16_t get_size_x() const;
    [[nodiscard]] std::uint16_t get_size_y() const;

    std::string to_string() const;
};

#endif //ROBOTS_SERVER_OPTIONS_HPP
