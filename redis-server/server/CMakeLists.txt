INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/event)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/event)

set(SRCS
    networking.c
    adlist.c)    
add_library (networking STATIC ${SRCS})

add_executable(server server.c)
target_link_libraries(server event)
target_link_libraries(server networking)
