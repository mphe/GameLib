#ifndef GAMELIB_BATCHALLOCATOR_HPP
#define GAMELIB_BATCHALLOCATOR_HPP

#include <vector>
#include <cassert>

/*
 * BatchAllocator uses std::vector internally to allocate a resizable, continuous
 * chunk of memory of a certain type.
 * This allows to dynamically allocate differently sized batches of the same data
 * while maintaining cache efficiency.
 *
 * allocate() can be used to allocate a block of given size and returns a handle to it.
 * The handle consists of the index and size of the block. Size and index are needed to
 * free the block later. If the size is otherwise known, it suffices to store only the index.
 * Data can be accessed using the index and get().
 * free() requires the index and the size of the block to free it.
 *
 * Passing 0 as size in allocate() or free() is not allowed.
 * Passing out-of-bounds indices is not allowed.
 *
 * As BatchAllocator uses std::vector internally, reallocations invalidate all pointers.
 * It is therefore not advised to use pointers to access elements if a reallocate could happen.
 */

namespace gamelib
{
    struct BatchHandle
    {
        size_t index;
        size_t size;

        BatchHandle() :
            index(0),
            size(0)
        { }

        inline bool isValid() const
        {
            return size != 0;
        }
    };

    template <typename T>
    class BatchAllocator
    {
        public:
            BatchHandle allocate(size_t size);
            void        free(BatchHandle handle);
            void        clear();
            T*          get(size_t index);
            const T*    get(size_t index) const;

        private:
            std::vector<T> _data;
            std::vector<BatchHandle> _freelist;
    };
}

// Implementation
namespace gamelib
{
    template <typename T>
    void BatchAllocator<T>::clear()
    {
        _data.clear();
        _freelist.clear();
    }

    template <typename T>
    BatchHandle BatchAllocator<T>::allocate(size_t size)
    {
        assert(size > 0 && "Size should not be zero");

        BatchHandle handle;
        handle.size = size;

        if (!_freelist.empty())
        {
            size_t candidate = -1;
            for (size_t i = 0; i < _freelist.size(); ++i)
            {
                size_t chunksize = _freelist[i].size;
                if (chunksize == size)
                {
                    candidate = i;
                    break;
                }
                else if (chunksize > size)
                {
                    // TODO: find good policy
                    if (candidate == (size_t)-1)
                        candidate = i;
                }
            }

            if (candidate != (size_t)-1)
            {
                handle.index = _freelist[candidate].index;
                size_t sizediff = _freelist[candidate].size - size;

                if (sizediff == 0)
                    _freelist.erase(_freelist.begin() + candidate);
                else
                {
                    _freelist[candidate].index += size;
                    _freelist[candidate].size = sizediff;
                }

                return handle;
            }
        }

        handle.index = _data.size();
        _data.insert(_data.end(), size, T());
        return handle;
    }

    template <typename T>
    void BatchAllocator<T>::free(BatchHandle handle)
    {
        assert(handle.index < _data.size() && "Index out of bounds");
        assert(handle.size > 0 && "Size should not be zero");

        size_t end = handle.index + handle.size;

        // Simple erase if chunk is at the end
        if (end == _data.size())
        {
            auto begin = _data.begin() + handle.index;
            if (!_freelist.empty() && _freelist.back().index + _freelist.back().size == handle.index)
            {
                begin = _data.begin() + _freelist.back().index;
                _freelist.pop_back();
            }

            _data.erase(begin, _data.end());
            return;
        }

        if (_freelist.empty() || handle.index > _freelist.back().index + _freelist.back().size)
        {
            _freelist.push_back(handle);
        }
        else
        {
            // Search and merge connected chunks
            for (size_t i = 0; i < _freelist.size(); ++i)
            {
                size_t freeend = _freelist[i].index + _freelist[i].size;
                if (end == _freelist[i].index)
                {
                    _freelist[i].index = handle.index;
                    _freelist[i].size += handle.size;
                    break;
                }
                else if (freeend == handle.index)
                {
                    _freelist[i].size += handle.size;
                    // check if next block is also mergeable (current block is in between these two)
                    if (i + 1 < _freelist.size() && _freelist[i + 1].index == freeend + handle.size)
                    {
                        _freelist[i].size += _freelist[i + 1].size;
                        _freelist.erase(_freelist.begin() + i + 1);
                    }
                    break;
                }
                else if (_freelist[i].index > end)
                {
                    // if unconnected, insert new free node
                    _freelist.insert(_freelist.begin() + i, handle);
                    break;
                }
            }
        }

        // destruct/overwrite objects if neccesesary
        if (!std::is_trivially_destructible<T>::value)
            for (size_t i = handle.index; i < handle.index + handle.size; ++i)
                _data[i] = T();
    }

    template <typename T>
    const T* BatchAllocator<T>::get(size_t index) const
    {
        assert(index < _data.size() && "Index out of bounds");
        return &_data[index];
    }

    template <typename T>
    T* BatchAllocator<T>::get(size_t index)
    {
        return const_cast<T*>(const_cast<const BatchAllocator<T>*>(this)->get(index));
    }
}

#endif
