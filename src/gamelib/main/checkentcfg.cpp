#include <iostream>
#include <cstring>
#include "gamelib/core/ecs/serialization.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout<<"Checks and optionally fixes entity config files."<<endl;
        cout<<"Prints the normalized config to stdout or writes it to outfile, if specified."<<endl;
        cout<<"If -s is passed, the config won't be printed to stdout."<<endl;
        cout<<"Usage: checkentcfg [-s] <config file> [outfile]"<<endl;
        return 0;
    }

    int starti = 1;
    bool silent = false;

    if (strcmp(argv[starti], "-s") == 0)
    {
        silent = true;
        ++starti;
    }

    if (argc >= starti + 1)
    {
        Json::Value cfg;
        if (!gamelib::loadJsonFromFile(argv[starti], cfg))
            return 1;

        ++starti;

        Json::Value normalized;
        bool good = gamelib::normalizeConfig(cfg, &normalized);

        if (!silent)
            cout<<normalized.toStyledString()<<endl;

        if (argc >= starti + 1 && strlen(argv[starti]) > 0)
            gamelib::writeJsonToFile(argv[starti], normalized);

        return !good;
    }
    else
    {
        LOG_ERROR("No file given");
        return 1;
    }
}
