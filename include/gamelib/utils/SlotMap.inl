#ifndef GAMELIB_SLOTMAP_INL
#define GAMELIB_SLOTMAP_INL

#include "SlotMap.hpp"
#include <cassert>

// TODO: Consider reverting back to the implementation where empty slots are
//       stored in a separate vector and use the version tag to mark a slot
//       as unused (-1).

namespace gamelib
{
    template <typename T, typename IT, typename VT>
    SlotMap<T, IT, VT>::SlotMap() :
        _firstempty(-1)
    { }

    template <typename T, typename IT, typename VT>
    SlotMap<T, IT, VT>::SlotMap(IT size) :
        _firstempty(-1)
    {
        _data.reserve(size);
    }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::Handle SlotMap<T, IT, VT>::acquire()
    {
        if (_firstempty == (IT)-1)
        {
            _data.emplace_back();
            return Handle(_data.size() - 1, 0);
        }
        else
        {
            auto i = _firstempty;
            if (i == _data[i].nextempty)
                _firstempty = -1;
            else
                _firstempty = _data[i].nextempty;

            _data[i].nextempty = -1;
            return Handle(i, _data[i].version);
        }
    }

    template <typename T, typename IT, typename VT>
    void SlotMap<T, IT, VT>::destroy(Handle key)
    {
        if (isValid(key))
        {
            if (_firstempty == (IT)-1)
                _data[key.index].nextempty = key.index;
            else
                _data[key.index].nextempty = _firstempty;

            _firstempty = key.index;
            ++_data[key.index].version;
            if (std::is_class<T>::value)
                _data[key.index].data = T();
        }
    }

    template <typename T, typename IT, typename VT>
    bool SlotMap<T, IT, VT>::isValid(Handle key) const
    {
        return key.index < _data.size() &&
            key.version == _data[key.index].version &&
            _data[key.index].nextempty == (IT)-1;
    }

    template <typename T, typename IT, typename VT>
    void SlotMap<T, IT, VT>::clear()
    {
        _data.clear();
        _firstempty = -1;
    }

    // template <typename T, typename IT, typename VT>
    // unsigned int SlotMap<T, IT, VT>::size() const
    // {
    //     throw "Not implemented";
    // }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::iterator SlotMap<T, IT, VT>::begin()
    {
        if (_data.empty() || _data[0].nextempty != (IT)-1)
            return ++iterator(_data, 0);
        return iterator(_data, 0);
    }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::const_iterator SlotMap<T, IT, VT>::begin() const
    {
        if (_data.empty() || _data[0].nextempty != (IT)-1)
            return ++const_iterator(_data, 0);
        return const_iterator(_data, 0);
    }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::iterator SlotMap<T, IT, VT>::end()
    {
        return iterator(_data, -1);
    }

    template <typename T, typename IT, typename VT>
    typename SlotMap<T, IT, VT>::const_iterator SlotMap<T, IT, VT>::end() const
    {
        return const_iterator(_data, -1);
    }

    template <typename T, typename IT, typename VT>
    const T& SlotMap<T, IT, VT>::operator[](Handle key) const
    {
        assert(isValid(key) && "Key is not valid (anymore)");
        return _data[key.index].data;
    }

    template <typename T, typename IT, typename VT>
    T& SlotMap<T, IT, VT>::operator[](Handle key)
    {
        assert(isValid(key) && "Key is not valid (anymore)");
        return _data[key.index].data;
    }


    // Iterator

    template <typename T, typename IndexType, typename ValueType>
    SlotMapIterator<T, IndexType, ValueType>::SlotMapIterator() :
        _index(-1),
        _vec(nullptr)
    { };

    template <typename T, typename IndexType, typename ValueType>
    SlotMapIterator<T, IndexType, ValueType>::SlotMapIterator(T& vec, IndexType i) :
        _index(i),
        _vec(&vec)
    { };

    template <typename T, typename IndexType, typename ValueType>
    SlotMapIterator<T, IndexType, ValueType>& SlotMapIterator<T, IndexType, ValueType>::operator++()
    {
        do
            ++_index;
        while (_index < _vec->size() && (*_vec)[_index].nextempty != (IndexType)-1);

        if (_index >= _vec->size())
            _index = -1;

        return *this;
    }

    template <typename T, typename IndexType, typename ValueType>
    SlotMapIterator<T, IndexType, ValueType> SlotMapIterator<T, IndexType, ValueType>::operator++(int)
    {
        auto tmp = *this;
        this->operator++();
        return tmp;
    }

    template <typename T, typename IndexType, typename ValueType>
    bool SlotMapIterator<T, IndexType, ValueType>::operator==(const type& rhs) const
    {
        return _index == rhs._index && _vec == rhs._vec;
    }

    template <typename T, typename IndexType, typename ValueType>
    bool SlotMapIterator<T, IndexType, ValueType>::operator!=(const type& rhs) const
    {
        return _index != rhs._index || _vec != rhs._vec;
    }

    template <typename T, typename IndexType, typename ValueType>
    ValueType& SlotMapIterator<T, IndexType, ValueType>::operator*()
    {
        return (*_vec)[_index].data;
    }

    template <typename T, typename IndexType, typename ValueType>
    SlotMapIterator<T, IndexType, ValueType>::operator const_iterator() const
    {
        return const_iterator(_vec, _index);
    }

    template <typename T, typename IndexType, typename ValueType>
    typename SlotMapIterator<T, IndexType, ValueType>::Handle SlotMapIterator<T, IndexType, ValueType>::handle() const
    {
        return Handle(_index, (*_vec)[_index].version);
    }
}

#endif
