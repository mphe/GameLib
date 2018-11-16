#ifndef GAMELIB_IMGUI_INPUTS_HPP
#define GAMELIB_IMGUI_INPUTS_HPP

#include <string>
#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    class SceneData;
    class Transformable;
    class PropertyHandle;
    class PropertyContainer;
    class Entity;
    class Component;

    bool inputBitflags(const char* label, unsigned int* flags, int num, const char* const* names);

    bool inputLayer(const char* label, Layer::Handle* handle);
    bool inputSceneData(SceneData& sd);

    bool inputTransform(Transformable& trans);

    bool inputProperties(const PropertyContainer& props);
    bool inputProperty(const std::string& name, const PropertyHandle& handle);
    void inputEntityProps(Entity& ent);
    void inputComponent(Component& comp);
}

#endif
