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
    static void AutoCredentials(const std::string& input);
    constexpr static std::string GetIdentifier() { return "settings"; }
};
