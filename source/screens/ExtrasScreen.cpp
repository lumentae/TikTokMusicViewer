#include "ExtrasScreen.h"

#include <imgui.h>

#include "ImGuiHandler.h"
#include "utils/Config.h"
#include "utils/File.h"
#include "utils/ImGuiExtensions.h"

void ExtrasScreen::Render()
{
    ImGui::Text("TikTok Music Viewer by lumentae");
    ImGui::Text("This application allows you to listen to saved TikTok sounds");

    if (ImGui::Button("Visit GitHub Repository"))
    {
        std::system("start https://github.com/lumentae/TikTokMusicViewer");
    }

    ImGui::Separator();

    if (ImGui::Button("Restart tutorial"))
    {
        CONFIG_SET_VALUE(Tutorial, 0);
    }
    EXPLANATION(RestartTutorial, "Restarts the tutorial");

    ImGui::Separator();

    if (ImGui::Button("Delete cache"))
    {
        File::Delete(File::GetCachePath());
    }
    EXPLANATION(DeleteCache, "WARNING: Deletes the cache");
    ImGui::SameLine();
    if (ImGui::Button("Delete all data"))
    {
        File::Delete(File::GetCachePath());
        File::Delete("cookie.txt");
    }
    EXPLANATION(DeleteAllData, "WARNING: Deletes all data");
}
