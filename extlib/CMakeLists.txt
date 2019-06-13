add_subdirectory(cppmath)
add_subdirectory(jsoncpp)

# setup imgui-sfml
set(IMGUI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
if (EXISTS "${IMGUI_ROOT}/imconfig.h.original")
    # NOT EXISTS doesn't seem to work
else()
    file(RENAME "${IMGUI_ROOT}/imconfig.h" "${IMGUI_ROOT}/imconfig.h.original")
endif()
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml/imconfig-SFML.h" "${IMGUI_ROOT}/imconfig.h" COPYONLY)

include_directories(imgui imgui-sfml imguifilesystem)
add_library(imgui imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui-sfml/imgui-SFML.cpp imguifilesystem/imguifilesystem.cpp)
target_link_libraries(imgui sfml-graphics sfml-window sfml-system)
