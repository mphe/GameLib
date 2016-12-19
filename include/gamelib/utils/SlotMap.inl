#ifndef GAMELIB_SLOTMAP_INL
#define GAMELIB_SLOTMAP_INL

#include "SlotMap.hpp"
#include <cassert>

namespace gamelib
{
    template <typename T, typename IT, typename VT>
    SlotMap<T, IT, VT>::SlotMap()
    { }

    template <typename T, typename IT, typename VT>
    SlotMap<T, IT, VT>::SlotMap(unsigned int size)
    {
        _data.reserve(size);
    }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::SlotKey SlotMap<T, IT, VT>::acquire()
    {
        if (_free.empty())
        {
            _data.emplace_back();
            return SlotKey(_data.size() - 1, 0);
        }
        auto& i = _free.back();
        _free.pop_back();
        return SlotKey(i, _data[i].version);
    }

    template <typename T, typename IT, typename VT>
    void SlotMap<T, IT, VT>::destroy(SlotKey key)
    {
        if (isValid(key))
        {
            _data[key.index].version++;
            if (std::is_class<T>::value)
                _data[key.index].data = T();
            _free.push_back(key.index);
        }
    }

    template <typename T, typename IT, typename VT>
    bool SlotMap<T, IT, VT>::isValid(SlotKey key) const
    {
        return key.index < _data.size() &&
            key.version == _data[key.index].version;
    }

    template <typename T, typename IT, typename VT>
    void SlotMap<T, IT, VT>::clear()
    {
        _data.clear();
        _free.clear();
    }

    template <typename T, typename IT, typename VT>
    unsigned int SlotMap<T, IT, VT>::size() const
    {
        return _data.size();
    }

    template <typename T, typename IT, typename VT>
    const T& SlotMap<T, IT, VT>::operator[](SlotKey key) const
    {
        assert(isValid(key) && "Key not valid (anymore)");
        return _data[key.index].data;
    }

    template <typename T, typename IT, typename VT>
    T& SlotMap<T, IT, VT>::operator[](SlotKey key)
    {
        assert(isValid(key) && "Key not valid (anymore)");
        return _data[key.index].data;
    }

}

#endif
