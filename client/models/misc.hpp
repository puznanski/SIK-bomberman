#ifndef ROBOTS_CLIENT_MISC_HPP
#define ROBOTS_CLIENT_MISC_HPP

#include <string>
#include <memory>

#include "../types.hpp"

class Position {
public:
    Position(std::uint16_t x, std::uint16_t y);
    Position(const ByteList& message, size_t beg);

    std::uint16_t x;
    std::uint16_t y;

    ByteList serialize() const;
};

class Bomb {
public:
    Bomb(Position position, std::uint16_t timer);

    Position position;
    std::uint16_t timer;

    ByteList serialize() const;
};

class Player {
public:
    Player(std::string name, std::string address);

    std::string name;
    std::string address;

    ByteList serialize() const;
};

void append_to_vector(ByteList& dest, const ByteList& src);

#endif //ROBOTS_CLIENT_MISC_HPP
