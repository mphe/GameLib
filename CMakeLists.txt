cmake_minimum_required(VERSION 2.6)
project(GameLib)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Options {{{ -
option(GAMELIB_DISABLE_LOGGING "Disable logging completely" OFF)
option(GAMELIB_DEBUG_LOG_RELEASE "Show debug log entries in release build" OFF)
option(GAMELIB_DEBUG_LOG_DEBUG "Show debug log entries in debug build" ON)
option(GAMELIB_BUILD_TESTS "Build tests" OFF)
option(GAMELIB_BUILD_EXAMPLES "Build examples" OFF)
option(GAMELIB_USE_CCACHE "Use ccache if available" ON)
set(GAMELIB_SFML_ROOT "" CACHE PATH "SFML library directory (optional)")

# All binarys are tests, except examples, whose paths are set separately in examples/CMakeLists.txt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/test)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable warnings
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -Wno-switch -Wno-sign-compare -fdiagnostics-color=auto)
elseif (MSVC)
    # Untested
    add_definitions(/W3)
endif()

if(NOT GAMELIB_DEBUG_LOG_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNLOGDEBUG")
endif()

if(NOT GAMELIB_DEBUG_LOG_RELEASE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNLOGDEBUG")
endif()

if(GAMELIB_DISABLE_LOGGING)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDISABLE_LOGGING")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DDISABLE_LOGGING")
endif()

# Use ccache if available
if(GAMELIB_USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()

# }}} Options -


add_subdirectory(extlib)
add_subdirectory(src/gamelib)

if(GAMELIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(GAMELIB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
