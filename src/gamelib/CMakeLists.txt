set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)

set(EXT_LIBRARIES
    ${SFML_LIBRARIES}
    cppmath
    jsoncpp
)

set(GAMELIB_SOURCES
    events/ResourceReloadEvent.cpp

    components/update/QPhysics.cpp
    components/update/QController.cpp
    components/geometry/AABB.cpp
    components/geometry/Polygon.cpp
    components/geometry/PixelCollision.cpp
    components/rendering/PolygonShape.cpp

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

    Engine.cpp
    export.cpp

    # legacy/Scene/Scene.cpp
    # legacy/Scene/TilemapAdapter.cpp
    # legacy/Scene/VimCam.cpp
    # legacy/component/Entity.cpp
    # legacy/component/subsystem/UpdateSystem.cpp
    # legacy/component/UpdateComponent.cpp
    # legacy/TileWalker.cpp
)

source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${SFML_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

if(GAMELIB_BUILD_EDITOR)
    find_package(OpenGL)

    set(EDITOR_SOURCES
        editor/components/BrushComponent.cpp
        editor/editor/Editor.cpp
        editor/editor/EditorShared.cpp
        editor/editor/tools/SelectTool.cpp
        editor/editor/tools/BrushTool.cpp
        editor/editor/tools/VertexTool.cpp
        editor/editor/tools/EntityTool.cpp
        editor/editor/tools/ToolUtils.cpp
        editor/editor/ui/resources.cpp
        editor/editor/ui/props.cpp
        editor/editor/ui/LayerUI.cpp
        editor/editor/ui/Grid.cpp
        editor/editor/ui/JsonView.cpp
        editor/events/OnSelect.cpp
    )

    set(EXT_LIBRARIES
        ${EXT_LIBRARIES}
        ${OPENGL_LIBRARIES}
        imgui
    )

    set(GAMELIB_SOURCES
        ${GAMELIB_SOURCES}
        ${EDITOR_SOURCES}
    )

    source_group(editor FILES ${EDITOR_SOURCES} editor/main.cpp)

    include_directories(
        ${PROJECT_SOURCE_DIR}/extlib/imgui
        ${PROJECT_SOURCE_DIR}/extlib/imgui-sfml
        ${PROJECT_SOURCE_DIR}/extlib/imguifilesystem
        ${PROJECT_SOURCE_DIR}/include/gamelib
    )

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

    add_executable(editor editor/main.cpp)
    target_link_libraries(editor ${PROJECT_NAME})
    set_property(TARGET editor PROPERTY CXX_STANDARD 11)
endif()

add_library(${PROJECT_NAME} ${GAMELIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
