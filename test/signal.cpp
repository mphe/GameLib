#include <cassert>
#include <vector>
#include <random>
#include <iostream>
#include "gamelib/utils/Signal.hpp"

using namespace std;
using namespace gamelib;

void testtrigger(const Signal& signal, int expected)
{
    int control = 0;
    signal.trigger(&control);
    assert(control == signal.size() && "Not all handlers were called");
    assert(control == expected && "Not all handlers were called");
}

int main()
{
    Signal signal;
    signal.trigger(nullptr);

    std::vector<SignalHandle> handles;
    int datacontrol = 42;

    for (int i = 0; i < 100; ++i)
        handles.push_back(signal.connect(+[](int* arg, int* data)
                    {
                        assert(*data == 42 && "wrong data");
                        *arg += 1;
                    }, &datacontrol));

    assert(signal.size() == handles.size() && "Wrong size");
    testtrigger(signal, handles.size());

    int left = handles.size();

    for (int k = 0; k < handles.size() / 2; ++k)
    {
        auto i = rand() % handles.size();

        if (handles[i])
            --left;

        if (rand() % 2 == 0)
            handles.erase(handles.begin() + i);
        else
            handles[i].unregister();

        testtrigger(signal, left);
    }

    signal.clear();
    testtrigger(signal, 0);

    handles.clear();
    handles.reserve(100);   // prevent reallocation

    // test unregister during loop
    for (int i = 0; i < 100; ++i)
    {
        SignalHandle handle;

        if (i % 3 == 0) // randomly unregister others
            handle = signal.connect(+[](void*, decltype(handles)* handles)
                    {
                        (*handles)[rand() % handles->size()].unregister();
                    }, &handles);
        else    // unregister self
            handle = signal.connect(+[](void*, SignalHandle* handle)
                    {
                        handle->unregister();
                    }, handles.data() + i);
        handles.push_back(std::move(handle));
    }

    signal.trigger(nullptr);

    // test SignalWrapper
    SignalWrapper<int> intsignal;
    auto handle = intsignal.connect(+[](int* arg, void*) { (*arg)++; }, (void*)nullptr);
    testtrigger(intsignal, 1);

    return 0;
}
