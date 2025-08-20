#include "MainScreen.h"

#include <string>
#include <utils/DataStore.h>
#include <tiktok/ApiManager.h>

#include <components/SoundComponent.h>

#include "components/VolumeComponent.h"
#include "utils/File.h"
#include "utils/ImGuiExtensions.h"

void MainScreen::Render()
{
    const auto& instance = GetInstance();
    auto& api = ApiManager::GetInstance();
    auto& datastore = DataStore::GetInstance();

    if (ImGui::Button("Refresh"))
    {
        instance.mShouldUpdateMusicList = true;
        File::DeleteFilesInCacheWithPrefix("musicList_");
    }
    EXPLANATION(Refresh, "Refresh the music list");

    ImGui::SameLine();

    if (const auto looptext = datastore.GetLooping() ? "Stop Looping" : "Loop"; ImGui::Button(looptext))
        datastore.Loop();

    EXPLANATION(Loop, "Loops the currently playing sound");

    ImGui::SameLine();

    const auto autoPlay = datastore.GetAutoPlay();
    if (const auto autoPlayText = autoPlay ? "Stop Auto Play" : "Auto Play"; ImGui::Button(autoPlayText))
        datastore.SetAutoPlay(!autoPlay);

    EXPLANATION(AutoPlay, "Loops the currently playing sound");

    ImGui::Separator();
    ImGui::BeginGroup();
    if (ImGui::Button("<"))
    {
        if (instance.mPage > 0)
        {
            instance.mPage--;
            instance.mShouldUpdateMusicList = true;
        }
    }
    ImGui::SameLine();
    ImGui::Text("%d", instance.mPage + 1);
    ImGui::SameLine();
    if (ImGui::Button(">"))
    {
        if (instance.mPage < 3)
        {
            instance.mPage++;
            instance.mShouldUpdateMusicList = true;
        }
    }
    ImGui::EndGroup();
    EXPLANATION(Pagination, "This allows you to switch pages. Each page has 35 sounds. Due to TikTok's API limitations, you can only access 140 sounds at a time.");

    ImGui::SameLine();

    VolumeComponent::Render();
    EXPLANATION(Volume, "Adjust the volume of the currently playing sound");

    ImGui::Separator();

    if (instance.mShouldUpdateMusicList)
    {
        instance.mMusicList = api.MusicList(instance.mCursors[instance.mPage]);
        instance.mShouldUpdateMusicList = false;

        instance.mCursors.resize(instance.mPage + 2);
        instance.mCursors[instance.mPage + 1] = instance.mMusicList["cursor"].get<long long>();
    }

    auto index = 0;
    for (const auto& music : instance.mMusicList["musicList"])
    {
        SoundComponent::Render(music, index);
        index++;
    }
}
