cmake_minimum_required(VERSION 3.0)
project(inform C)

if (UNIX)
    add_definitions("-Wall -Wextra -Werror -Wno-unused-parameter -std=gnu11")
endif()

enable_testing()
add_subdirectory(test)
