#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/GameState.hpp"
#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/Scene/Scene.hpp"
#include "gamelib/tile/StaticRenderTilemap.hpp"
#include "gamelib/event/SFMLEvent.hpp"
#include "gamelib/utils/DebugGui.hpp"
#include "Player.hpp"

namespace gamelib
{
    class Game;
    class EventManager;
}

class MainState : public gamelib::GameState
{
    public:
        MainState();

        bool init(gamelib::Game* game);
        void quit();

        void update(float fps);
        void render(sf::RenderTarget& target);

        const gamelib::Game& getGame() const;
        const gamelib::SpriteSet& getSpriteSet() const;
        const gamelib::StaticTilemap<gamelib::SpriteData>& getCollisionMap() const;

    private:
        static void _eventCallback(MainState* me, gamelib::EventPtr ev);

    private:
        gamelib::Game* _game;
        gamelib::SpriteSet _spriteset;
        gamelib::StaticRenderTilemap _map;
        Player _player;
};

#endif
