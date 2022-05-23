cmake_minimum_required(VERSION 3.18)
project(robots-client)

set(Boost_INCLUDE_DIR /usr/include/boost)
set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_executable(robots-client main.cpp)

TARGET_INCLUDE_DIRECTORIES(robots-client PRIVATE ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(robots-client LINK_PUBLIC ${Boost_LIBRARIES})
