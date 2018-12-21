#ifndef GAMELIB_PROPERTY_HANDLE_HPP
#define GAMELIB_PROPERTY_HANDLE_HPP

namespace gamelib
{
    typedef void(*PropSetterCallback)(void* var, const void* value, void* self);

    template <typename T, typename U>
    using NicePropSetterCallback = void(*)(T* var, const T* value, U* self);


    class IPropType;

    // TODO: ConstPropertyHandle -> doesn't allow modification

    class PropertyHandle
    {
        public:
            PropertyHandle();
            PropertyHandle(void* var, void* data = nullptr);
            PropertyHandle(const void* var, PropSetterCallback setter, void* self);

            auto isSetter() const    -> bool;

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
                if (isSetter())
                    _setter(_ptr, &val, _self);
                else
                    *static_cast<T*>(_ptr) = val;
            }

        public:
            const IPropType* serializer;
            const char* const* hints;

            union {
                unsigned int id;    // Resource ID
                struct { int min, max; };
            };

        private:
            // Basically const_cast and not particulary nice, but makes things easier
            union {
                void* _ptr;              // Pointer to the variable
                const void* _constptr;   // Const pointer to the variable in case of setter
            };

            PropSetterCallback _setter;  // Setter function if direct access isn't possible
            void* _self;                 // Data pointer that will be passed to the setter
    };
}

#endif
