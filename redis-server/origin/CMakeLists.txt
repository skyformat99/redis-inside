INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/event)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/event)

set(SRCS
    networking.c)    
add_library (networking STATIC ${SRCS})

add_executable(server-a server.c)
target_link_libraries(server-a event)
target_link_libraries(server-a networking)
