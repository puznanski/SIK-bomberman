#ifndef ROBOTS_CLIENT_MISC_HPP
#define ROBOTS_CLIENT_MISC_HPP

#include <string>
#include <memory>

#include "../networking/tcp_bytestream.hpp"
#include "../networking/udp_bytestream.hpp"
#include "../types.hpp"

class Position {
public:
    Position();
    Position(std::uint16_t x, std::uint16_t y);
    explicit Position(TcpBytestream& bytestream);
    explicit Position(UdpBytestream& bytestream);

    std::uint16_t x;
    std::uint16_t y;

    ByteList serialize() const;
    bool operator<(const Position &rhs) const;
};

class Bomb {
public:
    Bomb(Position position, std::uint16_t timer);
    explicit Bomb(TcpBytestream& bytestream);
    explicit Bomb(UdpBytestream& bytestream);

    Position position{};
    std::uint16_t timer;

    ByteList serialize() const;
};

class Player {
public:
    Player();
    Player(std::string name, std::string address);
    explicit Player(TcpBytestream& bytestream);
    explicit Player(UdpBytestream& bytestream);

    std::string name;
    std::string address;

    ByteList serialize() const;
};

void append_to_vector(ByteList& dest, const ByteList& src);

#endif //ROBOTS_CLIENT_MISC_HPP
