cmake_minimum_required(VERSION 2.6)
project(TileEngine)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNLOGDEBUG")

enable_testing()

add_subdirectory(extlib)
add_subdirectory(src)
add_subdirectory(test)

