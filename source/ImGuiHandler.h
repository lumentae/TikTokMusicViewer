#pragma once
#include <nlohmann/json.hpp>

class ImGuiHandler
{
public:
    static ImGuiHandler& GetInstance()
    {
        static ImGuiHandler instance;
        return instance;
    }

    static void Init();
    static void Render();

private:
    ImGuiHandler() = default;
    ~ImGuiHandler() = default;

    mutable std::string mCurrentScreen{"main"};
};
