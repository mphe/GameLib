#include "gamelib/editor/ui/inputs.hpp"
#include "gamelib/utils/Property.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/components/editor/BrushComponent.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/ui/LayerUI.hpp"
#include "gamelib/editor/ui/resources.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include <climits>

namespace gamelib
{
    namespace detail
    {
        bool inputPropertyUI(const std::string& name, void* ptr, const PropertyHandle& handle)
        {
            static char buf[256];

            switch (handle.type)
            {
                case PropInt:
                    {
                        auto val = static_cast<int*>(ptr);
                        if (handle.hints && handle.max > 0)
                            return ImGui::Combo(name.c_str(), val, handle.hints, handle.max);
                        else
                            return ImGui::InputInt(name.c_str(), val, 1, 10, ImGuiInputTextFlags_EnterReturnsTrue);
                    }

                case PropBitflags:
                    if (handle.hints && handle.max > 0)
                        return inputBitflags(static_cast<unsigned int*>(ptr), handle.max, handle.hints);
                    else
                        // TODO: might not be good...
                        return ImGui::InputInt(name.c_str(), static_cast<int*>(ptr), ImGuiInputTextFlags_EnterReturnsTrue);

                case PropFloat:
                    return ImGui::InputFloat(name.c_str(), static_cast<float*>(ptr), 1, 10, 2, ImGuiInputTextFlags_EnterReturnsTrue);

                case PropString:
                    {
                        auto str = static_cast<std::string*>(ptr);
                        auto size = std::min(handle.max > 0 ? (size_t)handle.max : INT_MAX, sizeof(buf) - 1);
                        strncpy(buf, str->c_str(), size);
                        buf[size] = '\0';
                        if (ImGui::InputText(name.c_str(), buf, size, ImGuiInputTextFlags_EnterReturnsTrue))
                        {
                            (*str).assign(buf, size);
                            return true;
                        }
                        return false;
                    }
                case PropBool:
                    return ImGui::Checkbox(name.c_str(), static_cast<bool*>(ptr));
                case PropVec2i:
                    return ImGui::InputInt2(name.c_str(), static_cast<int*>(ptr), ImGuiInputTextFlags_EnterReturnsTrue);
                case PropVec2f:
                    return ImGui::InputFloat2(name.c_str(), static_cast<float*>(ptr), 2, ImGuiInputTextFlags_EnterReturnsTrue);
                case PropColor:
                    {
                        auto& col = *static_cast<sf::Color*>(ptr);
                        auto tmp = math::Vec4f(col.r, col.g, col.b, col.a) / 255.f;
                        if (ImGui::ColorEdit4(name.c_str(), &tmp.r))
                        {
                            tmp *= 255;
                            col = sf::Color(tmp.r, tmp.g, tmp.b, tmp.a);
                            return true;
                        }
                        return false;
                    }
                case PropResource:
                    return inputResource(static_cast<BaseResourceHandle*>(ptr), handle.id);
                case PropUnknown:
                    ImGui::Text("Unknown property");
                    break;
            }
            return false;
        }

        template <typename T>
        bool inputPropertyBuffered(const std::string& name, const PropertyHandle& handle)
        {
            T tmp = handle.getAs<T>();
            if (detail::inputPropertyUI(name, &tmp, handle))
            {
                handle.set<T>(tmp);
                return true;
            }
            return false;
        }

    }


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


    bool inputBitflags(unsigned int* flags, int num, const char* const* names)
    {
        bool changed = false;
        for (int i = 0; i < num; ++i)
            if (names[i])
                if (ImGui::CheckboxFlags(names[i], flags, 1 << i))
                    changed = true;
        return changed;
    }


    void inputSceneData(SceneData& sd)
    {
        int depth = sd.getDepth();
        float parallax = sd.getParallax();

        inputBitflags(&sd.flags, num_renderflags, str_renderflags);

        if (ImGui::InputInt("Depth", &depth, 1, 100))
            sd.setDepth(depth);

        if (ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3))
            sd.setParallax(parallax);
    }

    void inputRenderComponent(RenderComponent& ren)
    {
        inputSceneData(ren);

        Layer::Handle handle = ren.getLayer();
        if (inputLayer("Layer", &handle))
            ren.setLayer(handle);
    }

    // void inputBrushComponent(BrushComponent& brush)
    // {
    //     PolygonShape* shape = brush->getBrushShape();
    //     auto offset = shape->getTexOffset();
    //     auto width = brush->getWidth();
    //
    //     drawTextureSelect(&shape->texture);
    //
    //     if (ImGui::InputFloat2("Texture offset", &offset[0], 2) && shape)
    //         shape->setTexOffset(offset);
    //
    //     if (ent.getName() == "linebrush")
    //         if (ImGui::InputInt("Line width", &width, 1, 32))
    //             brush->setWidth(width);
    // }

    bool inputTransform(Transformable& trans)
    {
        return inputTransform(trans, trans.getSupportedOps());
    }

    bool inputTransform(Transformable& trans, unsigned int transflags)
    {
        math::Vec2f tmp;
        bool changed = false;

        if (transflags | Transformable::movable)
        {
            tmp = trans.getPosition().asVector();
            if (ImGui::InputFloat2("Position", &tmp[0], 2))
            {
                trans.setPosition(tmp.asPoint());
                changed = true;
            }
        }

        if (transflags | Transformable::scalable)
        {
            tmp = trans.getScale();
            if (ImGui::InputFloat2("Scale", &tmp[0], 2))
            {
                trans.setScale(tmp);
                changed = true;
            }
        }

        if (transflags | Transformable::rotatable)
        {
            float rot = trans.getRotation();
            if (ImGui::InputFloat("Rotation", &rot, 1, 10, 2))
            {
                trans.setRotation(rot);
                changed = true;
            }
        }

        return changed;
    }


    bool inputProperties(const PropertyContainer& props)
    {
        bool changed = false;
        for (auto it : props)
            if (inputProperty(it.first, it.second))
                changed = true;
        return changed;
    }

    bool inputProperty(const std::string& name, const PropertyHandle& handle)
    {
        if (!handle.isSetter())
            return detail::inputPropertyUI(name, handle.getMutableOrNull(), handle);

        switch (handle.type)
        {
            case PropInt:
                return detail::inputPropertyBuffered<int>(name, handle);
            case PropBitflags:
                return detail::inputPropertyBuffered<unsigned int>(name, handle);
            case PropFloat:
                return detail::inputPropertyBuffered<float>(name, handle);
            case PropString:
                return detail::inputPropertyBuffered<std::string>(name, handle);
            case PropBool:
                return detail::inputPropertyBuffered<bool>(name, handle);
            case PropVec2i:
                return detail::inputPropertyBuffered<math::Vec2i>(name, handle);
            case PropVec2f:
                return detail::inputPropertyBuffered<math::Vec2f>(name, handle);
            case PropColor:
                return detail::inputPropertyBuffered<sf::Color>(name, handle);
            case PropResource:
                return detail::inputPropertyBuffered<BaseResourceHandle>(name, handle);
            case PropUnknown:
                ImGui::Text("Unknown property");
                break;
        }
        return false;
    }


    void inputEntity(Entity& ent)
    {
        static constexpr const char* entityFlags[] { "Persistent (Unimplemented)", "Static (Unimplemented)" };

        ImGui::Columns(2, nullptr, false);

        ImGui::Text("Geometry");
        ImGui::Indent(8);
        inputTransform(ent.getTransform());
        ImGui::Unindent(8);

        ImGui::NextColumn();

        ImGui::Text("Flags");
        ImGui::Indent(8);
        inputBitflags(&ent.flags, 2, entityFlags);
        ImGui::Unindent(8);

        ImGui::Columns(1);
    }

    void inputEntityProps(Entity& ent)
    {
        ImGui::Text("%s", ent.getName().c_str());
        ImGui::NewLine();

        inputEntity(ent);

        ImGui::NewLine();

        if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // static int index = 0;
            // static constexpr const char* comps[] { "ComponentA", "ComponentB", "ComponentC" };
            // ImGui::Combo("##newcomp", &index, comps, 3);
            // ImGui::SameLine();
            // ImGui::Button("Add");

            ImGui::PushItemWidth(0.5 * ImGui::GetWindowContentRegionWidth());
            ent.foreach([](Component* comp) {
                ImGui::PushID(comp);
                if (ImGui::TreeNode(comp->getName().c_str()))
                {
                    ImGui::PushID(comp);
                    if (comp->getID() == RenderComponent::id)
                        inputRenderComponent(*static_cast<RenderComponent*>(comp));

                    inputProperties(comp->getProperties());

                    ImGui::TreePop();
                    ImGui::PopID();
                }

                ImGui::PopID();

                return true;
            });
            ImGui::PopItemWidth();
        }
    }
}
