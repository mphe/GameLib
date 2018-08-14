#include "gamelib/editor/ui/GlobalGameConfig.hpp"
#include "gamelib/editor/ui/inputs.hpp"
#include "gamelib/core/Game.hpp"

namespace gamelib
{
    void drawGameConfig(bool* open)
    {
        if (ImGui::Begin("Global Game Config", open, ImVec2(250, 285)))
        {
            auto game = getSubsystem<Game>();

            if (game)
            {
                inputProperties(game->getProperties());

                if (ImGui::Button("Save"))
                    game->write();

                // TODO: load, save as, ..
            }
        }
        ImGui::End();
    }
}
