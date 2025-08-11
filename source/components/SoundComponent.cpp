#include "SoundComponent.h"

#ifdef PlaySound
#undef PlaySound
#endif

#include "imgui.h"
#include "../utils/File.h"

void SoundComponent::Render(nlohmann::json music)
{
    const std::string title = music["music"]["title"];
    const std::string authorName = music["music"]["authorName"];
    const std::string id = music["music"]["id"].get<std::string>();
    const long long idNum = std::stoll(id);

    ImGui::BeginChild("SoundComponent");
    ImGui::Text(title.c_str());
    ImGui::Text(authorName.c_str());
    ImGui::PushID(idNum);

    auto& datastore = DataStore::GetInstance();
    //auto playingSound = datastore.GetCurrentlyPlayingSound();
    if (ImGui::Button("Play"))
    {
        File::DownloadSoundAndLoad(music["music"]["playUrl"], idNum);
        datastore.PlaySound(idNum);
    }
    ImGui::PopID();
    ImGui::Separator();
    ImGui::EndChild();
}