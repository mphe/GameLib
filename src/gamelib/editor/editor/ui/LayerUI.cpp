#include "editor/editor/ui/LayerUI.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/rendering/flags.hpp"
#include "gamelib/utils/log.hpp"

#include "imgui.h"

namespace gamelib
{
    LayerUI::LayerUI() :
        _current(0)
    {
        _add();
    }

    void LayerUI::drawLayerUI()
    {
        Scene& scene = *Scene::getActive();
        Layer* layer = nullptr;

        { // Layer listbox with checkboxes to toggle visibility
            // TODO: Maybe open an issue on imgui's github page related to
            //       proper scaling when a listbox contains not only text

            if (ImGui::ListBoxHeader("Layers", -1, 7))
            {
                for (int i = 0; i < _layers.size(); i++)
                {
                    layer = scene.getLayer(_layers[i].handle);
                    bool layervis = layer ? !(layer->flags & render_invisible) : true;

                    ImGui::PushID(i);

                    if (ImGui::Checkbox("##layervis", &layervis) && layer)
                    {
                        if (layervis)
                            RMFLAG(layer->flags, render_invisible);
                        else
                            layer->flags |= render_invisible;
                    }

                    ImGui::SameLine();

                    if (ImGui::Selectable(_layers[i].name.c_str(), (i == _current)))
                        _current = i;

                    ImGui::PopID();
                }

                ImGui::ListBoxFooter();
            }
        }

        if (ImGui::Button("New Layer"))
        {
            _add(scene.createLayer());
            _current = _layers.size() - 1;
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete") && _current > 0)
        {
            scene.deleteLayer(getCurrent());
            _layers.erase(_layers.begin() + _current);
            if (_current >= _layers.size())
                _current = _layers.size() - 1;
        }

        ImGui::Separator();

        layer = scene.getLayer(getCurrent());

        if (layer)
        {
            ImGui::InputText("Name", &_layers[_current].name[0], 30);

            int depth = layer->getDepth();
            float parallax = layer->getParallax();

            if (ImGui::InputInt("Depth", &depth, 1, 100))
                layer->setDepth(depth);

            if (ImGui::InputFloat("Parallax", &parallax, 0.01, 0.1, 3))
                layer->setParallax(parallax);

            ImGui::CheckboxFlags("Disable parallax", &layer->flags, render_noparallax);
        }
    }

    LayerUI::LayerHandle LayerUI::drawSelector(const std::string& label, LayerHandle default_) const
    {
        static int selected = 0;
        static LayerHandle last;

        // Cache index
        if (default_ != last)
        {
            selected = 0;
            last = default_;
            for (size_t i = 0; i < _layers.size(); ++i)
                if (_layers[i].handle == default_)
                {
                    selected = i;
                    break;
                }
        }
        else if (Scene::getActive()->getLayer(default_) == nullptr)
            selected = 0;

        if (ImGui::Combo(label.c_str(), &selected, _itemGetter, (void*)&_layers, _layers.size()))
        {
            if (_layers[selected].handle.isNull())
                // Dirty hack to differentiate between null handle and default layer
                return LayerHandle(-1, -2);
            return _layers[selected].handle;
        }

        return LayerHandle();
    }

    void LayerUI::refresh()
    {
        LayerHandle oldhandle = getCurrent();;

        _layers.clear();
        _add();

        Scene::getActive()->foreachLayer([&](LayerHandle handle, Layer& layer) {
                if (handle == oldhandle)
                    _current = _layers.size();
                _add(handle);
            });

        if (_current >= _layers.size())
            _current = _layers.size() - 1;
    }

    LayerUI::LayerHandle LayerUI::getCurrent() const
    {
        return _layers[_current].handle;
    }

    bool LayerUI::_itemGetter(void* layers, int index, const char** out)
    {
        *out = static_cast<decltype(_layers)*>(layers)->at(index).name.c_str();
        return true;
    }

    void LayerUI::_add(LayerHandle handle)
    {
        auto scene = Scene::getActive();
        if (handle.isNull())
            _layers.emplace_back(LayerCache { handle, "default" });
        else
            _layers.emplace_back(LayerCache { handle, join("Layer ", scene->getLayer(handle)->getUniqueID()) });
        _layers.back().name.resize(30);
    }
}
