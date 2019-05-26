#ifndef GAMELIB_RESOURCE_INL
#define GAMELIB_RESOURCE_INL

#include "Resource.hpp"

namespace gamelib
{
    template <typename T>
    template <typename U>
    ResourceHandle<U> ResourceHandle<T>::as() const
    {
        return std::static_pointer_cast<U>(_res);
    }

    template <typename T>
    ResourceHandle<BaseResource> ResourceHandle<T>::asBase() const
    {
        return as<BaseResource>();
    }

    template <typename T>
    const ResourceHandle<BaseResource>* ResourceHandle<T>::asBasePtr() const
    {
        // This looks dangerous but it's safe, because
        // * shared_ptr should only hold a pointer and reference count
        // * T is guaranteed/required to be of Resource or BaseResource
        // * Resource does not use multiple inheritance, therefore the
        //   base pointer should be identical to the derived pointer
        return reinterpret_cast<const BaseResourceHandle*>(this);
    }

    template <typename T>
    ResourceHandle<BaseResource>* ResourceHandle<T>::asBasePtr()
    {
        return reinterpret_cast<BaseResourceHandle*>(this);
    }


    template <typename T>
    T* ResourceHandle<T>::getResource() const
    {
        return _res.get();
    }

    template <typename T>
    long ResourceHandle<T>::use_count() const
    {
        return _res.use_count();
    }

    template <typename T>
    void ResourceHandle<T>::reset()
    {
        _res.reset();
    }

    template <typename T>
    ResourceHandle<T>& ResourceHandle<T>::operator=(const std::shared_ptr<T>& rhs)
    {
        _res = rhs;
        return *this;
    }

    template <typename T>
    ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle<T>& rhs)
    {
        _res = rhs._res;
        return *this;
    }

    template <typename T>
    typename ResourceHandle<T>::WrappedType* ResourceHandle<T>::get() const
    {
        if (_res && _assertBase())
            return &_res->res;
        else
            return nullptr;
    }

    template <typename T>
    typename ResourceHandle<T>::WrappedType& ResourceHandle<T>::operator*()
    {
        _assertBase();
        return _res->res;
    }

    template <typename T>
    const typename ResourceHandle<T>::WrappedType& ResourceHandle<T>::operator*() const
    {
        _assertBase();
        return _res->res;
    }

    template <typename T>
    typename ResourceHandle<T>::WrappedType* ResourceHandle<T>::operator->() const
    {
        _assertBase();
        return get();
    }

    template <typename T>
    bool ResourceHandle<T>::operator==(const ResourceHandle<T>& rhs) const
    {
        return getResource() == rhs.getResource();
    }

    template <typename T>
    bool ResourceHandle<T>::operator!=(const ResourceHandle<T>& rhs) const
    {
        return !(*this == rhs);
    }


    template <typename T>
    ResourceHandle<T>::operator bool() const
    {
        return (bool)_res;
    }

    template <typename T>
    template <typename T2>
    ResourceHandle<T>::operator ResourceHandle<T2>() const
    {
        return as<T2>();
    }
}

#endif
