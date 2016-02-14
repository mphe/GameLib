#ifndef GAMELIB_STABLE_VECTOR_HPP
#define GAMELIB_STABLE_VECTOR_HPP

#include <vector>

/*
 * StableVector allows safely removing elements while iterating over them.
 * Elements are internally stored in a std::vector and are guaranteed to
 * stay in the same order as they were added.
 * New elements can only be added SAFELY using push_back() which works like
 * std::vector::push_back. 
 * Inserting at a specific position is allowed but invalidates iterators.
 * When removing an element it will not be removed instantly, but marked as
 * removed.
 * Calling clean() will go through the vector and remove every element that
 * has been marked for removal (using std::remove_if).
 * To iterate over the list, a STL friendly ForwardIterator is provided.
 * Adding elements to the container will not invalidate iterators but can
 * lead to undefined behaviour when comparing an older iterator to a new one.
 * For example:
 *     StableVector<int> sv;
 *     for (auto it = sv.begin(); it != sv.end(); ++it)
 *          sv.push_back(42);
 * This will lead to UB because the end iterator changes when adding elements.
 * To avoid these problems, the end iterator needs to be saved at the beginning:
 *     StableVector<int> sv;
 *     for (auto it = sv.begin(), end = sv.end(); it != end; ++it)
 *          sv.push_back(42);
 * This will work as expected.
 */

namespace gamelib
{
    template <class T, class Alloc = std::allocator<T>>
    class StableVector
    {
        private:
            template <class U>
            class _Iterator;

        public:
            typedef _Iterator<T> iterator;
            typedef _Iterator<const T> const_iterator;
            typedef std::ptrdiff_t difference_type;
            typedef std::size_t size_type;
            typedef T value_type;
            typedef T* pointer;
            typedef T& reference;

        public:
            StableVector();

            void push_back(const T& val);
            void push_back(T&& val);

            iterator insert(const const_iterator& pos, const T& val);
            iterator insert(const const_iterator& pos, T&& val);

            iterator erase(const const_iterator& pos);

            template <class... Args>
            void emplace_back(Args&&... args);

            iterator begin();
            const_iterator begin() const;

            iterator end();
            const_iterator end() const;

            void clean();
            void clear();

            bool empty() const;
            std::size_t capacity() const;
            std::size_t size() const;
            std::size_t real_size() const;

        private:
            struct Data
            {
                T data;
                bool remove;

                Data() : remove(false) {}
                Data(const T& d) : data(d), remove(false) {}
                Data(T&& d) : data(d), remove(false) {}

                bool operator==(const Data& d) const
                {
                    return data == d.data && remove == d.remove;
                }
            };

        private:
            std::size_t _remove;
            std::vector<Data, Alloc> _data;

    };
}

#include "StableVector.inl"

#endif
