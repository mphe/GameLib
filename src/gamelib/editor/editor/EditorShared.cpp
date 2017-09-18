#include "gamelib/editor/editor/EditorShared.hpp"
#include "gamelib/editor/editor/Editor.hpp"

namespace gamelib
{
    const math::Point2f& EditorShared::getMouse()
    {
        return _editor->_mouse;
    }

    const math::Point2f& EditorShared::getMouseSnapped()
    {
        return _editor->_mouseSnapped;
    }

    math::Point2f EditorShared::snap(const math::Point2f& p)
    {
        if (_editor->_snap)
            return _editor->_grid.snap(p);
        return p;
    }

    bool EditorShared::isSnapEnabled()
    {
        return _editor->_snap;
    }

    const LayerUI& EditorShared::getLayerUI()
    {
        return _editor->_layerui;
    }

    SelectTool& EditorShared::getSelectTool()
    {
        return _editor->getSelectTool();
    }


    Editor* EditorShared::_editor = nullptr;
}
