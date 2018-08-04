set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)

macro(gen_binary BINNAME SOURCE)
    add_executable(${BINNAME} ${SOURCE} ${ARGN})
    target_link_libraries(${BINNAME} ${PROJECT_NAME})
    set_property(TARGET ${BINNAME} PROPERTY CXX_STANDARD 11)
endmacro()


set(EXT_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
    cppmath
    jsoncpp
)

set(GAMELIB_SOURCES
    events/ResourceReloadEvent.cpp

    components/update/QPhysics.cpp
    components/update/QController.cpp
    components/update/AnimationComponent.cpp
    components/update/CameraTracker.cpp
    components/geometry/AABB.cpp
    components/geometry/Polygon.cpp
    components/geometry/PixelCollision.cpp
    components/rendering/PolygonShape.cpp
    components/rendering/SpriteComponent.cpp
    components/editor/BrushComponent.cpp

    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
    utils/string.cpp
    utils/json.cpp
    utils/Property.cpp

    core/input/InputSystem.cpp
    core/Game.cpp
    core/event/EventManager.cpp
    core/Camera.cpp
    core/sprite/AnimatedSprite.cpp
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
    core/res/SpriteResource.cpp
    core/res/EntityConfigResource.cpp
    core/rendering/Scene.cpp
    core/rendering/SceneData.cpp
    core/rendering/SceneObject.cpp
    core/rendering/Layer.cpp
    core/FreeCam.cpp
    core/ecs/Entity.cpp
    core/ecs/EntityManager.cpp
    core/ecs/EntityFactory.cpp
    core/ecs/serialization.cpp
    core/ecs/Component.cpp
    core/ecs/RenderComponent.cpp
    core/ecs/CollisionComponent.cpp
    core/ecs/UpdateComponent.cpp
    core/update/UpdateSystem.cpp
    core/update/Updatable.cpp

    Engine.cpp
    export.cpp
)

source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

if(GAMELIB_BUILD_EDITOR)
    find_package(OpenGL)

    set(EDITOR_SOURCES
        editor/Editor.cpp
        editor/EditorShared.cpp
        editor/tools/SelectTool.cpp
        editor/tools/SpriteTool.cpp
        editor/tools/BrushTool.cpp
        editor/tools/VertexTool.cpp
        editor/tools/EntityTool.cpp
        editor/tools/ToolUtils.cpp
        editor/ui/resources.cpp
        editor/ui/LayerUI.cpp
        editor/ui/Grid.cpp
        editor/ui/JsonView.cpp
        editor/ui/ResourceViewer.cpp
        editor/ui/EntityList.cpp
        editor/ui/GlobalGameConfig.cpp
        editor/ui/inputs.cpp
        editor/EntityDesigner.cpp
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

    source_group(editor FILES ${EDITOR_SOURCES} main/editormain.cpp)

    include_directories(
        ${PROJECT_SOURCE_DIR}/extlib/imgui
        ${PROJECT_SOURCE_DIR}/extlib/imgui-sfml
        ${PROJECT_SOURCE_DIR}/extlib/imguifilesystem
    )

    gen_binary(editor main/editormain.cpp)

    add_custom_target(run
        COMMAND editor assets/testmap.json
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
endif()

if (GAMELIB_BUILD_TOOLS)
    source_group(tools FILES main/checkentcfg.cpp)
    gen_binary(checkentcfg main/checkentcfg.cpp)
endif()


add_library(${PROJECT_NAME} ${GAMELIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
