#include "SettingsScreen.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "utils/Config.h"
#include "utils/ImGuiExtensions.h"

void SettingsScreen::Render()
{
    std::string cookie = Config::Cookie;
    std::string secUid = Config::SecUid;

    ImGui::Text("Cookie:");
    if (ImGui::InputText("##cookie", &cookie, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AlwaysOverwrite))
    {
        CONFIG_SET_VALUE(Cookie, cookie);
    }
    EXPLANATION(Cookie, "This is your TikTok cookie. It is used to authenticate your requests to the TikTok API. You can find it when you copy any request as 'fetch (Node.js) and copy the data in the cookie field.");
    ImGui::Separator();
    ImGui::Text("SecUid:");
    if (ImGui::InputText("##secuid", &secUid, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_AlwaysOverwrite))
    {
        CONFIG_SET_VALUE(SecUid, secUid);
    }
    EXPLANATION(SecUid, "This is your TikTok SecUid. It is used to authenticate your requests to the TikTok API. You can find it when you copy any request as 'fetch (Node.js) and look at the request parameters. It will look like '&secUid=<your secUid>'.");
}
