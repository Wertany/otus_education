cmake_minimum_required(VERSION 3.20)

project(fashio_mnist)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HOMEWORK_13 main.cpp
                classifier.cpp
                accuracy_finder.cpp)

add_executable(fashio_mnist ${HOMEWORK_13})
target_link_libraries(fashio_mnist fmt::fmt Eigen3::Eigen)