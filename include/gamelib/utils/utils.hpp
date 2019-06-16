#ifndef GAMELIB_PREPROCESSOR_HPP
#define GAMELIB_PREPROCESSOR_HPP

#include <type_traits>

// unsafe for non-array types or types overloading operator[], but good enough for now
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(*(array)))

// Can be used to call the const overload from a non-const overload
#define CALL_CONST_OVERLOAD(returntype, func, ...)    \
const_cast<returntype>(const_cast<   \
        const typename std::remove_pointer<decltype(this)>::type*>    \
        (this)->func(__VA_ARGS__))


namespace gamelib
{
}

#endif
