cmake_minimum_required(VERSION 2.6)
project(GameLib)

option(DEBUG_LOG_RELEASE "Show debug log entries in release build" OFF)
option(DEBUG_LOG_DEBUG "Show debug log entries in debug build" ON)
option(BUILD_TESTS "Build tests" ON)

if(NOT DEBUG_LOG_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} -DNLOGDEBUG")
endif()

if(NOT DEBUG_LOG_RELEASE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNLOGDEBUG")
endif()


add_subdirectory(extlib)
add_subdirectory(src/gamelib)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
