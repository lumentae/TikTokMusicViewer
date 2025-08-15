#pragma once
#include <string>

class SettingsScreen
{
public:
    static SettingsScreen& GetInstance()
    {
        static SettingsScreen instance;
        return instance;
    }

    static void Render();
    constexpr static std::string GetIdentifier() { return "settings"; }
};
