set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.3 COMPONENTS system window graphics REQUIRED)

set(GAMELIB_SOURCES
    Game.cpp
    event/EventManager.cpp
    tile/TileSet.cpp
    tile/Tile.cpp
    tile/FloatingTile.cpp
    tile/tilemap/TileMap.cpp
    tile/tilemap/StaticTileMap.cpp
    tile/tilemap/FloatingTileMap.cpp
    tile/tilemap/Layer.cpp
    tile/TileScene.cpp
    Camera.cpp
    sprite/AnimatedSprite.cpp
    sprite/SpriteSet.cpp
    sprite/SpriteData.cpp
    Scene.cpp
    collision/CollisionSystem.cpp
    component/Entity.cpp
)

set(EXT_LIBRARIES
    ${SFML_LIBRARIES}
    cppmath
    jsoncpp
)

set(UTILS_SOURCES
    utils/JsonObject.cpp
    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
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
