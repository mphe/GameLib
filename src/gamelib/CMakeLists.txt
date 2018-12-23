set(CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.4 COMPONENTS system window graphics REQUIRED)
find_package(OpenGL)
find_package(Boost COMPONENTS system filesystem REQUIRED)

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
    ${OPENGL_LIBRARIES}
    imgui
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

set(GAMELIB_SOURCES
    events/ResourceReloadEvent.cpp

    components/RenderComponent.cpp
    components/CollisionComponent.cpp
    components/UpdateComponent.cpp
    components/update/QPhysics.cpp
    components/update/QController.cpp
    components/update/AnimationComponent.cpp
    components/update/CameraTracker.cpp
    components/update/ActorComponent.cpp
    components/geometry/AABB.cpp
    components/geometry/Polygon.cpp
    components/geometry/PixelCollision.cpp
    components/rendering/PolygonShape.cpp
    components/rendering/SpriteComponent.cpp
    components/editor/BrushComponent.cpp
    components/editor/LineBrushComponent.cpp

    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
    utils/string.cpp
    utils/aspectratio.cpp

    json/json-file.cpp
    json/JsonSerializer.cpp
    json/json-utils.cpp
    json/json-vector.cpp
    json/json-transformable.cpp

    properties/Property.cpp
    properties/PropertyHandle.cpp
    properties/basictypes.cpp
    properties/PropComponent.cpp
    properties/PropDummy.cpp

    imgui/inputs.cpp
    imgui/buttons.cpp
    imgui/resources.cpp
    imgui/FileDialog.cpp

    core/input/InputSystem.cpp
    core/Game.cpp
    core/event/EventManager.cpp
    core/sprite/AnimatedSprite.cpp
    core/sprite/SpriteData.cpp
    core/geometry/CollisionSystem.cpp
    core/geometry/Transformable.cpp
    core/geometry/GroupTransform.cpp
    core/geometry/MatrixPolygon.cpp
    core/movement/Acceleration.cpp
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
    core/rendering/Camera.cpp
    core/rendering/FreeCam.cpp
    core/ecs/Entity.cpp
    core/ecs/EntityManager.cpp
    core/ecs/EntityFactory.cpp
    core/ecs/serialization.cpp
    core/ecs/Component.cpp
    core/update/UpdateSystem.cpp

    Engine.cpp
    export.cpp
)

source_group(gamelib FILES ${GAMELIB_SOURCES})

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/extlib/cppmath/include
    ${PROJECT_SOURCE_DIR}/extlib/jsoncpp/include
)

add_library(${PROJECT_NAME} ${GAMELIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXT_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)


if (GAMELIB_BUILD_TOOLS)
    source_group(tools FILES main/checkentcfg.cpp)
    gen_binary(checkentcfg main/checkentcfg.cpp)
endif()


if(GAMELIB_BUILD_EDITOR)
    set(EDITOR_SOURCES
        main/editormain.cpp
        editor/Editor.cpp
        editor/EditorShared.cpp
        editor/tools/SelectTool.cpp
        editor/tools/SpriteTool.cpp
        editor/tools/BrushTool.cpp
        editor/tools/VertexTool.cpp
        editor/tools/EntityTool.cpp
        editor/tools/ToolUtils.cpp
        editor/ui/LayerUI.cpp
        editor/ui/CameraUI.cpp
        editor/ui/Grid.cpp
        editor/ui/JsonView.cpp
        editor/ui/ResourceViewer.cpp
        editor/ui/EntityList.cpp
        editor/ui/GlobalGameConfig.cpp
        editor/ui/Overlay.cpp
        editor/events/OnSelect.cpp
    )

    source_group(editor FILES ${EDITOR_SOURCES} main/editormain.cpp)

    set(EXT_EDITOR_LIBRARIES
    )

    include_directories(
        ${PROJECT_SOURCE_DIR}/extlib/imgui
        ${PROJECT_SOURCE_DIR}/extlib/imgui-sfml
        ${PROJECT_SOURCE_DIR}/extlib/imguifilesystem
    )

    gen_binary(editor ${EDITOR_SOURCES})
    target_link_libraries(editor ${EXT_EDITOR_LIBRARIES})

    add_custom_target(run
        COMMAND editor assets/testmap.json
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()
