#ifndef GAMELIB_UTILS_BITFLAGS_HPP
#define GAMELIB_UTILS_BITFLAGS_HPP

#define RMFLAG(set, flags) (set) = gamelib::rmflag(set, flags)
#define ADDFLAG(set, flags) (set) = gamelib::addflag(set, flags)
#define TOGGLEFLAG(set, flags) (set) = gamelib::toggleflag(set, flags)

namespace gamelib
{
    template <typename T>
    constexpr T addflag(T set, T flags)
    {
        return set | flags;
    }

    template <typename T>
    constexpr T rmflag(T set, T flags)
    {
        return set & (~flags);
    }

    template <typename T>
    constexpr bool hasflag(T set, T flags)
    {
        return (set & flags) == flags; // works for checks with more than one flag
    }

    template <typename T>
    constexpr T toggleflag(T set, T flags)
    {
        return hasflag(set, flags) ? rmflag(set, flags) : addflag(set, flags);
    }
}

#endif

