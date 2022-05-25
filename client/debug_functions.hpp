#ifndef ROBOTS_CLIENT_DEBUG_FUNCTIONS_HPP
#define ROBOTS_CLIENT_DEBUG_FUNCTIONS_HPP

#include <string>
#include <vector>
#include <cstdint>

#include "models/client_message.hpp"
#include "management/client_options.hpp"
#include "types.hpp"

void print_debug_msg_beg(std::string name);
void print_debug_msg_end();
void print_serialized(const ByteList& x);
void print_client_params(const ClientOptions& options);
void print_deserialized(const std::string& deserialized);

#endif //ROBOTS_CLIENT_DEBUG_FUNCTIONS_HPP
