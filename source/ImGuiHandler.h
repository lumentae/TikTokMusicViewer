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

    mutable std::string mCurrentScreen{"main"};

private:
    ImGuiHandler() = default;
    ~ImGuiHandler() = default;
};

#define DEFINE_SCREEN(name, description) \
    if (ImGui::Button(#name)) \
    { \
        instance.mCurrentScreen = name##Screen::GetIdentifier(); \
    } \
    EXPLANATION(name, description);

#define HANDLER_SCREEN(name) \
    if (instance.mCurrentScreen == name##Screen::GetIdentifier()) \
    { \
        name##Screen::Render(); \
    }