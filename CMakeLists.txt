cmake_minimum_required(VERSION 3.0)
project(inform C)

if (UNIX)
    add_definitions("-Wall -Wextra -Werror -Wno-unused-parameter -std=gnu11")
endif()

include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME} m)

enable_testing()
add_subdirectory(test)
