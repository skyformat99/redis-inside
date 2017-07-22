INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/event)
LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/event)

add_executable(echo echo.c)
target_link_libraries(echo event)

add_executable(timer timer.c)
target_link_libraries(timer event)
