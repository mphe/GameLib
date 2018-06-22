#include "gamelib/core/Game.hpp"
#include <SFML/Graphics.hpp>
#include "gamelib/core/GameState.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/json.hpp"
#include "gamelib/events/SFMLEvent.hpp"

constexpr const char* game_title = "Unnamed game";
constexpr int game_width         = 640;
constexpr int game_height        = 480;
constexpr int game_max_fps       = 0;
constexpr bool game_vsync        = true;
constexpr bool game_escclose     = true;
constexpr bool game_closebutton  = true;
constexpr bool game_repeatkeys   = false;

namespace gamelib
{
    constexpr const char* Game::name;

    Game::Game() :
        _frametime(0),
        _focused(true),
        _handleclose(game_closebutton),
        _escclose(game_escclose)
    { }

    Game::~Game()
    {
        destroy();
    }


    bool Game::init()
    {
        LOG("Initializing game...");
        _window.create(sf::VideoMode(game_width, game_height), game_title, sf::Style::Close);
        _window.setFramerateLimit(game_max_fps);
        _window.setVerticalSyncEnabled(game_vsync);
        _window.setKeyRepeatEnabled(game_repeatkeys);
        return true;
    }

    void Game::run()
    {
        sf::Clock clock;
        SFMLEvent::Pointer ev(new SFMLEvent());

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
                        _focused = true;
                        break;

                    case sf::Event::LostFocus:
                        _focused = false;
                        break;
                }

                _evmgr.triggerEvent(ev);
                ev.reset(new SFMLEvent());
            }

            if (_focused)
            {
                bool frozen = false;
                for (auto it = _states.rbegin(), end = _states.rend(); it != end; ++it)
                {
                    auto state = (*it).get();
                    if (state->flags & gamestate_paused)
                        continue;

                    if (!frozen || state->flags & gamestate_forceupdate)
                        state->update(_frametime);

                    if (state->flags & gamestate_freeze)
                        frozen = true;
                }

                _evmgr.update();
            }
            _window.resetGLStates(); // without this things start randomly disappearing
            _window.clear(_bgcolor);
            for (auto& i : _states)
                i->render(_window);
            _window.display();

            // Get elapsed time
            _frametime = clock.getElapsedTime().asMilliseconds() / 1000.0f;
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
                i->quit();
            _states.clear();
        }
    }


    bool Game::loadFromJson(const Json::Value& node)
    {
        auto size = _window.getSize();
        auto pos = _window.getPosition();
        size.x = node.get("width", size.x).asUInt();
        size.y = node.get("height", size.y).asUInt();

        auto diff = size - _window.getSize();
        pos.x -= diff.x / 2;
        pos.y -= diff.y / 2;

        _window.setSize(size);
        _window.setPosition(pos);
        _window.setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));

        // Don't create a new window, as it could lead to problems
        // if (size != _window.getSize())
        // {
        //     _window.close();
        //     _window.create(sf::VideoMode(size.x, size.y), game_title, sf::Style::Close);
        // }

        if (node.isMember("title"))
            _window.setTitle(node["title"].asString());

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

    float Game::getFrametime() const
    {
        return _frametime;
    }

    sf::RenderWindow& Game::getWindow()
    {
        return _window;
    }

    EventManager& Game::getEventManager()
    {
        return _evmgr;
    }

    bool Game::isFocused() const
    {
        return _focused;
    }

    bool Game::isKeyPressed(sf::Keyboard::Key key) const
    {
        return _focused && sf::Keyboard::isKeyPressed(key);
    }

    math::Point2f Game::getMouse() const
    {
        auto m = sf::Mouse::getPosition(_window);
        auto p = _window.mapPixelToCoords(m);
        return math::Point2f(p.x, p.y);
    }
}

