#ifndef MAINSTATE_HPP
#define MAINSTATE_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/GameState.hpp"
#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/Scene/Scene.hpp"
#include "gamelib/Scene/TilemapAdapter.hpp"
#include "gamelib/event/SFMLEvent.hpp"

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

        void update(float fps);
        void render(sf::RenderTarget& target);

    private:
        static void _eventCallback(MainState* me, gamelib::EventPtr ev);

    private:
        gamelib::Game* _game;
        gamelib::SpriteSet _spriteset;
        gamelib::Scene _scene;
        gamelib::TilemapAdapter _map;
        gamelib::TilemapAdapter _bgmap;
};

#endif
