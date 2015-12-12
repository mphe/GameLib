#include "gamelib/Game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "gamelib/GameState.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/event/SFMLEvent.hpp"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480
#define GAME_TITLE "Unnamed game"
#define GAME_MAX_FPS 0
#define GAME_VSYNC false
#define GAME_ESCCLOSE true
#define GAME_CLOSEBUTTON true
#define GAME_REPEATKEYS false

namespace gamelib
{
    Game::Game() :
        _active(true),
        _handleclose(GAME_CLOSEBUTTON),
        _escclose(GAME_ESCCLOSE)
    { }

    Game::~Game()
    {
        destroy();
    }


    bool Game::init()
    {
        LOG("Initializing game...");
        _window.create(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), GAME_TITLE, sf::Style::Close);
        _window.setFramerateLimit(GAME_MAX_FPS);
        _window.setVerticalSyncEnabled(GAME_VSYNC);
        _window.setKeyRepeatEnabled(GAME_REPEATKEYS);

        return true;
    }

    void Game::run()
    {
        float fps = 1; // TODO: Consider switching to double
        sf::Clock clock;
        std::shared_ptr<SFMLEvent> ev(new SFMLEvent());

        while (_window.isOpen())
        {
            clock.restart();

            while (_window.pollEvent(ev->ev))
            {
                switch (ev->ev.type)
                {
                    case sf::Event::KeyPressed:
                        if (_escclose && ev->ev.key.code == sf::Keyboard::Escape)
                        {
                            close();
                            return;
                        }
                        break;

                    case sf::Event::Closed:
                        if (_handleclose)
                        {
                            close();
                            return;
                        }
                        break;

                    case sf::Event::GainedFocus:
                        _active = true;
                        break;

                    case sf::Event::LostFocus:
                        _active = false;
                        break;
                }
                _evmgr.triggerEvent(ev);
                ev.reset(new SFMLEvent());
            }

            for (auto& i : _states)
            {
                i->update(fps);
            }

            _evmgr.update();

            _window.clear(_bgcolor);
            for (auto& i : _states)
            {
                i->render(_window);
            }
            _window.display();

            //calculate fps
            sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
            fps = 1000.0f / (elapsed ? elapsed : 1);
            LOG_DEBUG_RAW("\r", LOG_DUMP(elapsed), "ms, ", LOG_DUMP(fps), "     \r"); // write some spaces to overwrite existing characters
        }
    }

    void Game::close()
    {
        if (_window.isOpen())
        {
            _window.close();
            LOG_DEBUG("Game window closed");
        }
    }

    void Game::destroy()
    {
        close();
        if (!_states.empty())
        {
            LOG_DEBUG("Closing game states...");
            for (auto& i : _states)
            {
                i->quit();
            }
            _states.clear();
        }
    }


    bool Game::loadFromJson(const Json::Value& node)
    {
        if (node.isMember("title"))
            _window.setTitle(node["title"].asString());

        auto size = _window.getSize();
        size.x = node.get("width", size.x).asUInt();
        size.y = node.get("height", size.y).asUInt();
        _window.setSize(size);

        if (node.isMember("maxfps"))
            _window.setFramerateLimit(node["maxfps"].asUInt());

        if (node.isMember("vsync"))
            _window.setVerticalSyncEnabled(node["vsync"].asBool());

        if (node.isMember("repeatkeys"))
            _window.setKeyRepeatEnabled(node["repeatkeys"].asBool());

        _handleclose = node.get("handleclose", _handleclose).asBool();
        _escclose = node.get("escclose", _escclose).asBool();

        if (node.isMember("bg"))
        {
            const auto& bg = node["bg"];
            _bgcolor.r = bg.get("r", _bgcolor.r).asUInt();
            _bgcolor.g = bg.get("g", _bgcolor.g).asUInt();
            _bgcolor.b = bg.get("b", _bgcolor.b).asUInt();
        }

        return true;
    }


    void Game::pushState(std::unique_ptr<GameState> state)
    {
        if (!state->init(this))
        {
            LOG_ERROR("Failed to initialize game state");
            return;
        }
        _states.push_back(std::move(state));
        LOG_DEBUG("Game state added");
    }

    void Game::popState()
    {
        _states.back()->quit();
        _states.pop_back();
    }

    GameState& Game::pullState() const
    {
        return *_states.back().get();
    }

    sf::RenderWindow& Game::getWindow()
    {
        return _window;
    }

    bool Game::isActive() const
    {
        return _active;
    }

    bool Game::isKeyPressed(sf::Keyboard::Key key) const
    {
        return _active && sf::Keyboard::isKeyPressed(key);
    }

    void Game::regEventCallback(EventID id, const EventCallback& callback)
    {
        _evmgr.regCallback(id, callback);
        LOG_DEBUG("Added event callback for ", id);
    }

    void Game::unregEventCallback(EventID id, const EventCallback& callback)
    {
        _evmgr.unregCallback(id, callback);
        LOG_DEBUG("Removed event callback for ", id);
    }
}

