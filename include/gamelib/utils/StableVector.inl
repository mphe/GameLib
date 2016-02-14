#include "StableVector.hpp"
#include <algorithm>

namespace gamelib
{
    template <class T, class Alloc>
    StableVector<T, Alloc>::StableVector() :
        _remove(0)
    { }

    template <class T, class Alloc>
    void StableVector<T, Alloc>::push_back(const T& val)
    {
        _data.push_back(val);
    }

    template <class T, class Alloc>
    void StableVector<T, Alloc>::push_back(T&& val)
    {
        _data.push_back(std::forward<T>(val));
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::iterator StableVector<T, Alloc>::insert(const const_iterator& pos, const T& val)
    {
        _data.insert(_data.begin() + pos._i, val);
        return iterator(this, pos._i, pos._size);
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::iterator StableVector<T, Alloc>::insert(const const_iterator& pos, T&& val)
    {
        _data.insert(_data.begin() + pos._i, std::forward<T>(val));
        return iterator(this, pos._i, pos._size);
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::iterator StableVector<T, Alloc>::erase(const const_iterator& it)
    {
        _data[it._i].remove = true;
        ++_remove;
        return ++iterator(this, it._i, it._size);
    }

    template <class T, class Alloc>
    template <class... Args>
    void StableVector<T, Alloc>::emplace_back(Args&&... args)
    {
        _data.emplace_back(std::forward<Args>(args)...);
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::iterator StableVector<T, Alloc>::begin()
    {
        if (!_data.empty() && _data[0].remove)
            return ++iterator(this, 0, real_size());
        return iterator(this, 0, real_size());
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::const_iterator StableVector<T, Alloc>::begin() const
    {
        if (!_data.empty() && _data[0].remove)
            return ++const_iterator(this, 0, real_size());
        return const_iterator(this, 0, real_size());
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::iterator StableVector<T, Alloc>::end()
    {
        return iterator(this, real_size(), real_size());
    }

    template <class T, class Alloc>
    typename StableVector<T, Alloc>::const_iterator StableVector<T, Alloc>::end() const
    {
        return const_iterator(this, real_size(), real_size());
    }

    template <class T, class Alloc>
    void StableVector<T, Alloc>::clean()
    {
        _data.erase(std::remove_if(_data.begin(), _data.end(), [](Data& c) {
            return c.remove;
        }), _data.end());

        _remove = 0;
    }

    template <class T, class Alloc>
    void StableVector<T, Alloc>::clear()
    {
        _data.clear();
        _remove = 0;
    }

    template <class T, class Alloc>
    bool StableVector<T, Alloc>::empty() const
    {
        return !size();
    }

    template <class T, class Alloc>
    size_t StableVector<T, Alloc>::capacity() const
    {
        return _data.capacity();
    }

    template <class T, class Alloc>
    size_t StableVector<T, Alloc>::size() const
    {
        return _data.size() - _remove;
    }

    template <class T, class Alloc>
    size_t StableVector<T, Alloc>::real_size() const
    {
        return _data.size();
    }

    template <class T, class Alloc>
    template <class U>
    class StableVector<T, Alloc>::_Iterator : public std::iterator<std::forward_iterator_tag, U>
    {
        public:
            friend class StableVector<T, Alloc>;

        public:
            constexpr _Iterator() : _i(0), _size(0), _sv(NULL) {}
            constexpr _Iterator(StableVector<T, Alloc>* sv, size_t i, size_t s) :
                _i(i), _size(s), _sv(sv) {}

            _Iterator<U>& operator++()
            {
                while (++_i < _size && _sv->_data[_i].remove) {}
                return *this;
            }

            _Iterator<U> operator++(int)
            {
                _Iterator<U> tmp(*this);
                operator++();
                return tmp;
            }

            constexpr bool operator==(const _Iterator<U>& rhs) const
            {
                return _i == rhs._i && _size == rhs._size && _sv == rhs._sv;
            }

            constexpr bool operator!=(const _Iterator<U>& rhs) const
            {
                return !operator==(rhs);
            }

            U& operator*()
            {
                return _sv->_data[_i].data;
            }

            constexpr operator const_iterator() const
            {
                return const_iterator(_sv, _i, _size);
            }

        private:
            size_t _i;
            size_t _size;
            StableVector<T, Alloc>* _sv;
    };

}
