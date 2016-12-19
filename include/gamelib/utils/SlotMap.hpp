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
    template <typename T, typename IndexType = unsigned int, typename VersionType = unsigned int>
    class SlotMap
    {
        public:
            struct SlotKey
            {
                IndexType index;
                VersionType version;

                SlotKey() : index(-1), version(-1) {}
                SlotKey(IndexType i, VersionType v) : index(i), version(v) {}
            };

        public:
            SlotMap();
            SlotMap(unsigned int size);

            auto acquire()                  -> SlotKey;
            auto destroy(SlotKey key)       -> void;
            auto isValid(SlotKey key) const -> bool;
            auto clear()                    -> void;
            auto size() const               -> unsigned int;

            auto operator[](SlotKey key) const  -> const T&;
            auto operator[](SlotKey key)        -> T&;

        private:
            struct DataField
            {
                VersionType version;
                T data;

                DataField() : version(0) {}
            };

        private:
            std::vector<DataField> _data;
            std::vector<IndexType> _free;
    };

    template <typename T>
    using SlotMapShort = SlotMap<T, unsigned short, unsigned short>;
}

#include "SlotMap.inl"

#endif
