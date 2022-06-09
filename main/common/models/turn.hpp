#ifndef ROBOTS_TURN_HPP
#define ROBOTS_TURN_HPP

#include <cstdlib>
#include <vector>

#include "event.hpp"
#include "../types.hpp"
#include "../networking/tcp_bytestream.hpp"

class Turn {
public:
    Turn() = default;
    Turn(std::uint16_t turn_number, std::vector<std::shared_ptr<Event>> events);
    explicit Turn(TcpBytestream &bytestream);

    std::uint16_t turn_number{};
    std::vector<std::shared_ptr<Event>> events;

    ByteList serialize();
};


#endif //ROBOTS_TURN_HPP
