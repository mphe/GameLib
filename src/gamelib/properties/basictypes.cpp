#include "gamelib/properties/basictypes.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/imgui/resources.hpp"
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
    PropResource propResource;
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
        auto str = ptr;
        auto size = std::min(prop.max > 0 ? (size_t)prop.max : INT_MAX, sizeof(buf) - 1);
        strncpy(buf, str->c_str(), size);
        buf[size] = '\0';
        if (ImGui::InputText(name.c_str(), buf, size, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            (*str).assign(buf, size);
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


    bool PropResource::loadFromJson(const PropertyHandle& prop, BaseResourceHandle* ptr, const Json::Value& node) const
    {
        *ptr = getSubsystem<ResourceManager>()->get(node.asString());
        return true;
    }

    void PropResource::writeToJson(const PropertyHandle& prop, Json::Value& node)   const
    {
        auto handle = prop.getAs<BaseResourceHandle>();
        if (handle)
            node = handle.getResource()->getPath();
    }

    bool PropResource::drawGui(const PropertyHandle& prop, const std::string& name, BaseResourceHandle* ptr) const
    {
        return inputResource(name.c_str(), ptr, prop.id);
    }


    bool PropColor::loadFromJson(const PropertyHandle& prop, sf::Color* ptr, const Json::Value& node) const
    {
        math::Vec4f tmpvec;
        gamelib::loadFromJson(node, tmpvec);
        *ptr = sf::Color(tmpvec.r, tmpvec.g, tmpvec.b, tmpvec.a);
        return true;
    }

    void PropColor::writeToJson(const PropertyHandle& prop, Json::Value& node)   const
    {
        auto& col = prop.getAs<sf::Color>();
        gamelib::writeToJson(node, math::Vec4f(col.r, col.g, col.b, col.a));
    }

    bool PropColor::drawGui(const PropertyHandle& prop, const std::string& name, sf::Color* ptr) const
    {
        auto col = *ptr;
        auto tmp = math::Vec4f(col.r, col.g, col.b, col.a) / 255.f;
        if (ImGui::ColorEdit4(name.c_str(), &tmp.r))
        {
            tmp *= 255;
            col = sf::Color(tmp.r, tmp.g, tmp.b, tmp.a);
            return true;
        }
        return false;
    }
}
