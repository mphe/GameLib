#ifndef GAMELIB_SLOTMAP_HPP
#define GAMELIB_SLOTMAP_HPP

#include <vector>

/*
 * A very simple Object Pool implementation.
 * It uses an std::vector internally to store objects, which allows
 * dynamically growing rather than having a fixed size.
 *
 * Calling acquire() will return a key to the next free slot. This key can
 * be used to access the object in this slot.
 * To check if a key is still valid, one can use the isValid() function.
 * When an object is not needed anymore, it can be freed using destroy().
 * destroy() will automatically check if the key is still valid. Otherwise
 * it does nothing.
 *
 * When destroying an object, its slot will be marked as free to be reused,
 * but the used memory will remain as it is.
 * If the object type (T) is a basic type, it will resist in its current
 * state until it gets overwritten by the next one, clear() is called, or
 * the map itself gets deleted.
 * If T is a class type, the object will be overwritten with a
 * default-constructed instance:
 *     obj = T();
 *
 * That means the following example is safe:
 *     SlotMap<std::unique_ptr<X>> map;
 *     auto key = map.acquire();
 *     map[key] = std::unique_ptr<X>(new X());
 *     // map[key].reset(); // Can be omitted
 *     map.destroy(key);
 *
 * The "map[key].reset()" line can be omitted, because it will be reset when
 * it gets overwritten by destroy().
 *
 * The key size can be changed by defining the type used for indices and
 * version tags during the template declaration.
 * Example:
 *     SlotMap<X, unsigned short, unsigned short> map;
 * This is useful to make the key smaller in exchange for less adressable
 * space.
 */

namespace gamelib
{
    template <typename IndexType = unsigned int, typename VersionType = unsigned int>
    struct SlotKey
    {
        typedef SlotKey<IndexType, VersionType> type;

        IndexType index;
        VersionType version;

        constexpr SlotKey() : index(-1), version(-1) {}
        constexpr SlotKey(IndexType i, VersionType v) : index(i), version(v) {}

        constexpr bool isNull() const
        {
            return index == (IndexType)-1 && version == (VersionType)-1;
        }

        constexpr bool operator==(const type& rhs) const
        {
            return index == rhs.index && version == rhs.version;
        }

        constexpr bool operator!=(const type& rhs) const
        {
            return index != rhs.index || version != rhs.version;
        }
    };

    // T is the vector to iterate over, IndexType is the same as in SlotMap.
    template <typename T, typename IndexType, typename ValueType>
    class SlotMapIterator : public std::iterator<std::forward_iterator_tag, ValueType>
    {
        public:
            typedef SlotMapIterator<T, IndexType, ValueType> type;
            typedef SlotMapIterator<const T, IndexType, const ValueType> const_iterator;
            typedef SlotKey<IndexType, decltype(T::value_type::version)> Handle;

        public:
            SlotMapIterator();
            SlotMapIterator(T& vec, IndexType i);

            auto operator++() -> type&;
            auto operator++(int) -> type;

            auto operator==(const type& rhs) const -> bool;
            auto operator!=(const type& rhs) const -> bool;

            auto operator*() -> ValueType&;

            auto handle() const -> Handle;

            operator const_iterator() const;

        private:
            IndexType _index;
            T* _vec;
    };

    template <typename T, typename IndexType = unsigned int, typename VersionType = unsigned int>
    class SlotMap
    {
        private:
            struct DataField
            {
                VersionType version;
                IndexType nextempty;
                T data;

                DataField() : version(0), nextempty(-1) {}
            };

        public:
            typedef SlotMap<T, IndexType, VersionType> selftype;
            typedef SlotKey<IndexType, VersionType> Handle;

            typedef std::ptrdiff_t difference_type;
            typedef IndexType size_type;
            typedef T value_type;
            typedef T* pointer;
            typedef T& reference;
            typedef SlotMapIterator<std::vector<DataField>, IndexType, T> iterator;
            typedef typename iterator::const_iterator const_iterator;

        public:
            SlotMap();
            SlotMap(IndexType size);

            auto acquire()                 -> Handle;
            auto destroy(Handle key)       -> void;
            auto isValid(Handle key) const -> bool;
            auto clear()                   -> void;
            // auto size() const               -> unsigned int;

            auto begin()       -> iterator;
            auto begin() const -> const_iterator;
            auto end()         -> iterator;
            auto end() const   -> const_iterator;

            auto operator[](Handle key) const -> const T&;
            auto operator[](Handle key)       -> T&;

        private:
            IndexType _firstempty;
            std::vector<DataField> _data;
    };

    template <typename T>
    using SlotMapShort = SlotMap<T, unsigned short, unsigned short>;

    using SlotKeyShort = SlotKey<unsigned short, unsigned short>;
}

#include "SlotMap.inl"

#endif
