#include "gamelib/tile/StaticRenderTilemap.hpp"
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


    void StaticRenderTilemap::update(float fps)
    {
        if (!_map.empty())
            for (auto& i : _tiles)
                i.anidata.update(fps);
    }

    void StaticRenderTilemap::render(sf::RenderTarget& surface, geometry::AABB<int> rect)
    {
        if (!_map.empty())
        {
            geometry::AABB<int> mapsize(
                _virtsize[X] == infiniteRepeat ? rect.pos.x : 0,
                _virtsize[Y] == infiniteRepeat ? rect.pos.y : 0,
                _virtsize[X] == infiniteRepeat ? rect.size.x : _virtsize[X] * _tsize[X],
                _virtsize[Y] == infiniteRepeat ? rect.size.y : _virtsize[Y] * _tsize[Y]
            );

            rect.crop(mapsize);
            rect.size += rect.pos;

            for (size_t i = 0; i < 2; ++i)
            {
                if (rect.size[i] > 0)
                    rect.size[i] += _tsize[i];
                if (rect.pos[i] < 0)
                    rect.pos[i] -= _tsize[i];

                rect.size[i] /= _tsize[i];
                rect.pos[i] /= _tsize[i];
            }

            SpriteBatch batch(&_dataset->getSpriteSheet());

            const unsigned int texw = _dataset->getSpriteSheet().getSize().x,
                               texh = _dataset->getSpriteSheet().getSize().y;

            for (int x = rect.pos.x; x < rect.size.x; ++x)
            {
                for (int y = rect.pos.y; y < rect.size.y; ++y)
                {
                    const SpriteData* tile = getTile(x, y);

                    if (tile)
                    {
                        batch.Add(
                            sf::FloatRect(x * _tsize[X], y * _tsize[Y], _tsize[X], _tsize[Y]),
                            tile->anidata.getRect(texw, texh)
                        );
                    }
                }
            }
            batch.Render(surface);
            _tilesRendered = batch.Size() / 4;
            // LOG_DEBUG(LOG_DUMP(_tilesRendered));
        }
    }
}
