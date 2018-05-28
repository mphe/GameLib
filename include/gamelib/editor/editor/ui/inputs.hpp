#ifndef GAMELIB_UI_INPUTS_HPP
#define GAMELIB_UI_INPUTS_HPP

#include <string>

namespace gamelib
{
    class RenderComponent;
    class Transformable;
    class PropertyHandle;
    class Entity;

    bool inputBitflags(unsigned int* flags, int num, const char* const* names);
    void inputRenderComponent(RenderComponent& ren);

    bool inputTransform(Transformable& trans);
    bool inputTransform(Transformable& trans, unsigned int transflags);

    bool inputProperty(const std::string& name, const PropertyHandle& handle);
    void inputEntity(Entity& ent);
    void inputEntityProps(Entity& ent);
}

#endif
