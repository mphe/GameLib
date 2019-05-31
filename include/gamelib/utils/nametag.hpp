#ifndef GAMELIB_INTERFACE_NAMETAG_HPP
#define GAMELIB_INTERFACE_NAMETAG_HPP

#include <string>

namespace gamelib
{
    class INameTag
    {
        public:
            virtual ~INameTag() {};
            virtual auto getName() const -> const std::string& = 0;
    };


#define ASSIGN_NAMETAG(namestring)    \
    inline static auto name() -> const std::string& \
    {   \
        static const std::string _name(namestring);  \
        return _name;    \
    }   \
    \
    inline auto getName() const -> const std::string& override    \
    {   \
        return name();  \
    }


    namespace detail
    {
        class general_ {};
        class special_ : public general_ {};

        template <typename T,
                 typename RetType = decltype(T::name()),
                 typename = typename std::enable_if<std::is_same<RetType, const std::string&>::value>::type>
        constexpr bool has_static_name(special_) { return true; };

        template <typename T>
        constexpr bool has_static_name(general_) { return false; };

        template <typename T>
        constexpr bool has_static_name() { return has_static_name<T>(special_()); }


        template <typename T,
                 typename = typename std::enable_if<std::is_base_of<INameTag, T>::value>::type>
        constexpr bool has_virtual_name(special_) { return true; };

        template <typename T>
        constexpr bool has_virtual_name(general_) { return false; };

        template <typename T>
        constexpr bool has_virtual_name() { return has_virtual_name<T>(special_()); }
    }

    template <typename T>
    constexpr bool has_nametag()
    {
        return detail::has_static_name<T>() && detail::has_virtual_name<T>();
    }
}

#endif
