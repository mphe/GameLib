#ifndef GAMELIB_PROPERTY_HANDLE_HPP
#define GAMELIB_PROPERTY_HANDLE_HPP

namespace gamelib
{
    class IPropType;

    template <typename T, typename U>
    using NicePropSetterCallback = void(*)(const T* value, U* self);

    typedef NicePropSetterCallback<void, void> PropSetterCallback;


    // Accessors are functions that provide get and set logic for this
    // property. Those properties therefore don't need to store a pointer to
    // the actual value but return it from their accessor function.
    // If the value argument is null, the function is used as get() and should
    // not set any values.
    // It should return a pointer even when in set() mode.
    template <typename T, typename U>
    using NicePropAccessorCallback = const T*(*)(const T* value, U* data);

    typedef NicePropAccessorCallback<void, void> PropAccessorCallback;


    // TODO: ConstPropertyHandle -> doesn't allow modification

    class PropertyHandle
    {
        public:
            PropertyHandle();
            PropertyHandle(void* var, void* data = nullptr);
            PropertyHandle(const void* var, PropSetterCallback setter, void* data);
            PropertyHandle(PropAccessorCallback accessor, void* data);

        public:
            auto hasSetter() const   -> bool;
            auto hasAccessor() const -> bool;

            auto get() const              -> const void*;
            auto getMutableOrNull() const -> void*;
            auto getData() const          -> void*;

            template <typename T>
            auto getAs() const -> const T&
            {
                return *static_cast<const T*>(get());
            }

            template <typename T>
            auto set(const T& val) const -> void
            {
                if (hasSetter())
                    _setter(&val, _data);
                else if (hasAccessor())
                    _accessor(&val, _data);
                else
                    *static_cast<T*>(_ptr) = val;
            }

        public:
            const IPropType* serializer;
            const char* const* hints;

            union {
                unsigned int id;    // e.g. Resource ID
                struct { int min, max; };
            };

        private:
            union {
                void* _ptr;             // Pointer for direct access
                const void* _constptr;  // Const pointer when using callbacks
            };

            union {
                PropSetterCallback _setter;  // Setter function
                PropAccessorCallback _accessor;  // Accessor function
            };

            void* _data;    // Data pointer that will be passed to callbacks
    };
}

#endif
