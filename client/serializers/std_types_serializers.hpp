#ifndef ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
#define ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP

#include <vector>
#include <cstdint>
#include <string>

#include "../types.hpp"

ByteList serialize_string(const std::string& str);
ByteList serialize_uint16(std::uint16_t val);
ByteList serialize_uint32(std::uint32_t val);

#endif //ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
