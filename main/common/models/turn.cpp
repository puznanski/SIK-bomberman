#include "turn.hpp"

#include <utility>

#include "../serializers/std_types_serializers.hpp"

Turn::Turn(std::uint16_t turn_number, std::vector<std::shared_ptr<Event>> events) :
    turn_number(turn_number), events(std::move(events)) {}

Turn::Turn(TcpBytestream &bytestream) {
    turn_number = deserialize_uint16(bytestream.get_bytes(UINT16_SIZE));
    size_t events_vector_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    events.reserve(events_vector_size);

    for (size_t i = 0; i < events_vector_size; i++) {
        auto type = static_cast<EventType>(bytestream.get_byte());

        switch (type) {
            case BombPlaced: {
                events.push_back(std::make_shared<EventBombPlaced>(EventBombPlaced(bytestream)));
                break;
            }
            case BombExploded: {
                events.push_back(std::make_shared<EventBombExploded>(EventBombExploded(bytestream)));
                break;
            }
            case PlayerMoved: {
                events.push_back(std::make_shared<EventPlayerMoved>(EventPlayerMoved(bytestream)));
                break;
            }
            case BlockPlaced: {
                events.push_back(std::make_shared<EventBlockPlaced>(EventBlockPlaced(bytestream)));
                break;
            }
        }
    }
}

ByteList Turn::serialize() {
    ByteList result;
    append_to_vector(result, serialize_uint16(turn_number));
    append_to_vector(result, serialize_uint32(static_cast<std::uint32_t>(events.size())));

    for (const auto& event : events) {
        append_to_vector(result, event->serialize());
    }

    return result;
}
