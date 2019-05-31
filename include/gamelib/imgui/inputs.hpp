#ifndef GAMELIB_IMGUI_INPUTS_HPP
#define GAMELIB_IMGUI_INPUTS_HPP

#include <string>
#include "gamelib/core/rendering/RenderStructs.hpp"

namespace gamelib
{
    class Camera;
    class Transformable;
    class PropertyHandle;
    class PropertyContainer;
    class Entity;
    class Component;
    class RenderSystem;

    bool inputBitflags(const char* label, unsigned int* flags, int num, const char* const* names);

    bool inputRenderOptions(const char* label, RenderOptions* options);
    bool inputLayer(const char* label, LayerHandle* handle, const RenderSystem* sys);
    bool inputCamera(Camera& cam);

    bool inputTransform(Transformable& trans);

    bool inputProperties(const PropertyContainer& props);
    bool inputProperty(const std::string& name, const PropertyHandle& handle);
    void inputEntityProps(Entity& ent);
    void inputComponent(Component& comp);

    bool inputComponentSelect(const std::string& name, Component** ptr, const Entity& ent, const Component* self, unsigned int filter = 0, int numfilters = 0, const char* const* namefilters = nullptr);
}

#endif
