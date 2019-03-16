#ifndef GAMELIB_TYPE_TRAITS_HPP
#define GAMELIB_TYPE_TRAITS_HPP

// unsafe for non-array types or types overloading operator[], but good enough for now
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*(array)))

namespace gamelib
{
}

#endif
