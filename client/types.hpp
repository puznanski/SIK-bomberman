#ifndef ROBOTS_CLIENT_TYPES_HPP
#define ROBOTS_CLIENT_TYPES_HPP

#include <cstdint>
#include <vector>

const size_t UINT16_SIZE = sizeof(std::uint16_t);
const size_t UINT32_SIZE = sizeof(std::uint32_t);

using ByteList = std::vector<std::uint8_t>;
using BombId = std::uint32_t;
using PlayerId = std::uint8_t;
using Score = std::uint32_t;

enum ClientMessageType {
    Join = 0,
    PlaceBomb = 1,
    PlaceBlock = 2,
    Move = 3
};

enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

enum EventType {
    BombPlaced = 0,
    BombExploded = 1,
    PlayerMoved = 2,
    BlockPlaced = 3
};

enum ServerMessageType {
    Hello = 0,
    AcceptedPlayer = 1,
    GameStarted = 2,
    Turn = 3,
    GameEnded = 4
};

enum DrawMessageType {
    Lobby = 0,
    Game = 1
};

enum GuiMessageType {
    PlaceBombGui = 0,
    PlaceBlockGui = 1,
    MoveGui = 2
};

enum GameState {
    InLobby = 0,
    InGame = 1
};

class UdpWrongMessageException : public std::exception {

};

class TcpWrongMessageException : public std::exception {
public:
    const char* what() const noexcept {return "Wrong TCP message";}
};

#endif //ROBOTS_CLIENT_TYPES_HPP
