cmake_minimum_required(VERSION 3.0)
project(gamelib)

# Options {{{

# Set default build type to Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

option(GAMELIB_DISABLE_LOGGING "Disable logging completely" OFF)
option(GAMELIB_DEBUG_LOG_RELEASE "Show debug log entries in release build" OFF)
option(GAMELIB_DEBUG_LOG_DEBUG "Show debug log entries in debug build" ON)
option(GAMELIB_BUILD_TESTS "Build tests" OFF)
option(GAMELIB_BUILD_EXAMPLES "Build examples" OFF)
option(GAMELIB_BUILD_EDITOR "Build the level editor" ON)
option(GAMELIB_BUILD_TOOLS "Build engine related tools" ON)
option(GAMELIB_USE_CCACHE "Use ccache if available" ON)
set(SFML_DIR "" CACHE PATH "SFML library directory (optional)")

# }}}

# Build config {{{
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use ccache if available
if(GAMELIB_USE_CCACHE)
    find_program(CCACHE_FOUND ccache)
    if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    endif()
endif()
# }}}

# Compile flags {{{
if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(
        -Wall
        -Wextra
        -Wno-switch
        -Wno-sign-compare
        -Wno-parentheses
        -Wsuggest-override
        -Wsuggest-final-methods
        -Wsuggest-final-types
        -Werror=suggest-override
        -Werror=return-type
        -Wmaybe-uninitialized
        -fdiagnostics-color=always
        )
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

# }}}

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

set(EXT_LIBRARIES
    sfml-audio
    sfml-graphics
    sfml-window
    sfml-system
    cppmath
    jsoncpp
    imgui
    ${OPENGL_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

include_directories(SYSTEM ${Boost_INCLUDE_DIR})

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
    ${PROJECT_SOURCE_DIR}/extlib/imgui
    ${PROJECT_SOURCE_DIR}/extlib/imgui-sfml
    ${PROJECT_SOURCE_DIR}/extlib/imguifilesystem
    ${PROJECT_SOURCE_DIR}/extlib/IconFontCppHeaders
)

add_subdirectory(extlib)
add_subdirectory(src/gamelib)

if (GAMELIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (GAMELIB_BUILD_TESTS)
    # enable_testing() must be called in this file for some reason
    enable_testing()
    add_subdirectory(test)
endif()
