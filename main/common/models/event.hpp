#ifndef ROBOTS_CLIENT_EVENT_HPP
#define ROBOTS_CLIENT_EVENT_HPP

#include <vector>

#include "../types.hpp"
#include "../networking/tcp_bytestream.hpp"
#include "../../client/client_game_state.hpp"
#include "misc.hpp"

class Event {
public:
    virtual ~Event() = default;
    explicit Event(EventType event_type) : event_type(event_type) {}

    EventType event_type;

    virtual ByteList serialize() = 0;
    virtual void handle_event(ClientGameState& client_game_state) = 0;
};

class EventBombPlaced : public Event {
public:
    EventBombPlaced(BombId bomb_id, Position position);
    explicit EventBombPlaced(TcpBytestream& bytestream);

    BombId bomb_id;
    Position position{0, 0};

    ByteList serialize() override;
    void handle_event(ClientGameState &client_game_state) override;
};

class EventBombExploded : public Event {
public:
    EventBombExploded(BombId bomb_id, std::vector<PlayerId> robots_destroyed, std::vector<Position> blocks_destroyed);
    explicit EventBombExploded(TcpBytestream& bytestream);

    BombId bomb_id;
    std::vector<PlayerId> robots_destroyed;
    std::vector<Position> blocks_destroyed;

    ByteList serialize() override;
    void handle_event(ClientGameState &client_game_state) override;
};

class EventPlayerMoved : public Event {
public:
    EventPlayerMoved(PlayerId id, Position position);
    explicit EventPlayerMoved(TcpBytestream& bytestream);

    PlayerId id;
    Position position;

    ByteList serialize() override;
    void handle_event(ClientGameState &client_game_state) override;
};

class EventBlockPlaced : public Event {
public:
    explicit EventBlockPlaced(Position position);
    explicit EventBlockPlaced(TcpBytestream& bytestream);

    Position position;

    ByteList serialize() override;
    void handle_event(ClientGameState &client_game_state) override;
};

#endif //ROBOTS_CLIENT_EVENT_HPP
