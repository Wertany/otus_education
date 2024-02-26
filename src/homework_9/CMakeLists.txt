cmake_minimum_required(VERSION 3.20)

project(libasync.so)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(libasync.so ${HOMEWORK_9})
target_link_libraries(libasync.so fmt::fmt)
target_compile_definitions(libasync.so PUBLIC LOG_TO_FILE LOG_TO_CONSOLE)
