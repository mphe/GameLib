#include "gamelib/legacy/Scene/TilemapAdapter.hpp"
#include "gamelib/legacy/Scene/Scene.hpp"

namespace gamelib
{
    TilemapAdapter::TilemapAdapter(const Scene& scene_, const SpriteSet& set) :
        map(set),
        scene(scene_)
    { }

    void TilemapAdapter::render(sf::RenderTarget& target)
    {
        map.render(target, scene.getCurrentCamera().getCamRect());
    }

    void TilemapAdapter::update(float elapsed)
    {
        map.update(elapsed);
    }

    StaticRenderTilemap& TilemapAdapter::operator*()
    {
        return map;
    }

    const StaticRenderTilemap& TilemapAdapter::operator*() const
    {
        return map;
    }

    StaticRenderTilemap* TilemapAdapter::operator->()
    {
        return &map;
    }

    const StaticRenderTilemap* TilemapAdapter::operator->() const
    {
        return &map;
    }

}
