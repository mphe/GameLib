set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

macro(add_example EXNAME SOURCE)
    add_executable(${EXNAME} ${SOURCE} ${ARGN})
    target_link_libraries(${EXNAME} ${PROJECT_NAME})
    set_property(TARGET ${EXNAME} PROPERTY CXX_STANDARD 11)
endmacro()

# add_example(scene_tilemap scene_tilemap/main.cpp scene_tilemap/MainState.hpp scene_tilemap/MainState.cpp)
add_example(tilewalker tilewalker/main.cpp tilewalker/MainState.hpp tilewalker/MainState.cpp)
add_example(platformer platformer/main.cpp platformer/MainState.hpp platformer/MainState.cpp platformer/Player.cpp)
