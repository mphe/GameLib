#include "imgui.h"
#include "imgui-SFML.h"
#include "IconsFontAwesome5.h"
// #include "IconsFontAwesome5Brands.h"
// #include "IconsFontAwesome4.h"
#include <SFML/Graphics.hpp>

void setupIconFont()
{
    ImGuiIO& io = ImGui::GetIO();
    // io.Fonts->AddFontDefault();

    // merge in icons from Font Awesome
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
    // icons_config.OversampleH = icons_config.OversampleV = 1;
    // io.Fonts->Flags |= ImFontAtlasFlags_NoPowerOfTwoHeight;

    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 16.0f, &icons_config, icons_ranges);
    // use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

    // static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    // io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FA, 16.0f, &icons_config, icons_ranges);
    ImGui::SFML::UpdateFontTexture();

    // static const ImWchar icons_ranges_brands[] = { ICON_MIN_FAB, ICON_MAX_FAB, 0 };
    // io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAB, 16.0f, &icons_config, icons_ranges_brands);

    // io.Fonts->Build();
}

int main()
{
    sf::RenderWindow win;
    win.create(sf::VideoMode(1024, 600), "ImGui test");
    win.setFramerateLimit(60);
    ImGui::SFML::Init(win);

    setupIconFont();

    while (win.isOpen())
    {
        sf::Event ev;
        while (win.pollEvent(ev))
        {
            ImGui::SFML::ProcessEvent(ev);
            if (ev.type == sf::Event::Closed)
                win.close();
            else if (ev.type == sf::Event::KeyPressed
                    && ev.key.code == sf::Keyboard::Escape)
                win.close();

        }

        ImGui::SFML::Update(win, sf::seconds(1.0/60.f));

        ImGui::ShowStyleEditor();
        // ImGui::ShowDemoWindow();

        win.resetGLStates();
        win.clear();
        ImGui::SFML::Render(win);
        win.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
