#include "gamelib/imgui/inputs.hpp"
#include "gamelib/properties/PropertyContainer.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/components/RenderComponent.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/rendering/Camera.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace gamelib
{
    bool inputBitflags(const char* label, unsigned int* flags, int num, const char* const* names)
    {
        bool changed = false;
        if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (int i = 0; i < num; ++i)
                if (names[i])
                    if (ImGui::CheckboxFlags(names[i], flags, 1 << i))
                        changed = true;
            ImGui::TreePop();
        }
        return changed;
    }

    bool inputRenderOptions(const char* label, RenderOptions* options)
    {
        bool changed = false;
        if (ImGui::TreeNodeEx(label, ImGuiTreeNodeFlags_DefaultOpen))
        {
            // TODO: blendmode, shader
            changed |= inputBitflags("Render flags", &options->flags, num_renderflags, str_renderflags);
            changed |= ImGui::InputFloat("Parallax", &options->parallax, 0.01, 0.1, 3);
            ImGui::TreePop();
        }
        return changed;
    }

    bool inputLayer(const char* label, LayerHandle* handle, const RenderSystem* sys)
    {
        sys = sys ? sys : RenderSystem::getActive();
        if (!sys)
        {
            ImGui::Text("Error: No RenderSystem available");
            return false;
        }

        // later needed for itemgetter
        struct {
            std::vector<LayerHandle> cache;
            const RenderSystem* sys;
        } context;

        context.sys = sys;
        auto& cache = context.cache;
        cache.emplace_back();
        int i = 1;
        int selected = 0;


        for (auto it = sys->beginLayers(), end = sys->endLayers(); it != end; ++it)
        {
            if (handle && it.handle() == *handle)
                selected = i;
            cache.push_back(it.handle());
            ++i;
        }

        auto itemgetter = [](void* data, int index, const char** name) {
            auto& context_ = *static_cast<decltype(context)*>(data);
            auto handle = context_.cache[index];
            if (handle.isNull())
                *name = "default";
            else
                *name = context_.sys->getLayer(handle)->name.c_str();
            return true;
        };

        if (ImGui::Combo(label, &selected, itemgetter, &context, cache.size()))
        {
            *handle = cache[selected];
            return true;
        }

        return false;
    }

    bool inputTransform(Transformable& trans)
    {
        if (!(trans.isMovable() || trans.isScalable() || trans.isRotatable()))
        {
            ImGui::Text("No transformations supported");
            return false;
        }

        TransformData data;
        bool changed = false;

        const ImGuiID modeid = ImGui::GetID(&trans);
        int* modeptr = ImGui::GetStateStorage()->GetIntRef(modeid, 0);

        ImGui::RadioButton("Local", modeptr, 0);
        ImGui::SameLine();
        ImGui::RadioButton("Global", modeptr, 1);

        if (*modeptr == 0)
            data = trans.getLocalTransformation();
        else
            data = trans.getTransformation();

        if (trans.isMovable())
            if (ImGui::InputFloat2("Position", &data.pos[0], 2))
                changed = true;

        if (trans.isScalable())
            if (ImGui::InputFloat2("Scale", &data.scale[0], 2))
                changed = true;

        if (trans.isRotatable())
            if (ImGui::InputFloat("Rotation", &data.angle, 1, 10, 2))
                changed = true;

        if (*modeptr == 0)
        {
            if (ImGui::InputFloat2("Origin", &data.origin[0], 2))
                changed = true;

            ImGui::SameLine();
            if (ImGui::Button("Center"))
            {
                data.origin = trans.getLocalBBox().getCenter();
                changed = true;
            }
        }

        if (changed)
        {
            if (*modeptr == 0)
                trans.setLocalTransformation(data);
            else
                trans.setTransformation(data);
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
        if (handle.serializer)
            return handle.serializer->drawGui(handle, name);
        return false;
    }


    void inputEntityProps(Entity& ent)
    {
        inputEntity(ent);

        ImGui::Separator();

        for (auto& i : ent)
        {
            Component* comp = i.ptr.get();
            ImGui::PushID(comp);
            if (ImGui::CollapsingHeader(comp->getName().c_str()))
            {
                // ImGui::Indent(10);
                inputComponent(*comp);
                // ImGui::Unindent(10);
            }
            ImGui::PopID();
        }
    }

    void inputEntity(Entity& ent)
    {
        // static constexpr const char* entityFlags[] { "Persistent (Unimplemented)", "Static (Unimplemented)" };
        ImGui::PushItemWidth(0.5 * ImGui::GetWindowContentRegionWidth());

        // ImGui::InputText("name", const_cast<char*>(ent.getName().c_str()), ent.getName().size(), ImGuiInputTextFlags_ReadOnly);
        // ImGui::NewLine();

        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            inputTransform(ent.getTransform());

        // if (ImGui::CollapsingHeader("Flags"))
        //     inputBitflags(&ent.flags, 2, entityFlags);
        ImGui::PopItemWidth();
    }

    void inputComponent(Component& comp)
    {
        ImGui::PushID(&comp);
        ImGui::PushItemWidth(0.5 * ImGui::GetWindowContentRegionWidth());
        if (comp.getTransform() && ImGui::TreeNode("Transform"))
        {
            inputTransform(*comp.getTransform());
            ImGui::TreePop();
        }
        inputProperties(comp.getProperties());
        ImGui::PopItemWidth();
        ImGui::PopID();
    }

    bool inputComponentSelect(const std::string& name, Component** ptr, const Entity& ent, const Component* self, unsigned int filter, int numfilters, const char* const* namefilters)
    {
        struct Cache
        {
            Component* ptr;
            std::string name;
        };

        std::vector<Cache> cache;
        int current = -1;

        for (auto& i : ent)
        {
            if (i.ptr.get() == self)
                continue;

            if (filter != 0 && i.ptr->getID() != filter)
                continue;

            if (numfilters > 0 && namefilters)
            {
                bool skip = true;
                for (int f = 0; f < numfilters; ++f)
                    if (i.ptr->getName() == namefilters[f])
                    {
                        skip = false;
                        break;
                    }

                if (skip)
                    continue;
            }

            if (i.ptr.get() == *ptr)
                current = cache.size();
            cache.push_back({ i.ptr.get(), generateName(i.ptr->getName(), i.id) });
        }

        auto getter = [](void* data, int index, const char** str) {
            auto& cache_ = (*static_cast<decltype(cache)*>(data));
            if (index < 0 || index >= cache_.size())
                return false;
            *str = cache_[index].name.c_str();
            return true;
        };

        if (ImGui::Combo(name.c_str(), &current, getter, &cache, cache.size()))
        {
            *ptr = cache[current].ptr;
            return true;
        }

        return false;
    }
}
