set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL)
find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIR})

macro(gen_binary BINNAME SOURCES)
    add_executable(${BINNAME} ${SOURCES} ${ARGN})
    target_link_libraries(${BINNAME} gamelib)
endmacro()


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

#  gamelib sources {{{
set(GAMELIB_SOURCES
    events/ResourceReloadEvent.cpp
    events/CameraEvents.cpp

    components/RenderComponent.cpp
    components/CollisionComponent.cpp
    components/UpdateComponent.cpp
    components/update/QPhysics.cpp
    components/update/QController.cpp
    components/update/AnimationComponent.cpp
    components/update/ActorComponent.cpp
    components/geometry/AABB.cpp
    components/geometry/Polygon.cpp
    components/geometry/PixelCollision.cpp
    components/geometry/AABBMask.cpp
    components/rendering/MeshRenderer.cpp
    components/rendering/SpriteComponent.cpp
    components/editor/PolygonBrushComponent.cpp
    components/editor/LineBrushComponent.cpp
    components/CameraComponent.cpp

    utils/SpriteBatch.cpp
    utils/DebugGui.cpp
    utils/string.cpp
    utils/aspectratio.cpp
    utils/Timer.cpp
    utils/Signal.cpp
    utils/LifetimeTracker.cpp

    json/json-file.cpp
    json/JsonSerializer.cpp
    json/json-utils.cpp
    json/json-vector.cpp
    json/json-transformable.cpp
    json/json-resources.cpp
    json/json-rendering.cpp

    properties/PropertyContainer.cpp
    properties/PropertyHandle.cpp
    properties/basictypes.cpp
    properties/PropComponent.cpp
    properties/PropDummy.cpp
    properties/PropResource.cpp

    imgui/inputs.cpp
    imgui/buttons.cpp
    imgui/resources.cpp
    imgui/FileDialog.cpp
    imgui/iconfont.cpp

    core/input/InputSystem.cpp
    core/Game.cpp
    core/event/EventManager.cpp
    core/event/EventHandle.cpp
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
    core/res/SoundResource.cpp
    core/rendering/Camera.cpp
    core/rendering/FreeCam.cpp
    core/rendering/RenderSystem.cpp
    core/rendering/RenderStructs.cpp
    core/rendering/CameraSystem.cpp
    core/ecs/Entity.cpp
    core/ecs/EntityManager.cpp
    core/ecs/EntityFactory.cpp
    core/ecs/serialization.cpp
    core/ecs/Component.cpp
    core/update/UpdateSystem.cpp

    Engine.cpp
    export.cpp
)
# }}} gamelib sources

source_group(gamelib FILES ${GAMELIB_SOURCES})


#  editor {{{ 
if (GAMELIB_BUILD_EDITOR)
    set(EDITOR_SOURCES
        editor/Editor.cpp
        editor/EditorShared.cpp
        editor/tools/SelectTool.cpp
        editor/tools/SpriteTool.cpp
        editor/tools/BrushTool.cpp
        editor/tools/VertexTool.cpp
        editor/tools/EntityTool.cpp
        editor/tools/ToolUtils.cpp
        editor/ui/LayerUI.cpp
        editor/ui/Grid.cpp
        editor/ui/JsonView.cpp
        editor/ui/ResourceViewer.cpp
        editor/ui/EntityList.cpp
        editor/ui/GlobalGameConfig.cpp
        editor/ui/Overlay.cpp
        editor/events/OnSelect.cpp
    )

    source_group(editor FILES ${EDITOR_SOURCES})

    set(EXT_LIBRARIES
        ${EXT_LIBRARIES}
        # external editor libraries
    )

    # Append to GAMELIB_SOURCES so when linking with the static lib, the editor code is included
    set(GAMELIB_SOURCES
        ${GAMELIB_SOURCES}
        ${EDITOR_SOURCES}
    )

    if (NOT TARGET run)
        add_custom_target(run
            # COMMAND editor assets/newtestmap.json
            COMMAND gdb -ex run --args $<TARGET_FILE:editor> assets/newtestmap.json
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL
        )
    endif()

    if (NOT TARGET gdbrun)
        add_custom_target(gdbrun
            COMMAND gdb --args $<TARGET_FILE:editor> assets/newtestmap.json
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            USES_TERMINAL
        )
    endif()
endif()
# }}} editor


# create static gamelib library
add_library(gamelib ${GAMELIB_SOURCES})
target_link_libraries(gamelib ${EXT_LIBRARIES})


if (GAMELIB_BUILD_TOOLS)
    source_group(tools FILES
        main/checkentcfg.cpp
        main/editormain.cpp
    )

    gen_binary(checkentcfg main/checkentcfg.cpp)

    if (GAMELIB_BUILD_EDITOR)
        gen_binary(editor main/editormain.cpp)
    endif()
endif()
