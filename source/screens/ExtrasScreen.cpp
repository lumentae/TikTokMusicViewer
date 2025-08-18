#include "ExtrasScreen.h"

#include <imgui.h>

#include "ImGuiHandler.h"
#include "utils/Config.h"
#include "utils/File.h"
#include "utils/ImGuiExtensions.h"

#ifdef WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <malloc.h>
#endif

void ExtrasScreen::Render()
{
    ImGui::Text("TikTok Music Viewer by lumentae");
    ImGui::Text("This application allows you to listen to saved TikTok sounds");

    if (ImGui::Button("Visit GitHub Repository"))
    {
        std::system("start https://github.com/lumentae/TikTokMusicViewer");
    }

    ImGui::Separator();

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
#ifdef WIN32
    PROCESS_MEMORY_COUNTERS_EX2 pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc))) {
        ImGui::Text("PrivateWorkingSetSize: %zu MB", pmc.PrivateWorkingSetSize / 1024 / 1024);
    } else {
        ImGui::Text("Failed to retrieve memory info");
    }
#else
    struct mallinfo mi = mallinfo();
    ImGui::Text("Heap total: %d KB", mi.arena / 1024);
    ImGui::Text("Heap used: %d KB", mi.uordblks / 1024);
    ImGui::Text("Heap free: %d KB", mi.fordblks / 1024);
#endif

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
    EXPLANATION(DeleteCache, "Deletes the cache");
    ImGui::SameLine();
    if (ImGui::Button("Delete all data"))
    {
        File::Delete(File::GetCachePath());
        File::Delete("cookie.txt");
    }
    EXPLANATION(DeleteAllData, "Deletes all data");
}
