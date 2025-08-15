#pragma once
#include <nlohmann/json.hpp>

class MainScreen
{
public:
    static MainScreen& GetInstance()
    {
        static MainScreen instance;
        return instance;
    }

    static void Render();
    constexpr static std::string GetIdentifier() { return "main"; }

private:
    mutable nlohmann::json mMusicList{};
    mutable bool mShouldUpdateMusicList{true};
};
