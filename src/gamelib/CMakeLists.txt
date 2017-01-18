set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.3 COMPONENTS system window graphics REQUIRED)

set(GAMELIB_SOURCES
    Game.cpp
    event/EventManager.cpp
    tile/StaticRenderTilemap.cpp
    Camera.cpp
    sprite/AnimatedSprite.cpp
    sprite/SpriteSet.cpp
    sprite/SpriteData.cpp
    Scene/Scene.cpp
    Scene/TilemapAdapter.cpp
    Scene/VimCam.cpp
    collision/CollisionSystem.cpp
    collision/AABBCollision.cpp
    collision/Polygon.cpp
    component/Entity.cpp
    component/subsystem/UpdateSystem.cpp
    component/UpdateComponent.cpp
    movement/Acceleration.cpp
    movement/TileWalker.cpp
    res/JsonSerializer.cpp
)

set(EXT_LIBRARIES
    ${SFML_LIBRARIES}
    cppmath
    jsoncpp
)

set(UTILS_SOURCES
    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
    utils/string.cpp
)


source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${SFML_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

add_library(${PROJECT_NAME} ${GAMELIB_SOURCES} ${UTILS_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
