#include "gamelib/res/resources.hpp"
#include "gamelib/System.hpp"

namespace gamelib
{
    void registerPredefLoaders()
    {
        if (System::getActive()->getResourceManager())
            registerPredefLoaders(*System::getActive()->getResourceManager());
    }

    void registerPredefLoaders(ResourceManager& resmgr)
    {
        registerTextureLoader(resmgr);
        registerJsonLoader(resmgr);
        registerEntityConfigLoader(resmgr);
    }
}
