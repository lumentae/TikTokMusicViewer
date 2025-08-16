#include "SettingsScreen.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "utils/Config.h"

void SettingsScreen::Render()
{
    std::string cookie = Config::Cookie;

    ImGui::Text("Cookie:");
    if (ImGui::InputText("##", &cookie, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AlwaysOverwrite))
    {
        CONFIG_SET_VALUE(Cookie, cookie);
    }
}
