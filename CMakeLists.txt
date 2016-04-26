cmake_minimum_required(VERSION 3.0)
project(inform C)

if (UNIX)
    add_definitions("-Wall -Wextra -Werror -Wno-unused-parameter -std=gnu11")
endif()

include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif()

if (NOT MSVC)
    enable_testing()
    add_subdirectory(test)
endif()
