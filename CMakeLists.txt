cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(otus_education VERSION ${PROJECT_VESRION})

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -warnings-as-errors=*)

configure_file(version.h.in version.h)

option(BUILD_TESTS "Builds tests when option is ON" ON)

#Собираем пользовательские файлы из включенных в основную папку и подпапки
file(GLOB MAIN_SOURCES LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HOMEWORK_2 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_2/*.cpp)
file(GLOB HOMEWORK_3 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_3/*.cpp)
file(GLOB HOMEWORK_4 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_4/*.cpp)

include_directories("src")
include_directories("${CMAKE_BINARY_DIR}")

#Собираем в один лист
set(SOURCES_LIST ${MAIN_SOURCES}
                 ${HOMEWORK_2}
                 ${HOMEWORK_3}
                 ${HOMEWORK_4})


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

add_executable(otus_education ${SOURCES_LIST})
target_link_libraries(otus_education fmt::fmt)
# TODO: если раскомментриовать, то компилятор будет жаловаться на fmt. Для решения этой проблемы, 
# сделать CMakeLists для папки с исходниками
# add_library(otus_education_lib ${SOURCES_LIST})

# Если надо собирать тесты, собираем исходники в библиотеку и включаем директорию тестов
if (BUILD_TESTS)
    #Удаляем файл с main() для повторного использования списка исходников
    list(REMOVE_ITEM SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_subdirectory(tests)
    enable_testing()
    add_test(otus_education_tests tests/otus_education_tests)
endif ()

install(TARGETS otus_education RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

# if (MSVC)
#     target_compile_options(otus_education PRIVATE
#         /W4
#     )
#     target_compile_options(otus_education_lib PRIVATE
#         /W4
#     )
#     target_compile_options(otus_education_tests PRIVATE
#         /W4
#     )
# else ()
#     target_compile_options(otus_education PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )
#     target_compile_options(otus_education_lib PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )
#     target_compile_options(otus_education_tests PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )
# endif()