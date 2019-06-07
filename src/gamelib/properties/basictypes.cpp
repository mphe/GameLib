#include "gamelib/properties/basictypes.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "imgui.h"
#include <climits>

namespace gamelib
{
    PropInt propInt;
    PropBitflags propBitflags;
    PropFloat propFloat;
    PropString propString;
    PropBool propBool;
    PropVec2f propVec2f;
    PropVec2i propVec2i;
    PropColor propColor;


    bool PropInt::drawGui(const PropertyHandle& prop, const std::string& name, int* ptr) const
    {
        if (prop.hints && prop.max > 0)
            return ImGui::Combo(name.c_str(), ptr, prop.hints, prop.max);
        else
            return ImGui::InputInt(name.c_str(), ptr, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue);
    }

    bool PropFloat::drawGui(const PropertyHandle& prop, const std::string& name, float* ptr) const
    {
        return ImGui::InputFloat(name.c_str(), ptr, 1, 10, 2, ImGuiInputTextFlags_EnterReturnsTrue);
    }

    bool PropString::drawGui(const PropertyHandle& prop, const std::string& name, std::string* ptr) const
    {
        static char buf[256];
        auto size = std::min(prop.max > 0 ? (size_t)prop.max : INT_MAX, sizeof(buf) - 1);
        strncpy(buf, ptr->c_str(), std::min(size, ptr->size()));
        buf[size] = '\0';
        if (ImGui::InputText(name.c_str(), buf, size, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            ptr->assign(buf);
            return true;
        }
        return false;
    }

    bool PropBool::drawGui(const PropertyHandle& prop, const std::string& name, bool* ptr) const
    {
        return ImGui::Checkbox(name.c_str(), ptr);
    }

    bool PropBitflags::drawGui(const PropertyHandle& prop, const std::string& name, unsigned int* ptr) const
    {
        if (prop.hints && prop.max > 0)
            return inputBitflags(name.c_str(), ptr, prop.max, prop.hints);
        return false;
        // else
        //     // TODO: might not be good...
        //     return ImGui::InputInt(name.c_str(), ptr, ImGuiInputTextFlags_EnterReturnsTrue);
    }

    bool PropVec2f::drawGui(const PropertyHandle& prop, const std::string& name, math::Vec2f* ptr) const
    {
        return ImGui::InputFloat2(name.c_str(), &ptr->x, 2, ImGuiInputTextFlags_EnterReturnsTrue);
    }

    bool PropVec2i::drawGui(const PropertyHandle& prop, const std::string& name, math::Vec2i* ptr) const
    {
        return ImGui::InputInt2(name.c_str(), &ptr->x, ImGuiInputTextFlags_EnterReturnsTrue);
    }


    bool PropColor::loadFromJson(const PropertyHandle& prop, sf::Color* ptr, const Json::Value& node) const
    {
        math::Vec4f tmpvec;
        gamelib::loadFromJson(node, tmpvec);
        *ptr = sf::Color(tmpvec.r, tmpvec.g, tmpvec.b, tmpvec.a);
        return true;
    }

    void PropColor::writeToJson(const PropertyHandle& prop, const sf::Color* ptr, Json::Value& node)   const
    {
        gamelib::writeToJson(node, math::Vec4f(ptr->r, ptr->g, ptr->b, ptr->a));
    }

    bool PropColor::drawGui(const PropertyHandle& prop, const std::string& name, sf::Color* ptr) const
    {
        auto tmp = math::Vec4f(ptr->r, ptr->g, ptr->b, ptr->a) / 255.f;
        if (ImGui::ColorEdit4(name.c_str(), &tmp.r))
        {
            tmp *= 255;
            *ptr = sf::Color(tmp.r, tmp.g, tmp.b, tmp.a);
            return true;
        }
        return false;
    }
}
