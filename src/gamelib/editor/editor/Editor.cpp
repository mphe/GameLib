#include "gamelib/editor/editor/Editor.hpp"
#include "gamelib/core/Game.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/core/res/resources.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/events/SFMLEvent.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguifilesystem.h"
#include "gamelib/editor/editor/tools/BrushTool.hpp"
#include "gamelib/editor/editor/tools/VertexTool.hpp"
#include "gamelib/editor/editor/tools/SelectTool.hpp"
#include "gamelib/editor/editor/tools/EntityTool.hpp"
#include "gamelib/editor/editor/ui/JsonView.hpp"
#include "gamelib/editor/editor/ui/props.hpp"
#include "gamelib/editor/editor/EditorShared.hpp"

// Components
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/update/QController.hpp"
#include "gamelib/editor/components/BrushComponent.hpp"
#include "gamelib/editor/components/PolygonShape.hpp"

// TODO: block key pressed when they are consumed by imgui

namespace gamelib
{
    constexpr const char* buttonStrings[] = {
        "Select Tool",
        "Polygon Tool",
        "Vertex Tool",
        "Entity Tool"
    };

    Editor::Editor() :
        _game(nullptr),
        _currenttool(nullptr),
        _camctrl(&_scene),
        _grid(32, 32),
        _snap(true),
        _drag(false),
        _gridOnTop(false),
        _run(false)
    { }

    bool Editor::init(Game* game)
    {
        LOG_DEBUG("Init Editor...");
        _game = game;
        EditorShared::_editor = this;

        ImGui::SFML::Init(_game->getWindow());

        auto evmgr = getSubsystem<EventManager>();
        evmgr->regCallback(SFMLEvent::id, _eventCallback, this);

        _entfactory.addComponent<Polygon>(Polygon::name);
        _entfactory.addComponent<AABB>(AABB::name);
        _entfactory.addComponent<QPhysics>(QPhysics::name);
        _entfactory.addComponent<QController>(QController::name);
        _entfactory.addComponent<PolygonShape>(PolygonShape::name);
        _entfactory.addComponent<BrushComponent>(BrushComponent::name);

        registerPredefLoaders(_resmgr);
        _resmgr.loadFromFile("assets/res.json");

        _scene.addCamera();
        _scene.getCamera(0)->loadFromFile("assets/cam.json");
        _scene.setDefaultCamera(0);
        _scene.flags |= render_noparallax | render_drawhidden;

        _tools[ToolSelect].reset(new SelectTool());
        _tools[ToolBrush].reset(new BrushTool());
        _tools[ToolVertex].reset(new VertexTool());
        _tools[ToolEntity].reset(new EntityTool());
        setTool(ToolBrush);

        _layerui.refresh();

        return true;
    }

    void Editor::quit()
    {
        auto evmgr = getSubsystem<EventManager>();
        evmgr->unregCallback(SFMLEvent::id, _eventCallback, this);

        _currenttool = nullptr;
        for (auto& i : _tools)
            i.reset();

        _entmgr.clear();
        _scene.destroy();
        _colsys.destroy();
        _updatesystem.destroy();
        _resmgr.clear();
        _entfactory.clear();
        ImGui::SFML::Shutdown();
        LOG_DEBUG("Editor unloaded");
    }

    void Editor::update(float elapsed)
    {
        if (_game->getWindow().hasFocus())
        {
            if (!_run)
                _camctrl.update(elapsed);
            else
                _updatesystem.update(elapsed);

            _scene.update(elapsed);

            ImGui::SFML::Update(_game->getWindow(), sf::seconds(elapsed));
            _drawGui();
        }
    }

    void Editor::render(sf::RenderTarget& target)
    {
        // necessary, otherwise things will disappear randomly
        _game->getWindow().resetGLStates();

        if (!_gridOnTop && _snap)
            _grid.render(target);

        _scene.render(target);

        if (_gridOnTop && _snap)
            _grid.render(target);

        if (_currenttool)
            _currenttool->render(target);

        // if (_currenttool != &getSelectTool())
        //     getSelectTool().render(target);

        ImGui::Render();
    }

    void Editor::setTool(Tools tool)
    {
        _currenttool = _tools[tool].get();
        LOG("Selected tool: ", buttonStrings[static_cast<size_t>(tool)]);
    }

    SelectTool& Editor::getSelectTool()
    {
        return *(SelectTool*)_tools[ToolSelect].get();
    }

    void Editor::writeToJson(Json::Value& node)
    {
        auto scene = Scene::getActive();
        if (scene)
            scene->writeToJson(node["scene"]);

        auto entmgr = EntityManager::getActive();
        if (entmgr)
            entmgr->writeToJson(node["entmgr"]);

        QPhysics::writeGlobalsToJson(node["physics"]);
    }

    bool Editor::loadFromJson(const Json::Value& node)
    {
        auto scene = Scene::getActive();
        if (scene)
            scene->loadFromJson(node["scene"]);

        auto entmgr = EntityManager::getActive();
        if (entmgr)
            entmgr->loadFromJson(node["entmgr"]);

        QPhysics::loadGlobalsFromJson(node["physics"]);

        _layerui.refresh();

        return true;
    }

    void Editor::_eventCallback(Editor* me, EventPtr ev)
    {
        sf::Event& sfev = ev->get<SFMLEvent>()->ev;
        ImGui::SFML::ProcessEvent(sfev);
        bool consumed = ImGui::GetIO().WantCaptureMouse;

        Tool* realtool = me->_currenttool;  // backup
        if (me->_game->isKeyPressed(sf::Keyboard::LControl))
            me->_currenttool = &me->getSelectTool();

        switch (sfev.type)
        {
            case sf::Event::KeyPressed:
                if (ImGui::GetIO().WantCaptureKeyboard)
                    break;

                switch (sfev.key.code)
                {
                    case sf::Keyboard::R:
                        LOG_WARN("---------------- Reloading...");
                        me->_game->reload();
                        me->quit();
                        me->init(me->_game);
                        LOG("---------------- Reloading complete");
                        return; // early out, so that _currenttool doesn't get reset at the end of the function

                    // case sf::Keyboard::Q:
                    //     me->_game->close();
                    //     break;

                    case sf::Keyboard::Delete:
                        if (me->getSelectTool().getSelected())
                        {
                            me->getSelectTool().getSelected()->destroy();
                            me->getSelectTool().select(nullptr);
                        }
                        break;

                    case sf::Keyboard::Q:
                        me->_grid.increase();
                        break;

                    case sf::Keyboard::E:
                        me->_grid.decrease();
                        break;

                    case sf::Keyboard::G:
                        me->_snap = !me->_snap;
                        break;

                    case sf::Keyboard::F5:
                        me->_run = !me->_run;
                        break;
                }
                break;

            case sf::Event::MouseButtonPressed:
                if (!consumed && sfev.mouseButton.button == sf::Mouse::Left)
                {
                    me->_updateShared(sfev.mouseButton.x, sfev.mouseButton.y);
                    me->_currenttool->onMousePressed();
                    me->_drag = true;
                }
                break;

            case sf::Event::MouseMoved:
                {
                    me->_updateShared(sfev.mouseMove.x, sfev.mouseMove.y);
                    if (me->_drag)
                        me->_currenttool->onDrag();
                    else
                        me->_currenttool->onMouseMove();
                }
                break;

            case sf::Event::MouseButtonReleased:
                {
                    me->_updateShared(sfev.mouseButton.x, sfev.mouseButton.y);
                    me->_drag = false;
                    me->_currenttool->onMouseRelease();
                }
                break;

            case sf::Event::MouseWheelMoved:
                if (!consumed)
                {
                    auto p = me->_mapCoords(sfev.mouseWheel.x, sfev.mouseWheel.y);
                    auto& cam = *me->_scene.getCamera(0);
                    cam.zoomTowards(p.x, p.y, sfev.mouseWheel.delta / -10.0);
                }
                break;
        }

        me->_currenttool = realtool;
    }

    void Editor::_drawGui()
    {
        static bool testwindow = false;
        static bool jsonwindow = false;
        static ImGuiFs::Dialog loaddlg;
        static ImGuiFs::Dialog savedlg;
        static std::string currentFilePath;

        bool chooseload = false;
        bool choosesave = false;
        const char* chosenPath;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    if (currentFilePath.empty())
                        choosesave = true;
                    else
                        writeToFile(currentFilePath);
                }

                if (ImGui::MenuItem("Save as"))
                    choosesave = true;

                if (ImGui::MenuItem("Load"))
                    chooseload = true;

                if (ImGui::MenuItem("Quit"))
                    _game->close();

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Run", "F5", &_run))
                {
                    if (_run)
                        RMFLAG(_scene.flags, render_noparallax | render_drawhidden);
                    else
                        _scene.flags |= render_noparallax | render_drawhidden;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Show test window", nullptr, &testwindow);
                ImGui::MenuItem("Show json window", nullptr, &jsonwindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Grid"))
            {
                ImGui::MenuItem("Snap to grid", nullptr, &_snap);
                ImGui::MenuItem("Grid on top", nullptr, &_gridOnTop);
                if (ImGui::MenuItem("Increase"))
                    _grid.increase();
                if (ImGui::MenuItem("Decrease"))
                    _grid.decrease();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (testwindow)
            ImGui::ShowTestWindow(&testwindow);

        if (jsonwindow)
            drawJsonView(&jsonwindow);

        chosenPath = loaddlg.chooseFileDialog(chooseload);
        if (strlen(chosenPath) > 0)
        {
            currentFilePath = chosenPath;
            loadFromFile(currentFilePath);
        }

        chosenPath = savedlg.saveFileDialog(choosesave);
        if (strlen(chosenPath) > 0)
        {
            currentFilePath = chosenPath;
            writeToFile(currentFilePath);
        }

        ImGui::Begin("Toolbox");
        for (size_t i = 0; i < NumTools; ++i)
            if (ImGui::Button(buttonStrings[i]))
                setTool(static_cast<Tools>(i));
        ImGui::End();

        ImGui::Begin("Tool properties");
        _currenttool->drawGui();
        ImGui::End();

        ImGui::Begin("Layer Properties");
        _layerui.drawLayerUI();
        ImGui::End();

        if (getSelectTool().getSelected())
        {
            ImGui::Begin("Entity properties");
            drawEntityProps(*getSelectTool().getSelected());
            ImGui::End();
        }
    }

    math::Point2f Editor::_mapCoords(float x, float y)
    {
        auto p = _game->getWindow().mapPixelToCoords(sf::Vector2i(x, y));
        return math::Point2f(p.x, p.y);
    }

    void Editor::_updateShared(float mx, float my)
    {
        _mouse = _mapCoords(mx, my);
        _mouseSnapped = EditorShared::snap(_mouse);
    }
}
