cmake_minimum_required(VERSION 3.20)

project(bulk_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG master
)

FetchContent_MakeAvailable(asio)

set(HOMEWORK_10 async.cpp
                main.cpp
                command.cpp
                threads_observers.cpp)

add_executable(bulk_server ${HOMEWORK_10})

install(TARGETS bulk_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

target_link_libraries(bulk_server fmt::fmt)
target_include_directories(${PROJECT_NAME}
                                  PRIVATE
                                      ${CMAKE_BINARY_DIR}
                                      ${asio_SOURCE_DIR}/asio/include)