set(CXX_STANDARD_REQUIRED ON)

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
    tile/Scene.cpp
    Camera.cpp
    Sprite.cpp
    SpriteSet.cpp
)

set(EXT_LIBRARIES
    sfml-graphics
    sfml-system
    sfml-window
    cppmath
    jsoncpp
)

set(UTILS_SOURCES
    utils/JsonObject.cpp
    utils/SpriteBatch.cpp
)


source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

add_library(${PROJECT_NAME} ${GAMELIB_SOURCES} ${UTILS_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
