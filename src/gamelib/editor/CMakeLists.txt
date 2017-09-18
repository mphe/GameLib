set(CXX_STANDARD_REQUIRED ON)

find_package(OpenGL)

set(EDITOR_SOURCES
    main.cpp
    components/PolygonShape.cpp
    components/BrushComponent.cpp
    editor/Editor.cpp
    editor/EditorShared.cpp
    editor/tools/SelectTool.cpp
    editor/tools/BrushTool.cpp
    editor/tools/VertexTool.cpp
    editor/tools/EntityTool.cpp
    editor/tools/ToolUtils.cpp
    editor/ui/resources.cpp
    editor/ui/props.cpp
    editor/ui/LayerUI.cpp
    editor/ui/Grid.cpp
    editor/ui/JsonView.cpp
    events/OnSelect.cpp
)

set(EXT_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${SFML_LIBRARIES}
    imgui
    GameLib
)

source_group(editor FILES ${EDITOR_SOURCES})

include_directories(
    ${PROJECT_SOURCE_DIR}/extlib/imgui
    ${PROJECT_SOURCE_DIR}/extlib/imgui-sfml
    ${PROJECT_SOURCE_DIR}/extlib/imguifilesystem
    ${PROJECT_SOURCE_DIR}/include/gamelib
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(editor ${EDITOR_SOURCES})
target_link_libraries(editor ${EXT_LIBRARIES})
set_property(TARGET editor PROPERTY CXX_STANDARD 11)

# add_custom_target(run
#     COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/editor
#     DEPENDS editor
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )
