cmake_minimum_required(VERSION 2.8)
project(inform C)

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 0.3)
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR})

if (UNIX)
    add_definitions("-Wall -Wextra -Werror -Wno-unused-parameter -std=gnu1x")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -pg")
elseif(MSVC)
    add_definitions("/Wall")
endif()

if (APPLE)
	set(CMAKE_MACOSX_RPATH ON)
endif()

include_directories(include)
add_subdirectory(src)
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
add_library(${PROJECT_NAME}_static ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} VERSION ${${PROJECT_NAME}_VERSION})
if (MSVC)
    set_target_properties(${PROJECT_NAME}_static PROPERTIES VERSION ${${PROJECT_NAME}_VERSION})
else()
    set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME} VERSION ${${PROJECT_NAME}_VERSION})
endif()

if (UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif()

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION lib)
install(TARGETS ${PROJECT_NAME}_static EXPORT ${PROJECT_NAME} DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

enable_testing()
add_subdirectory(test)
