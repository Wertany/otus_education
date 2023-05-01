cmake_minimum_required(VERSION 3.20)

set(PATCH_VERSION "7" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(otus_education VERSION ${PROJECT_VESRION})

configure_file(version.h.in version.h)

option(BUILD_TESTS "Builds tests when option is ON" ON)

#Собираем пользовательсие файлы из включенных в основную папку и подпапки
file(GLOB MAIN_SOURCES LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories("src")
include_directories("${CMAKE_BINARY_DIR}")

#Собираем в один лист
set(SOURCES_LIST ${MAIN_SOURCES})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(otus_education ${SOURCES_LIST})
add_library(otus_education_lib ${SOURCES_LIST})

# Если надо собирать тесты, собираем исходники в библиотеку и включаем директорию тестов
if (BUILD_TESTS)
    #Удаляем файл с main() для повторного использования списка исходников
    list(REMOVE_ITEM SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_subdirectory(tests)
endif ()

install(TARGETS otus_education RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

if (MSVC)
    target_compile_options(otus_education PRIVATE
        /W4
    )
    target_compile_options(otus_education_lib PRIVATE
        /W4
    )
    target_compile_options(otus_education_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(otus_education PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(otus_education_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(otus_education_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()