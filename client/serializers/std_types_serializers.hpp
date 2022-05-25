#ifndef ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
#define ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP

#include <vector>
#include <cstdint>
#include <string>

#include "../types.hpp"

ByteList serialize_string(const std::string& str);
ByteList serialize_uint16(std::uint16_t val);
ByteList serialize_uint32(std::uint32_t val);

std::string deserialize_string(const ByteList& bytes, size_t beg, size_t len);
std::uint16_t deserialize_uint16(const ByteList& bytes, size_t beg);
std::uint32_t deserialize_uint32(const ByteList& bytes, size_t beg);

#endif //ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
