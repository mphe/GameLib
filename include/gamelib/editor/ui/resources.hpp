#ifndef EDITOR_RESOURCE_MANAGER_HPP
#define EDITOR_RESOURCE_MANAGER_HPP

#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include <SFML/Graphics.hpp>

namespace gamelib
{
    constexpr int defaultThumbsize = 72;

    typedef bool(*ThumbnailFunction)(BaseResourceHandle, sf::Sprite*);
    typedef void(*PreviewFunction)(BaseResourceHandle);

    void getResourceMeta(ID id, const char** name, ThumbnailFunction* thumbgetter, PreviewFunction* preview);

    bool getThumbnailSprite(BaseResourceHandle res, sf::Sprite* sprite);
    bool getThumbnailTexture(BaseResourceHandle res, sf::Sprite* sprite);
    void previewTexture(BaseResourceHandle res);
    void previewSprite(BaseResourceHandle res);
    void previewJson(BaseResourceHandle res);

    void previewTexture(BaseResourceHandle res, int imgsize);
    void previewSprite(BaseResourceHandle res, int imgsize);

    bool inputResource(BaseResourceHandle* res, ID id, bool preview = false, int thumbsize = defaultThumbsize);
    bool inputResource(BaseResourceHandle* res, ID id, const char* name, ThumbnailFunction thumbgetter, PreviewFunction preview, int thumbsize = defaultThumbsize);

    template <typename T>
    bool inputResource(ResourceHandle<T>* res, bool preview = false, int thumbsize = defaultThumbsize)
    {
        auto handle = res->asBase();
        if (inputResource(&handle, T::id, preview, thumbsize))
        {
            *res = handle;
            return true;
        }

        return false;
    }

    bool drawResourceSelection(BaseResourceHandle* handle, ID id, bool preview = false, int thumbsize = defaultThumbsize);
    bool drawResourceSelection(BaseResourceHandle* handle, ID id, ThumbnailFunction thumbgetter, PreviewFunction preview, int size);
    bool drawResourceSelectionList(BaseResourceHandle* handle, ID id);
    bool drawResourceSelectionGrid(BaseResourceHandle* handle, ID id, ThumbnailFunction thumbgetter, int size);
}

#endif
