set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)

set(GAMELIB_SOURCES
    Game.cpp
    System.cpp
    event/EventManager.cpp
    tile/StaticRenderTilemap.cpp
    Camera.cpp
    sprite/AnimatedSprite.cpp
    sprite/SpriteSet.cpp
    sprite/SpriteData.cpp
    geometry/CollisionSystem.cpp
    geometry/AABB.cpp
    geometry/Transformable.cpp
    geometry/GroupTransform.cpp
    movement/Acceleration.cpp
    res/JsonSerializer.cpp
    res/ResourceManager.cpp
    res/predefs.cpp
    rendering/Scene.cpp
    rendering/SceneObject.cpp
    rendering/Layer.cpp
    FreeCam.cpp
    ecs/Entity.cpp
    ecs/EntityManager.cpp
    ecs/Component.cpp
    ecs/RenderComponent.cpp
    ecs/CollisionComponent.cpp
    ecs/UpdateComponent.cpp
    update/UpdateSystem.cpp
    # legacy/Scene/Scene.cpp
    # legacy/Scene/TilemapAdapter.cpp
    # legacy/Scene/VimCam.cpp
    # legacy/component/Entity.cpp
    # legacy/component/subsystem/UpdateSystem.cpp
    # legacy/component/UpdateComponent.cpp
    # legacy/TileWalker.cpp
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
