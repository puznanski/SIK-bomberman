#ifndef ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
#define ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP

#include <vector>
#include <cstdint>
#include <string>

#include "../types.hpp"

ByteList serialize_string(const std::string& str);

#endif //ROBOTS_CLIENT_STD_TYPES_SERIALIZERS_HPP
