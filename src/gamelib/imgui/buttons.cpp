#include "gamelib/imgui/buttons.hpp"
#include <SFML/Window/Keyboard.hpp>

namespace gamelib
{
    bool okButton(const char* label, const ImVec2& size)
    {
        return defaultButton(label, sf::Keyboard::Return, size);
    }

    bool cancelButton(const char* label, const ImVec2& size)
    {
        return defaultButton(label, sf::Keyboard::Escape, size);
    }

    bool defaultButton(const char* label, int key, const ImVec2& size)
    {
        if (ImGui::Button(label, size))
            return true;
        if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(key))
        {
            ImGui::GetIO().WantCaptureKeyboard = true;
            return true;
        }
        return false;
    }
}
