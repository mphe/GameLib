#ifndef GAMELIB_PREDEF_RESOURCES_HPP
#define GAMELIB_PREDEF_RESOURCES_HPP

namespace gamelib
{
    class ResourceManager;

    void registerPredefLoaders();
    void registerPredefLoaders(ResourceManager& resmgr);
}

#endif
