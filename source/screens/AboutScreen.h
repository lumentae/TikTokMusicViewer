#pragma once
#include <string>

class AboutScreen
{
public:
    static AboutScreen& GetInstance()
    {
        static AboutScreen instance;
        return instance;
    }

    static void Render();
    constexpr static std::string GetIdentifier() { return "about"; }
};
