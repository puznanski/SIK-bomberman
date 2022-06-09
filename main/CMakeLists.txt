cmake_minimum_required(VERSION 3.18)
project(robots)

cmake_host_system_information(RESULT host_name QUERY HOSTNAME)
message("Hostname: " ${host_name})

if(${host_name} STREQUAL "students")
        set(LD_LIBRARY_PATH "/opt/gcc-11.2/lib64")
        set(CMAKE_CXX_COMPILER "/opt/gcc-11.2/bin/g++-11.2")
endif()

set(Boost_INCLUDE_DIR /usr/include/boost)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ADD_COMPILE_OPTIONS(-Wall -Wextra -Wconversion -Werror -O2 -std=gnu++20)

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(robots-client client/client.cpp

        common/models/client_message.cpp common/models/client_message.hpp
        common/models/server_message.cpp common/models/server_message.hpp
        common/models/draw_message.cpp common/models/draw_message.hpp
        common/models/gui_message.cpp common/models/gui_message.hpp
        common/models/event.cpp common/models/event.hpp
        common/models/misc.cpp common/models/misc.hpp
        common/models/turn.cpp common/models/turn.hpp

        common/serializers/std_types_serializers.cpp common/serializers/std_types_serializers.hpp

        client/client_options.cpp client/client_options.hpp
        client/client_manager.cpp client/client_manager.hpp
        client/client_game_state.cpp client/client_game_state.hpp

        common/networking/tcp_bytestream.cpp common/networking/tcp_bytestream.hpp
        common/networking/udp_bytestream.cpp common/networking/udp_bytestream.hpp

        common/types.hpp)

TARGET_INCLUDE_DIRECTORIES(robots-client PRIVATE ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(robots-client LINK_PUBLIC ${Boost_LIBRARIES})

add_executable(robots-server server/server.cpp

        common/models/misc.cpp common/models/misc.hpp
        common/models/turn.cpp common/models/turn.hpp
        common/models/event.cpp common/models/event.hpp
        common/models/server_message.cpp common/models/server_message.hpp
        common/models/client_message.cpp common/models/client_message.hpp
        common/models/draw_message.cpp common/models/draw_message.hpp
        common/models/player_movement.hpp

        common/serializers/std_types_serializers.cpp common/serializers/std_types_serializers.hpp

        common/networking/tcp_bytestream.cpp common/networking/tcp_bytestream.hpp
        common/networking/udp_bytestream.cpp common/networking/udp_bytestream.hpp

        client/client_game_state.cpp client/client_game_state.hpp

        server/server_options.cpp server/server_options.hpp
        server/server_manager.cpp server/server_manager.hpp
        server/server_game_state.cpp server/server_game_state.hpp

        common/types.hpp)

TARGET_INCLUDE_DIRECTORIES(robots-server PRIVATE ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(robots-server LINK_PUBLIC ${Boost_LIBRARIES})