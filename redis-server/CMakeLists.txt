cmake_minimum_required (VERSION 2.8)

project(redis-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -Wall")

set(SUBDIRS event example server)
 
foreach(dir ${SUBDIRS})
    add_subdirectory(${dir})
endforeach()
