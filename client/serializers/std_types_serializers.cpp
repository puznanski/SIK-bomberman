#include "std_types_serializers.hpp"

#include <arpa/inet.h>
#include <iostream>

ByteList serialize_string(const std::string& str) {
    ByteList result;
    result.reserve(str.size() + 1);
    result.emplace_back(str.size());
    for (std::uint8_t c : str) {
        result.emplace_back(c);
    }

    return result;
}

ByteList serialize_uint16(std::uint16_t val) {
    ByteList result;
    result.reserve(UINT16_SIZE);

    auto net_val = htons(val);
    auto arr = reinterpret_cast<std::uint8_t*>(&net_val);

    for (size_t i = 0; i < UINT16_SIZE; i++) {
        result.emplace_back(arr[i]);
    }

    return result;
}

ByteList serialize_uint32(std::uint32_t val) {
    ByteList result;
    result.reserve(UINT32_SIZE);

    auto net_val = htonl(val);
    auto arr = reinterpret_cast<std::uint8_t*>(&net_val);

    for (size_t i = 0; i < UINT32_SIZE; i++) {
        result.emplace_back(arr[i]);
    }

    return result;
}
