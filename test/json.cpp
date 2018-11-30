#include <iostream>
#include <string>
#include <functional>
#include <cassert>
#include <algorithm>
#include "gamelib/json/json-utils.hpp"

using namespace std;
using namespace gamelib;

Json::Value createjson(const char* str);

int main()
{
    auto a = createjson("{\
            'a': '0',\
            'dict': {\
                'dict2': {\
                    'b': 1\
                },\
                'dict3': {\
                    'c': 2\
                },\
                'dict4': {\
                    'dict5': {\
                        'd': 3,\
                        'e': 4\
                    }\
                },\
                'f': 5,\
                'g': 6\
            }\
        }");

    auto b = createjson("{\
            'a': '42',\
            'dict': {\
                'dict2': {\
                    'b': 11\
                },\
                'dict4': {\
                    'dict5': {\
                        'd': 33\
                    }\
                },\
                'g': 66\
            },\
            'dict6': { 'foo': 99 }\
        }");

    auto correct = createjson("{\
            'a': '42',\
            'dict': {\
                'dict2': {\
                    'b': 11\
                },\
                'dict3': {\
                    'c': 2\
                },\
                'dict4': {\
                    'dict5': {\
                        'd': 33,\
                        'e': 4\
                    }\
                },\
                'f': 5,\
                'g': 66\
            },\
            'dict6': { 'foo': 99 }\
        }");

    mergeJson(b, &a);

    bool right = a.compare(correct) == 0;
    if (!right)
    {
        cout<<"a:\n"<<a.toStyledString()<<endl;
        cout<<"correct:\n"<<correct.toStyledString()<<endl;
    }

    assert(right && "Incorrect merge");
    assert(a.toStyledString() == correct.toStyledString() && "Incorrect merge");

    Json::Value tmp;
    assert(diffJson(a, correct, &tmp) == false && "Incorrect merge or diff");


    // TODO: add diffJson tests here
    // It works, but it wouldn't hurt to have a test

    return 0;
}


// TODO: move to lib?
Json::Value createjson(const char* str_)
{
    string str = str_;
    std::replace(str.begin(), str.end(), '\'', '"');
    Json::Value node;
    auto reader = Json::Reader();
    bool result = reader.parse(str, node);
    if (!result)
        cout<<reader.getFormattedErrorMessages()<<endl;
    assert(result && "Failed to parse");
    return node;
}
