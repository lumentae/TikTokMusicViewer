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

    nlohmann::json GetMusicList() const
    {
        return mMusicList;
    }

private:
    mutable nlohmann::json mMusicList{};
    mutable bool mShouldUpdateMusicList{true};
    mutable std::vector<long long> mCursors{0};
    mutable int mPage{0};
};
