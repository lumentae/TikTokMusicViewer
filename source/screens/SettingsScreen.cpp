#include "SettingsScreen.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "tiktok/ApiManager.h"

void SettingsScreen::Render()
{
    auto& api = ApiManager::GetInstance();
    std::string cookie = api.GetCookie();

    ImGui::Text("Cookie:");
    if (ImGui::InputText("##", &cookie, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AlwaysOverwrite))
    {
        api.SetCookie(cookie);
    }
}
