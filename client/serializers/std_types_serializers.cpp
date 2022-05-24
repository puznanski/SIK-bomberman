#include "std_types_serializers.hpp"

ByteList serialize_string(const std::string& str) {
    ByteList result;
    result.reserve(str.size() + 1);
    result.emplace_back(str.size());
    for (std::uint8_t c : str) {
        result.emplace_back(c);
    }

    return result;
}
