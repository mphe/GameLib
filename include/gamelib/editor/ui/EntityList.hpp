#ifndef EDITOR_ENTITY_LIST_HPP
#define EDITOR_ENTITY_LIST_HPP

namespace gamelib
{
    constexpr const char* entity_properties_window_name = "Properties";

    void drawSearchPopup(bool* open = nullptr);
    void drawEntityList(bool* open = nullptr);
    void drawEntityProperties(bool* open = nullptr);
}

#endif
