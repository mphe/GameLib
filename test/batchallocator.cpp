#include <random>
#include <ctime>
#include <iostream>

#define private public
#include "gamelib/utils/BatchAllocator.hpp"

using namespace std;
using namespace gamelib;

constexpr int max_size = 20;

struct TestData
{
    BatchHandle handle;
    std::vector<int> testdata;
};

void allocateRandom(BatchAllocator<int>& ba, std::vector<TestData>& handles)
{
    handles.emplace_back();
    auto& back = handles.back();
    auto& handle = back.handle;
    handle = ba.allocate(1 + rand() % max_size);

    for (size_t v = handle.index; v < handle.index + handle.size; ++v)
    {
        auto val = rand();
        *ba.get(v) = val;
        back.testdata.push_back(val);
    }
}

void printdebug(BatchAllocator<int>& ba, const std::string& title = "")
{
    using namespace std;

    if (!title.empty())
        std::cout<<title<<endl;
    cout<<"size: "<<ba._data.size()<<endl;
    cout<<"freelist: ";
    for (auto& i : ba._freelist)
        cout<<i.index<<"+"<<i.size<<", ";
    cout<<endl<<"--------------------"<<endl;
}


int main()
{
    auto seed = time(0);
    srand(seed);
    cout<<"seed: "<<seed<<endl;

    BatchAllocator<int> ba;
    std::vector<TestData> handles;

    for (size_t episode = 1; episode < 100; ++episode)
    {
        ba.clear();
        handles.clear();

        for (size_t i = 0; i < episode; ++i)
            allocateRandom(ba, handles);

        for (size_t i = 0; i < episode; ++i)
        {
            if (rand() % 2 == 0)
            {
                allocateRandom(ba, handles);
            }
            else
            {
                size_t handleindex = rand() % handles.size();
                ba.free(handles[handleindex].handle);
                handles.erase(handles.begin() + handleindex);
            }
        }

        // cout<<"Freeing remaining handles ("<<handles.size()<<")"<<endl;

        for (auto& i : handles)
        {
            assert(!ba._data.empty() && "Should not be empty already");

            auto& handle = i.handle;
            for (size_t v = 0; v < handle.size; ++v)
                assert(*ba.get(handle.index + v) == i.testdata[v] && "Data integrity failed");
            ba.free(handle);

            // cout<<"freeing handle: "<<handle.index<<"+"<<handle.size<<endl;
            // printdebug(ba);
        }

        handles.clear();

        // printdebug(ba);
        assert(ba._freelist.size() == 0 && "_freelist should be empty");
        assert(ba._data.size() == 0 && "_data should be empty");
    }

    return 0;
}
