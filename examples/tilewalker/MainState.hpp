#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include "gamelib/GameState.hpp"
#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/tile/StaticRenderTilemap.hpp"
#include "gamelib/collision/TilemapCollider.hpp"
#include "gamelib/movement/TileWalker.hpp"

namespace gamelib
{
    class Game;
}

class MainState : public gamelib::GameState
{
    public:
        MainState();

        bool init(gamelib::Game* game);
        void quit();

        void update(float elapsed);
        void render(sf::RenderTarget& target);

    private:
        gamelib::Game* _game;
        gamelib::SpriteSet _spriteset;
        gamelib::StaticRenderTilemap _map;
        gamelib::TilemapCollider<gamelib::SpriteData> _mapcol;
        gamelib::TileWalker _luke;
};

#endif
