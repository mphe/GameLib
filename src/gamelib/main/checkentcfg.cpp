#include <iostream>
#include <cstring>
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/Engine.hpp"
#include "gamelib/json/json-file.hpp"
#include "gamelib/json/json-utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout<<"Checks and optionally fixes entity config files."<<endl;
        cout<<"Prints the normalized config to stdout or writes it to outfile, if specified."<<endl;
        cout<<"Usage: checkentcfg <config file> [outfile]"<<endl;

        cout<<"Finds differences between two files"<<endl;
        cout<<"Prints the differences to stdout or writes it to outfile, if specified."<<endl;
        cout<<"Usage: checkentcfg -d <a> <b> [outfile]"<<endl;
        return 0;
    }

    if (argc < 2)
    {
        LOG_ERROR("No file given");
        return 1;
    }

    if (strcmp(argv[1], "-d") == 0)
    {
        if (argc < 4)
        {
            LOG_ERROR("Not enough arguments");
            return 1;
        }

        Json::Value a, b, diff;
        if (!gamelib::loadJsonFromFile(argv[2], a))
            return 1;
        if (!gamelib::loadJsonFromFile(argv[3], b))
            return 1;

        gamelib::diffJson(a, b, &diff);
        cout<<diff.toStyledString()<<endl;

        if (argc >= 5 && strlen(argv[4]) > 0)
            gamelib::writeJsonToFile(argv[4], diff);

        return 0;
    }
    else if (strlen(argv[1]) > 0)
    {
        Json::Value cfg;
        if (!gamelib::loadJsonFromFile(argv[1], cfg))
            return 1;

        gamelib::EntityFactory factory;
        gamelib::registerComponents(factory);

        Json::Value normalized;
        bool good = gamelib::normalizeConfig(cfg, &normalized, factory);
        cout<<normalized.toStyledString()<<endl;

        if (argc >= 3 && strlen(argv[2]) > 0)
            gamelib::writeJsonToFile(argv[2], normalized);

        return !good;
    }
    else
    {
        LOG_ERROR("No file given");
        return 1;
    }
}
