cmake_minimum_required(VERSION 3.20)

project(join_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BOOST_INCLUDE_LIBRARIES system)
set(BOOST_ENABLE_CMAKE ON)

find_package(Boost
              COMPONENTS
                system)

include_directories(${Boost_INCLUDE_DIR})

set(HOMEWORK_11 main.cpp
                server.cpp
                session.cpp)

add_executable(join_server ${HOMEWORK_11})
target_link_libraries(join_server fmt::fmt Boost::system)