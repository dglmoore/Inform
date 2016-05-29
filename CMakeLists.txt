cmake_minimum_required(VERSION 2.8)
project(inform C)

if (UNIX)
    add_definitions("-Wall -Wextra -Werror -Wno-unused-parameter -std=gnu1x")
endif()

include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
add_library(${PROJECT_NAME}_STATIC ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME}_STATIC PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION lib)
    install(TARGETS ${PROJECT_NAME}_STATIC EXPORT ${PROJECT_NAME} DESTINATION lib)
    install(DIRECTORY include/ DESTINATION include)
endif()

if (NOT MSVC)
    enable_testing()
    add_subdirectory(test)
endif()
