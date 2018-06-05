#include "gamelib/core/tile/StaticRenderTilemap.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "gamelib/utils/SpriteBatch.hpp"
#include "math/math.hpp"

namespace gamelib
{
    StaticRenderTilemap::StaticRenderTilemap(const SpriteSet& spriteset) :
        StaticDataSetTilemap<SpriteData, SpriteSet>(spriteset),
        _tilesRendered(0)
    { }


    void StaticRenderTilemap::update(float elapsed)
    {
        if (!_map.empty())
            for (auto& i : _tiles)
                i.anidata.update(elapsed);
    }

    void StaticRenderTilemap::render(sf::RenderTarget& surface, const math::AABB<float>& rect)
    {
        SpriteBatch batch(&_dataset->getSpriteSheet());

        const unsigned int texw = _dataset->getSpriteSheet().getSize().x,
                           texh = _dataset->getSpriteSheet().getSize().y;

        foreach(rect, [&](const SpriteData* tile, float x, float y) {
                batch.Add(sf::FloatRect(x, y, _tsize[X], _tsize[Y]),
                        tile->anidata.getRect(texw, texh));
                return false;
        });

        batch.Render(surface);
        _tilesRendered = batch.Size() / 4;
        // LOG_DEBUG(LOG_DUMP(_tilesRendered));
    }
}
