#pragma once
#include <string>

class ExtrasScreen
{
public:
    static ExtrasScreen& GetInstance()
    {
        static ExtrasScreen instance;
        return instance;
    }

    static void Render();
    constexpr static std::string GetIdentifier() { return "extras"; }
};
