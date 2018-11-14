#ifndef GAMELIB_IMGUI_BUTTONS_HPP
#define GAMELIB_IMGUI_BUTTONS_HPP

#include "imgui.h"

namespace gamelib
{
    bool okButton(const char* label = "OK", const ImVec2& size = ImVec2(0, 0));
    bool cancelButton(const char* label = "Cancel", const ImVec2& size = ImVec2(0, 0));
    bool defaultButton(const char* label, int key, const ImVec2& size = ImVec2(0, 0));
}

#endif
