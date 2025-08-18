#include "SettingsScreen.h"

#include <imgui.h>
#include <imgui_stdlib.h>

#include "utils/Config.h"
#include "utils/ImGuiExtensions.h"

std::string autoCredentials = "";
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

    ImGui::Separator();

    ImGui::InputText("##autoCredentials", &autoCredentials);
    if (ImGui::Button("Fill in credentials"))
    {
        AutoCredentials(autoCredentials);
    }
    EXPLANATION(AutoCredentials, "Paste a copied 'fetch (Node.js)' request here to automatically fill in the cookie and secUid fields. This is useful if you want to use the app without manually entering your cookie and secUid.");
}

void SettingsScreen::AutoCredentials(const std::string& input)
{
    const std::string secUid = [&]
    {
        const std::regex regex(R"(secUid=(.*)&)");
        std::smatch match;
        auto searchStart(input.cbegin());
        std::string matchStr = "";

        while (std::regex_search(searchStart, input.cend(), match, regex)) {
            searchStart = match.suffix().first;
            matchStr = match[1].str();
        }
        return matchStr;
    }();

    const std::string cookie = [&]
    {
        const std::regex regex(R"raw("cookie": "(.*)")raw");
        std::smatch match;
        auto searchStart(input.cbegin());
        std::string matchStr = "";

        while (std::regex_search(searchStart, input.cend(), match, regex)) {
            searchStart = match.suffix().first;
            matchStr = match[1].str();
        }
        return matchStr;
    }();

    CONFIG_SET_VALUE(Cookie, cookie);
    CONFIG_SET_VALUE(SecUid, secUid);
    Config::Save();
}