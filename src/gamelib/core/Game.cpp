#include "gamelib/core/Game.hpp"
#include <SFML/Graphics.hpp>
#include <climits>
#include "gamelib/core/GameState.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/events/SFMLEvent.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

namespace gamelib
{
    Game::Game() :
        bgcolor(sf::Color::Black),
        closebutton(true),
        escclose(true),
        unfocusPause(true),
        _frametime(0),
        _rendertime(0),
        _updatetime(0),
        _size(640, 480),
        _maxfps(60),
        _title("Unnamed Game"),
        _repeatkeys(false),
        _vsync(true),
        _initialized(false)
    {
        auto setSize = +[](const math::Vec2i* val, Game* self) {
            self->resize(sf::Vector2u(val->x, val->y));
        };
        auto setFPS = +[](const int* val, Game* self) {
            self->_maxfps = *val;
            if (self->_window.isOpen())
                self->_window.setFramerateLimit(*val);
        };
        auto setVsync = +[](const bool* val, Game* self) {
            self->_vsync = *val;
            if (self->_window.isOpen())
                self->_window.setVerticalSyncEnabled(*val);
        };
        auto setTitle = +[](const std::string* val, Game* self) {
            self->_title = *val;
            if (self->_window.isOpen())
                self->_window.setTitle(*val);
        };
        auto setRepeat = +[](const bool* val, Game* self) {
            self->_repeatkeys = *val;
            if (self->_window.isOpen())
                self->_window.setKeyRepeatEnabled(*val);
        };

        _props.registerProperty("bgcolor", bgcolor);
        _props.registerProperty("closebutton", closebutton);
        _props.registerProperty("escclose", escclose);
        _props.registerProperty("unfocusPause", unfocusPause);
        _props.registerProperty("size", _size, setSize, this);
        _props.registerProperty("maxfps", _maxfps, setFPS, this, 0, INT_MAX);
        _props.registerProperty("vsync", _vsync, setVsync, this);
        _props.registerProperty("title", _title, setTitle, this);
        _props.registerProperty("repeatkeys", _repeatkeys, setRepeat, this);
    }

    Game::~Game()
    {
        destroy();
    }


    bool Game::init()
    {
        LOG("Initializing game...");
        _window.create(sf::VideoMode(_size.x, _size.y), _title, sf::Style::Close);
        _window.setFramerateLimit(_maxfps);
        _window.setVerticalSyncEnabled(_vsync);
        _window.setKeyRepeatEnabled(_repeatkeys);

        ImGui::SFML::Init(_window);

        // ImGui::StyleColorsClassic();
        ImGui::GetStyle().WindowBorderSize = 0;
        ImGui::GetStyle().WindowRounding = 0;
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::GetIO().ConfigDockingWithShift = false;
        ImGui::GetIO().ConfigWindowsResizeFromEdges = true;

        _initialized = true;

        return true;
    }

    auto Game::isInitialized() const -> bool
    {
        return _initialized;
    }

    void Game::run()
    {
        if (!isInitialized())
        {
            LOG_ERROR("Game is not initialized");
            return;
        }

        sf::Clock clock, detailclock;
        sf::Event ev;

        while (_window.isOpen())
        {
            clock.restart();
            detailclock.restart();

            auto inputsys = getSubsystem<InputSystem>();
            if (inputsys)
                inputsys->beginFrame();

            while (_window.pollEvent(ev))
            {
                if (inputsys)
                    inputsys->process(ev);

                ImGui::SFML::ProcessEvent(ev);

                // TODO: clear keyboard

                auto evmgr = getSubsystem<EventManager>();
                if (evmgr)
                    evmgr->triggerEvent(SFMLEvent::create(ev));

                if (closebutton && ev.type == sf::Event::Closed)
                {
                    close();
                    return;
                }
            }

            if (_window.hasFocus() || !unfocusPause)
            {
                ImGui::SFML::Update(_window, sf::seconds(_frametime));

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

                if (escclose && inputsys && inputsys->isKeyPressed(sf::Keyboard::Escape))
                {
                    close();
                    return;
                }
            }

            _updatetime = detailclock.getElapsedTime().asMilliseconds() / 1000.f;
            detailclock.restart();

            _window.resetGLStates(); // without this things start randomly disappearing
            _window.clear(bgcolor);

            if (_vsync)
                detailclock.restart();

            for (auto& i : _states)
                i->render(_window);

            ImGui::SFML::Render(_window);

            // TODO: implement own fps capping.
            // SFML caps inside window.display(), so it's impossible to get the exact render time
            if (!_vsync)
                _rendertime = detailclock.getElapsedTime().asMilliseconds() / 1000.0f;

            _window.display();

            if (_vsync)
                _rendertime = detailclock.getElapsedTime().asMilliseconds() / 1000.0f;

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
        if (!isInitialized())
            return;

        close();

        if (!_states.empty())
        {
            LOG_DEBUG("Closing game states...");
            for (auto& i : _states)
                i->quit();
            _states.clear();
        }

        ImGui::SFML::Shutdown();
        _initialized = false;
    }


    bool Game::loadFromJson(const Json::Value& node)
    {
        return _props.loadFromJson(node);
    }

    void Game::writeToJson(Json::Value& node) const
    {
        _props.writeToJson(node);
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

    GameState* Game::pullState() const
    {
        return _states.back().get();
    }

    float Game::getFrametime() const
    {
        return _frametime;
    }

    float Game::getRealFrametime() const
    {
        return _rendertime + _updatetime;
    }

    float Game::getRenderTime() const
    {
        return _rendertime;
    }

    float Game::getUpdateTime() const
    {
        return _updatetime;
    }

    sf::RenderWindow& Game::getWindow()
    {
        return _window;
    }

    const PropertyContainer& Game::getProperties() const
    {
        return _props;
    }

    void Game::resize(const sf::Vector2u& size)
    {
        if (size.x <= 0 || size.y <= 0)
        {
            LOG_ERROR("Invalid window size: ", size.x, "x", size.y);
            return;
        }

        _size.fill(size.x, size.y);

        if (_window.isOpen())
        {
            auto diff = size - _window.getSize();
            auto pos = _window.getPosition();
            pos.x -= diff.x / 2;
            pos.y -= diff.y / 2;

            _window.setSize(size);
            _window.setPosition(pos);
            _window.setView(sf::View(sf::FloatRect(0, 0, size.x, size.y)));
        }

        // Don't create a new window, as it could lead to problems
        // if (size != _window.getSize())
        // {
        //     _window.close();
        //     _window.create(sf::VideoMode(size.x, size.y), game_title, sf::Style::Close);
        // }
    }
}

