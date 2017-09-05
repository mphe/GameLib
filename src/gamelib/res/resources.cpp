#include "gamelib/res/resources.hpp"
#include "gamelib/Subsystem.hpp"

namespace gamelib
{
    void registerPredefLoaders()
    {
        auto sys = getSubsystem<ResourceManager>();
        if (sys)
            registerPredefLoaders(*sys);
    }

    void registerPredefLoaders(ResourceManager& resmgr)
    {
        registerTextureLoader(resmgr);
        registerJsonLoader(resmgr);
        registerEntityConfigLoader(resmgr);
    }
}
