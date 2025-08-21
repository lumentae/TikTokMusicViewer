#include "ApiManager.h"
#include "../utils/File.h"
#include "components/SoundComponent.h"
#include "utils/Config.h"
#include <chrono>

using namespace std::chrono_literals;

void ApiManager::Init()
{
    mClient = httplib::Client("https://www.tiktok.com");
    mClient.set_follow_location(true);

    mParams.emplace("appId", "1988");
    mParams.emplace("aid", "1988");

    mHeaders.emplace("User-Agent",
                     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
    mHeaders.emplace("Accept", "application/json, text/plain, */*");
    mHeaders.emplace("Accept-Language", "de-DE,de;q=0.9,en-DE;q=0.8,en;q=0.7,en-US;q=0.6");

    mHeaders.emplace("Priority", "u=1, i");
    mHeaders.emplace("Sec-Ch-Ua", R"("Not)A;Brand";v="8", "Chromium";v="138", "Google Chrome";v="138")");
    mHeaders.emplace("Sec-Ch-Ua-Mobile", "?0");
    mHeaders.emplace("Sec-Ch-Ua-Platform", "\"Windows\"");
    mHeaders.emplace("Sec-Fetch-Dest", "empty");
    mHeaders.emplace("Sec-Fetch-Mode", "cors");
    mHeaders.emplace("Sec-Fetch-Site", "same-origin");
    mHeaders.emplace("Referer", "https://www.tiktok.com/tiktokstudio/upload?from=creator_center");

    const auto firstMusicList = File::GetFileFromCache("musicList_0");
    if (!std::filesystem::exists(firstMusicList))
        return;

    const auto fileTime = std::filesystem::last_write_time(firstMusicList);
    const auto sysFileTime = std::chrono::clock_cast<std::chrono::system_clock>(fileTime);

    const auto currentTime = std::chrono::system_clock::now();
    if (const auto invalidationTime = currentTime + std::chrono::days{1}; sysFileTime > invalidationTime)
    {
        File::DeleteFilesInCacheWithPrefix("musicList_");
    }
}

nlohmann::json ApiManager::MusicList(const long long cursor, const long long count)
{
    std::cout << "Loading music list... [cursor=" << cursor << ";count=" << count << "]" << std::endl;
    mEnableApi = false;

    const auto cacheName = "musicList_" + std::to_string(cursor);
    const auto cacheFile = File::GetFileFromCacheByName(cacheName);
    if (std::filesystem::exists(cacheFile))
    {
        return nlohmann::json::parse(File::ReadFile(cacheFile));
    }

    assert(count <= 35);

    httplib::Params params = mParams;
    params.emplace("cursor", std::to_string(cursor));
    params.emplace("count", std::to_string(count));
    params.emplace("secUid", Config::SecUid);

    httplib::Headers headers = mHeaders;
    headers.emplace("Cookie", Config::Cookie);

    auto result = mClient.Get("/api/user/collect/music_list", params, headers);
    std::cout << result->body << std::endl;

    auto json = nlohmann::json::parse(result->body);
    if (json["status_code"].get<int>() != 0)
    {
        std::cerr << "Failed to load music list: " << json["status_msg"].get<std::string>() << std::endl;
        return json;
    }

    File::WriteFile(cacheFile, result->body);
    mEnableApi = true;
    return json;
}