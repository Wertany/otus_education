cmake_minimum_required(VERSION 3.20)

project(otus_education)

option(BUILD_TESTS "Builds tests when option is ON" ON)

#Собираем пользовательсие файлы из включенных в основную папку и подпапки
file(GLOB MAIN_SOURCES LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories("src")

#Собираем в один лист
set(SOURCES_LIST ${MAIN_SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otus_education ${SOURCES_LIST})

# Если надо собирать тесты, собираем исходники в библиотеку и включаем директорию тестов
if (BUILD_TESTS)
    #Удаляем файл с main() для повторного использования списка исходников
    list(REMOVE_ITEM SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_subdirectory(tests)
endif ()
