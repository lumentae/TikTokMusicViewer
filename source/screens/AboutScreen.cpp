#include "AboutScreen.h"

#include <imgui.h>

#include "ImGuiHandler.h"
#include "utils/File.h"

void AboutScreen::Render()
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
        ImGuiHandler::GetInstance().mTutorial = true;
    }

    ImGui::Separator();

    if (ImGui::Button("Delete cache"))
    {
        File::Delete(File::GetCachePath());
    }
    ImGui::SameLine();
    if (ImGui::Button("Delete all data"))
    {
        File::Delete(File::GetCachePath());
        File::Delete("cookie.txt");
    }
}
