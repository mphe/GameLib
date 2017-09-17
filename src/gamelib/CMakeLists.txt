set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)

set(GAMELIB_SOURCES
    events/ResourceReloadEvent.cpp

    components/update/QPhysics.cpp
    components/update/QController.cpp
    components/geometry/AABB.cpp
    components/geometry/Polygon.cpp

    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
    utils/string.cpp
    utils/json.cpp

    core/Game.cpp
    core/event/EventManager.cpp
    core/tile/StaticRenderTilemap.cpp
    core/Camera.cpp
    core/sprite/AnimatedSprite.cpp
    core/sprite/SpriteSet.cpp
    core/sprite/SpriteData.cpp
    core/geometry/CollisionSystem.cpp
    core/geometry/Transformable.cpp
    core/geometry/GroupTransform.cpp
    core/movement/Acceleration.cpp
    core/res/JsonSerializer.cpp
    core/res/ResourceManager.cpp
    core/res/resources.cpp
    core/res/TextureResource.cpp
    core/res/JsonResource.cpp
    core/res/EntityConfigResource.cpp
    core/rendering/Scene.cpp
    core/rendering/SceneData.cpp
    core/rendering/SceneObject.cpp
    core/rendering/Layer.cpp
    core/FreeCam.cpp
    core/ecs/Entity.cpp
    core/ecs/EntityManager.cpp
    core/ecs/EntityFactory.cpp
    core/ecs/Component.cpp
    core/ecs/RenderComponent.cpp
    core/ecs/CollisionComponent.cpp
    core/ecs/UpdateComponent.cpp
    core/update/UpdateSystem.cpp
    core/update/Updatable.cpp

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

source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${SFML_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

add_library(${PROJECT_NAME} ${GAMELIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
