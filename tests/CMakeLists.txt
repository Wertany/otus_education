cmake_minimum_required(VERSION 3.20)

project(otus_education_tests)

#Собираем пользовательские файлы из включенных в корневом листе исходников(рекурсивный поиск по папкам)
file(GLOB MAIN_TEST_SOURCES LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB HOMEWORK_1 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_1/*.cpp)
file(GLOB HOMEWORK_2 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_2/*.cpp)
file(GLOB HOMEWORK_3 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_3/*.cpp)
file(GLOB HOMEWORK_4 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_4/*.cpp)
file(GLOB HOMEWORK_5 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_5/*.cpp)
file(GLOB HOMEWORK_6 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_6/*.cpp)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/ip_filter.tsv 
               ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

include_directories(${CMAKE_HOME_DIRECTORY}/src)

set(TEST_SOURCES ${MAIN_TEST_SOURCES}
                 ${HOMEWORK_1}
                 ${HOMEWORK_2}
                 ${HOMEWORK_3}
                 ${HOMEWORK_4}
                 ${HOMEWORK_5})

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(otus_education_tests ${TEST_SOURCES} ${SOURCES_LIST})

target_link_libraries(otus_education_tests gtest gtest_main fmt::fmt)