cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(otus_education VERSION ${PROJECT_VESRION})

# set(CMAKE_CXX_CLANG_TIDY clang-tidy -warnings-as-errors=*)

configure_file(version.h.in version.h)

option(BUILD_TESTS "Builds tests when option is ON" on)

#Собираем пользовательские файлы из включенных в основную папку и подпапки
file(GLOB HOMEWORK_2 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_2/*.cpp)
file(GLOB HOMEWORK_5 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_5/*.cpp)
file(GLOB HOMEWORK_6 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_6/*.cpp)
file(GLOB HOMEWORK_7 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_7/*.cpp)
file(GLOB HOMEWORK_8 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_8/*.cpp)
file(GLOB HOMEWORK_9 LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/src/homework_9/*.cpp)
add_subdirectory(src/homework_5)
add_subdirectory(src/homework_6)
add_subdirectory(src/homework_7)
add_subdirectory(src/homework_8)
add_subdirectory(src/homework_9)
add_subdirectory(src/homework_10)
add_subdirectory(src/homework_11)

#Собираем в один лист
set(SOURCES_LIST ${HOMEWORK_2}
                 ${HOMEWORK_5})

include_directories(src)
include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

# Если надо собирать тесты, собираем исходники в библиотеку и включаем директорию тестов
if (BUILD_TESTS)
    #Удаляем файл с main() для повторного использования списка исходников
    list(REMOVE_ITEM SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/homework_5/main.cpp)
    add_subdirectory(tests)
    enable_testing()
    add_test(otus_education_tests tests/otus_education_tests)
endif ()

# install(TARGETS otus_education RUNTIME DESTINATION bin)

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