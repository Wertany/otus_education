cmake_minimum_required(VERSION 3.20)

project(bayan)

set(BOOST_INCLUDE_LIBRARIES regex filesystem program_options)
set(BOOST_ENABLE_CMAKE ON)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost
              COMPONENTS
                filesystem
                regex
                program_options)

add_executable(bayan ${HOMEWORK_8})
target_link_libraries(bayan fmt::fmt 
                            Boost::filesystem
                            Boost::program_options
                            Boost::regex)

target_link_libraries(bayan Boost::filesystem
                            Boost::program_options
                            Boost::regex)