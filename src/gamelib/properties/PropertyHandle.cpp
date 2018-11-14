#include "gamelib/properties/PropertyHandle.hpp"

namespace gamelib
{
    PropertyHandle::PropertyHandle() :
        PropertyHandle(nullptr, nullptr, nullptr)
    { }

    PropertyHandle::PropertyHandle(void* var, PropSetterCallback setter, void* self) :
        serializer(nullptr),
        hints(nullptr),
        min(0),
        max(0),
        _ptr(var),
        _setter(setter),
        _self(self)
    { }

    bool PropertyHandle::isSetter() const
    {
        return _setter != nullptr;
    }

    const void* PropertyHandle::get() const
    {
        return _ptr;
    }

    void* PropertyHandle::getMutableOrNull() const
    {
        return isSetter() ? nullptr : _ptr;
    }
}
