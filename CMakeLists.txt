cmake_minimum_required(VERSION 2.6)
project(GameLib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Options {{{ -
option(DISABLE_LOGGING "Disable logging completely" OFF)
option(DEBUG_LOG_RELEASE "Show debug log entries in release build" OFF)
option(DEBUG_LOG_DEBUG "Show debug log entries in debug build" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)
option(USE_CCACHE "Use ccache if available" ON)
set(SFML_ROOT "" CACHE PATH "SFML library directory (optional)")

# All binarys are tests, except examples, whose paths are set separately in examples/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fdiagnostics-color=auto")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fdiagnostics-color=auto")

if(NOT DEBUG_LOG_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNLOGDEBUG")
endif()

if(NOT DEBUG_LOG_RELEASE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNLOGDEBUG")
endif()

if(DISABLE_LOGGING)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDISABLE_LOGGING")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DDISABLE_LOGGING")
endif()

# Use ccache if available
if(USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif(CCACHE_FOUND)
endif(USE_CCACHE)

# }}} Options -


add_subdirectory(extlib)
add_subdirectory(src/gamelib)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
