cmake_minimum_required(VERSION 3.20)

project(bulk)

option(LOG_TO_FILE "Вывод результата обработки в файл" ON)
option(LOG_TO_CONSOLE "Вывод результата обработки в консоль" ON)

add_executable(bulk ${HOMEWORK_7})
target_link_libraries(bulk fmt::fmt)
target_compile_definitions(bulk PUBLIC LOG_TO_FILE LOG_TO_CONSOLE)