cmake_minimum_required(VERSION 3.18)
project(robots-client)

set(Boost_INCLUDE_DIR /usr/include/boost)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(robots-client client.cpp
        debug_functions.cpp debug_functions.hpp

        models/client_message.cpp models/client_message.hpp
        models/gui_message.cpp models/gui_message.hpp
        models/server_message.cpp models/server_message.hpp
        models/event.cpp models/event.hpp
        models/misc.cpp models/misc.hpp

        serializers/std_types_serializers.cpp serializers/std_types_serializers.hpp

        management/client_options.cpp management/client_options.hpp
        management/client_manager.cpp management/client_manager.hpp
        management/client_game_state.cpp management/client_game_state.hpp

        networking/tcp_bytestream.cpp networking/tcp_bytestream.hpp
        networking/udp_bytestream.cpp networking/udp_bytestream.hpp

        types.hpp)

TARGET_INCLUDE_DIRECTORIES(robots-client PRIVATE ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(robots-client LINK_PUBLIC ${Boost_LIBRARIES})
