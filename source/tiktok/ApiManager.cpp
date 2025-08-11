#include "ApiManager.h"
#include "../utils/File.h"

void ApiManager::Init()
{
    mClient = httplib::Client("https://www.tiktok.com");
    mClient.set_follow_location(true);

    mParams.emplace("appId", "1988");
    mParams.emplace("secUid", "MS4wLjABAAAARLr2tuCQODjwlNVKePpG90sPsMr5w1y_jCnx-cpBjpO39dMSXMaJRRKvr1taYI_Z");
    mParams.emplace("aid", "1988");

    mHeaders.emplace("User-Agent",
                     "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/138.0.0.0 Safari/537.36");
    mHeaders.emplace("Accept", "application/json, text/plain, */*");
    mHeaders.emplace("Accept-Language", "de-DE,de;q=0.9,en-DE;q=0.8,en;q=0.7,en-US;q=0.6");
    mHeaders.emplace("Cookie", mCookie);
    mHeaders.emplace("Priority", "u=1, i");
    mHeaders.emplace("Sec-Ch-Ua", R"("Not)A;Brand";v="8", "Chromium";v="138", "Google Chrome";v="138")");
    mHeaders.emplace("Sec-Ch-Ua-Mobile", "?0");
    mHeaders.emplace("Sec-Ch-Ua-Platform", "\"Windows\"");
    mHeaders.emplace("Sec-Fetch-Dest", "empty");
    mHeaders.emplace("Sec-Fetch-Mode", "cors");
    mHeaders.emplace("Sec-Fetch-Site", "same-origin");
    mHeaders.emplace("Referer", "https://www.tiktok.com/tiktokstudio/upload?from=creator_center");

    mCookie = File::ReadFile("cookie.txt");
}

nlohmann::json ApiManager::MusicList(const int cursor, const int count)
{
    std::cout << "Loading music list... [cursor=" << cursor << ";count=" << count << "]" << std::endl;
    return nlohmann::json::parse(File::ReadFile("music_list.json"));
#if 0
    assert(count < 35);

    httplib::Params params = mParams;
    params.emplace("cursor", std::to_string(cursor));
    params.emplace("count", std::to_string(count));

    auto result = mClient.Get("/api/user/collect/music_list", params, mHeaders);
    std::cout << result->body << std::endl;
    return nlohmann::json::parse(result->body);
#endif
}

std::string ApiManager::GetCookie() const
{
    return mCookie;
}

void ApiManager::SetCookie(const std::string& cookie)
{
    mCookie = cookie;
    File::WriteFile("cookie.txt", cookie);
}