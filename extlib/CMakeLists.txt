add_subdirectory(cppmath)
add_subdirectory(jsoncpp)

# setup imgui-sfml
set(IMGUI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
if (EXISTS "${IMGUI_ROOT}/imconfig.h.original")
    # NOT EXISTS doesn't seem to work
else()
    # file(COPY "${IMGUI_ROOT}/imconfig.h" DESTINATION "${IMGUI_ROOT}/imconfig.h.original")
    configure_file("${IMGUI_ROOT}/imconfig.h" "${IMGUI_ROOT}/imconfig.h.original" COPYONLY)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml/imconfig-SFML.h" IMGUI_SFML_CONFIG)
    file(APPEND "${IMGUI_ROOT}/imconfig.h" "${IMGUI_SFML_CONFIG}")
endif()

include_directories(imgui imgui-sfml imguifilesystem)
add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui-sfml/imgui-SFML.cpp
    imguifilesystem/imguifilesystem.cpp)
target_link_libraries(imgui sfml-graphics sfml-window sfml-system)
