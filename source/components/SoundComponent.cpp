#include "SoundComponent.h"

#ifdef PlaySound
#undef PlaySound
#endif

#include "imgui.h"
#include "utils/File.h"
#include "utils/DataStore.h"

void SoundComponent::Render(nlohmann::json music)
{
    const std::string title = music["music"]["title"];
    const std::string authorName = music["music"]["authorName"];
    const std::string id = music["music"]["id"];
    const long long idNum = std::stoll(id);

    ImGui::BeginChild("SoundComponent");
    ImGui::Text(title.c_str());
    ImGui::Text(authorName.c_str());
    ImGui::PushID(idNum);

    auto& datastore = DataStore::GetInstance();
    if (const std::string playPause = datastore.GetCurrentlyPlayingId() == idNum ? "Pause" : "Play"; ImGui::Button(playPause.c_str()))
    {
        if (playPause == "Pause")
        {
            datastore.StopSound();
        }
        else
        {
            File::DownloadSoundAndLoad(music["music"]["playUrl"], idNum);
            datastore.PlaySound(idNum);
        }
    }
    ImGui::PopID();
    ImGui::Separator();
    ImGui::EndChild();
}
