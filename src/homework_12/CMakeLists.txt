cmake_minimum_required(VERSION 3.20)

project(mapreduce)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOMEWORK_12 main.cpp
                mapreduce.cpp)

add_executable(mapreduce ${HOMEWORK_12})
target_link_libraries(mapreduce fmt::fmt)