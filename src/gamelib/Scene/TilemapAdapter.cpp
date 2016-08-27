#include "gamelib/Scene/TilemapAdapter.hpp"
#include "gamelib/Scene/Scene.hpp"

namespace gamelib
{
    TilemapAdapter::TilemapAdapter(const Scene& scene_, const SpriteSet& set) :
        scene(scene_),
        map(set)
    { }

    void TilemapAdapter::render(sf::RenderTarget& target)
    {
        map.render(target, scene.getCurrentCamera().getCamRect());
    }

    void TilemapAdapter::update(float fps)
    {
        map.update(fps);
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
