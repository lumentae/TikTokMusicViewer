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

    mutable bool mTutorial;

private:
    ImGuiHandler() = default;
    ~ImGuiHandler() = default;

    mutable std::string mCurrentScreen{"main"};
};

#define DEFINE_SCREEN(name) \
    if (ImGui::Button(#name)) \
    { \
        instance.mCurrentScreen = name##Screen::GetIdentifier(); \
    }

#define HANDLER_SCREEN(name) \
    if (instance.mCurrentScreen == name##Screen::GetIdentifier()) \
    { \
        name##Screen::Render(); \
    }