#include "MainScreen.h"

#include <imgui.h>
#include <string>
#include <utils/DataStore.h>
#include <tiktok/ApiManager.h>

#include <components/SoundComponent.h>

void MainScreen::Render()
{
    const auto& instance = GetInstance();
    auto& api = ApiManager::GetInstance();
    auto& datastore = DataStore::GetInstance();
    std::string cookie = api.GetCookie();

    if (ImGui::Button("Refresh"))
    {
        instance.mShouldUpdateMusicList = true;
    }

    ImGui::SameLine();

    if (const auto looptext = datastore.GetLooping() ? "Stop Looping" : "Loop"; ImGui::Button(looptext))
        datastore.Loop();

    ImGui::Separator();

    auto musicList = instance.mMusicList;
    if (instance.mShouldUpdateMusicList)
    {
        musicList = api.MusicList(instance.mCursors[instance.mPage]);
        instance.mMusicList = musicList;
        instance.mShouldUpdateMusicList = false;
    }

    instance.mCursors.emplace_back(musicList["cursor"].get<long long>());
    for (const auto& music : musicList["musicList"])
    {
        SoundComponent::Render(music);
    }
}
