link_libraries(${PROJECT_NAME}_static)
if (UNIX)
    link_libraries(m)
endif()

add_executable(entropy entropy.c)
add_executable(mutual_information mutual_information.c)
add_executable(relative_entropy relative_entropy.c)

add_subdirectory(dist)
