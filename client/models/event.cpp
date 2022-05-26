#include "event.hpp"

#include <sstream>

#include "../serializers/std_types_serializers.hpp"

EventBombPlaced::EventBombPlaced(BombId bomb_id, Position position) : Event(EventType::BombPlaced), bomb_id(bomb_id),
                                                                      position(position) {}

EventBombPlaced::EventBombPlaced(TcpBytestream& bytestream) : Event(EventType::BombPlaced) {
    bomb_id = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    position = Position(bytestream);
}

ByteList EventBombPlaced::serialize() {
    ByteList result;
    result.push_back(event_type);

    ByteList serialized_bomb_id = serialize_uint32(bomb_id);
    append_to_vector(result, serialized_bomb_id);

    ByteList serialized_position = position.serialize();
    append_to_vector(result, serialized_position);

    return result;
}

std::string EventBombPlaced::to_string() {
    std::stringstream ss;
    ss << "Event (BombPlaced): { bomb_id: " << bomb_id << ", position: { x: " << position.x << ", y: " << position.y << " } }\n";

    return ss.str();
}

EventBombExploded::EventBombExploded(BombId bomb_id, std::vector<PlayerId> robots_destroyed,
                                     std::vector<Position> blocks_destroyed) : Event(EventType::BombExploded),
                                     bomb_id(bomb_id), robots_destroyed(std::move(robots_destroyed)), blocks_destroyed(std::move(blocks_destroyed)) {}

EventBombExploded::EventBombExploded(TcpBytestream& bytestream) : Event(EventType::BombExploded) {
    bomb_id = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    size_t robots_destroyed_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    robots_destroyed.reserve(robots_destroyed_size);

    for (size_t i = 0; i < robots_destroyed_size; i++) {
        robots_destroyed.push_back(bytestream.get_byte());
    }

    size_t blocks_destroyed_size = deserialize_uint32(bytestream.get_bytes(UINT32_SIZE));
    blocks_destroyed.reserve(blocks_destroyed_size);

    for (size_t i = 0; i < blocks_destroyed_size; i++) {
        blocks_destroyed.emplace_back(bytestream);
    }
}

ByteList EventBombExploded::serialize() {
    ByteList result;
    result.push_back(event_type);

    ByteList serialized_bomb_id = serialize_uint32(bomb_id);
    append_to_vector(result, serialized_bomb_id);

    ByteList serialized_robots_destroyed_length = serialize_uint16(robots_destroyed.size());
    append_to_vector(result, serialized_robots_destroyed_length);
    append_to_vector(result, robots_destroyed);

    ByteList serialized_blocks_destroyed_length = serialize_uint16(blocks_destroyed.size());
    append_to_vector(result, serialized_blocks_destroyed_length);

    for (const auto &pos : blocks_destroyed) {
        ByteList serialized_position = pos.serialize();
        append_to_vector(result, serialized_position);
    }

    return result;
}

std::string EventBombExploded::to_string() {
    std::stringstream ss;
    ss << "Event (BombExploded): { bomb_id: " << bomb_id << ", robots_destroyed: [";

    size_t len1 = robots_destroyed.size();
    for (size_t i = 0; i < len1; i++) {
        ss << robots_destroyed[i] + 0;
        if (i != len1 - 1) ss << ", ";
    }
    ss << "], blocks_destroyed: [";
    size_t len2 = blocks_destroyed.size();
    for (size_t i = 0; i < len2; i++) {
        ss << "{" << blocks_destroyed[i].x << ", " << blocks_destroyed[i].y << "}";
        if (i != len2 - 1) ss << ", ";
    }
    ss << "] }\n";

    return ss.str();
}

EventPlayerMoved::EventPlayerMoved(PlayerId id, Position position) : Event(EventType::PlayerMoved), id(id), position(position) {}

EventPlayerMoved::EventPlayerMoved(TcpBytestream &bytestream) : Event(EventType::PlayerMoved) {
    id = bytestream.get_byte();
    position = Position(bytestream);
}

ByteList EventPlayerMoved::serialize() {
    ByteList result;
    result.push_back(event_type);
    result.push_back(id);

    ByteList serialized_position = position.serialize();
    append_to_vector(result, serialized_position);

    return result;
}

std::string EventPlayerMoved::to_string() {
    std::stringstream ss;
    ss << "Event (PlayerMoved): { player_id: " << id + 0 << ", position: { x: " << position.x << ", y: " << position.y << " } }\n";

    return ss.str();
}

EventBlockPlaced::EventBlockPlaced(Position position) : Event(EventType::BlockPlaced), position(position) {}

EventBlockPlaced::EventBlockPlaced(TcpBytestream& bytestream) : Event(EventType::BlockPlaced), position(Position(bytestream)) {}

ByteList EventBlockPlaced::serialize() {
    ByteList result;
    result.push_back(event_type);

    ByteList serialized_position = position.serialize();
    append_to_vector(result, serialized_position);

    return result;
}

std::string EventBlockPlaced::to_string() {
    std::stringstream ss;
    ss << "Event (BlockPlaced): { position: { x: " << position.x << ", y: " << position.y << " } }\n";

    return ss.str();
}

