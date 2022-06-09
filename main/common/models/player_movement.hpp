#ifndef ROBOTS_PLAYER_MOVEMENT_HPP
#define ROBOTS_PLAYER_MOVEMENT_HPP

#include <optional>

#include "../types.hpp"

class PlayerMovement {
public:
    explicit PlayerMovement(ClientMessageType movement_type) : movement_type(movement_type) {};
    PlayerMovement(ClientMessageType movement_type, Direction direction) :
        movement_type(movement_type), direction(direction) {};

    ClientMessageType movement_type;
    std::optional<Direction> direction;
};


#endif //ROBOTS_PLAYER_MOVEMENT_HPP
