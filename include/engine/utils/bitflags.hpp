#ifndef UTILS_BITFLAGS_HPP
#define UTILS_BITFLAGS_HPP

namespace bitflag
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
}

#endif

