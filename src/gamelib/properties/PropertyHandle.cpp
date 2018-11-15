#include "gamelib/properties/PropertyHandle.hpp"

namespace gamelib
{
    PropertyHandle::PropertyHandle() :
        PropertyHandle(nullptr, nullptr, nullptr)
    { }

    PropertyHandle::PropertyHandle(void* var) :
        PropertyHandle(var, nullptr, nullptr)
    { }

    PropertyHandle::PropertyHandle(const void* var, PropSetterCallback setter, void* self) :
        serializer(nullptr),
        hints(nullptr),
        min(0),
        max(0),
        _constptr(var), // also sets _ptr (yes, yes, ugly)
        _setter(setter),
        _self(self)
    { }

    bool PropertyHandle::isSetter() const
    {
        return _setter != nullptr;
    }

    const void* PropertyHandle::get() const
    {
        return _constptr;
    }

    void* PropertyHandle::getMutableOrNull() const
    {
        return isSetter() ? nullptr : _ptr;
    }
}
