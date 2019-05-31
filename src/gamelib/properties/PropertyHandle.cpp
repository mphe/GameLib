#include "gamelib/properties/PropertyHandle.hpp"

namespace gamelib
{
    PropertyHandle::PropertyHandle() :
        PropertyHandle(nullptr)
    { }

    PropertyHandle::PropertyHandle(void* var, void* data) :
        serializer(nullptr),
        hints(nullptr),
        min(0),
        max(0),
        _ptr(var),
        _setter(nullptr),
        _data(data)
    { }

    PropertyHandle::PropertyHandle(const void* var, PropSetterCallback setter, void* data) :
        PropertyHandle()
    {
        _constptr = var;
        _setter = setter;
        _data = data;
    }

    PropertyHandle::PropertyHandle(PropAccessorCallback accessor, void* data) :
        PropertyHandle()
    {
        _accessor = accessor;
        _data = data;
    }

    bool PropertyHandle::hasSetter() const
    {
        return _constptr && _setter;
    }

    bool PropertyHandle::hasAccessor() const
    {
        return !_constptr && _accessor;
    }

    void* PropertyHandle::getMutableOrNull() const
    {
        return _ptr && !_setter ? _ptr : nullptr;
    }

    const void* PropertyHandle::get() const
    {
        if (hasAccessor())
            return _accessor(nullptr, _data);
        return _constptr;
    }

    void* PropertyHandle::getData() const
    {
        return _data;
    }
}
