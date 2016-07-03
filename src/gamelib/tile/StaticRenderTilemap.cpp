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
            rect.size += rect.pos;

            for (int i = 0; i < 2; ++i)
            {
                if (_virtsize[i] != infiniteRepeat)
                {
                    rect.pos[i] = std::max(rect.pos[i], 0);
                    rect.size[i] = std::min(rect.size[i], _virtsize[i] * _tsize[i]);
                }
                rect.pos[i] -= rect.pos[i] % _tsize[i] + (rect.pos[i] < 0 ? _tsize[i] : 0);
            }

            const unsigned int texw = _dataset->getSpriteSheet().getSize().x,
                               texh = _dataset->getSpriteSheet().getSize().y;

            SpriteBatch batch(&_dataset->getSpriteSheet());
            const SpriteData* tile;

            for (int y = rect.pos.y; y < rect.size.y; y += _tsize[Y])
                for (int x = rect.pos.x; x < rect.size.x; x += _tsize[X])
                    if ((tile = getTile(x / _tsize[X], y / _tsize[Y])))
                        batch.Add(sf::FloatRect(x, y, _tsize[X], _tsize[Y]),
                                tile->anidata.getRect(texw, texh));

            batch.Render(surface);
            _tilesRendered = batch.Size() / 4;
            // LOG_DEBUG(LOG_DUMP(_tilesRendered));
        }
    }
}
