#include "gamelib/utils/StableVector.hpp"
#include <cassert>

using namespace gamelib;

int main(int argc, char *argv[])
{
    StableVector<int> sv;

    for (int i = 1; i <= 5; ++i)
        sv.push_back(i);

    assert(sv.size() == 5 && "Wrong size");

    size_t i = 0;
    // sv contains numbers from 1 - 5
    for (auto& v : sv) // Works because sv.end() is stored at the beginning of the loop
    {
        assert(++i == v && "Iterator returns the wrong value");
        sv.push_back(5 + i);
    }

    assert(i == 5 && "Iterator out of range");
    assert(sv.size() == 10 && "Wrong size");

    auto ins = std::find(sv.begin(), sv.end(), 5);
    ins = sv.insert(ins, 55);
    assert(*ins == 55 && *(++decltype(ins)(ins)) == 5 && "Value not correctly inserted");
    sv.erase(ins);

    i = 0;
    // sv contains numbers from 1 - 10
    for (StableVector<int>::const_iterator it = sv.begin(); it != sv.end(); ++it)
    {
        assert(++i == *it && "Iterator returns the wrong value");
        if (*it <= 5)
            sv.erase(it);
    }

    assert(i == 10 && "Iterator out of range");
    assert(sv.size() == 5 && "Wrong size");
    assert(sv.real_size() == 11 && "Wrong size");

    i = 5;
    // sv contains numbers from 6 - 10
    // end must be stored at the beginning to avoid it being altered midways because of push_back()
    for (auto it = sv.begin(), end = sv.end(); it != end; ++it)
    {
        assert(++i == *it && "Iterator returns the wrong value");
        sv.push_back(10 + i);
        sv.erase(it);
    }

    sv.clean();

    assert(sv.size() == 5 && "Wrong size");
    assert(sv.real_size() == 5 && "Wrong size");

    sv.clear();
    assert(sv.empty() && "Wrong size");

    return 0;
}
