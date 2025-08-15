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

    if (const auto looptext = datastore.GetLooping() ? "Stop Looping" : "Loop"; ImGui::Button(looptext))
        datastore.Loop();

    ImGui::Separator();

    auto musicList = instance.mMusicList;
    if (instance.mShouldUpdateMusicList)
    {
        musicList = api.MusicList();
        instance.mMusicList = musicList;
        instance.mShouldUpdateMusicList = false;
    }

    //int cursor = musicList["cursor"].get<int>();
    for (const auto& music : musicList["musicList"])
    {
        SoundComponent::Render(music);
    }
}
