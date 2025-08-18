#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

#include "Config.h"
#include "ImGuiHandler.h"
#include "screens/MainScreen.h"
#include "screens/ExtrasScreen.h"
#include "screens/SettingsScreen.h"

enum TutorialState
{
    None = -1,
    Main,
    Refresh,
    Loop,
    Pagination,
    Volume,
    Settings,
    Cookie,
    SecUid,
    AutoCredentials,
    Extras,
    RestartTutorial,
    DeleteCache,
    DeleteAllData,
};

namespace ImGui
{
    inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
    {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    static void HighlightRect(const ImRect& rect) {
        ImDrawList* draw_list = GetForegroundDrawList();

        constexpr float padding = 10.0f;
        // Inflate rect by padding
        const ImRect padded(
            ImVec2(rect.Min.x - padding, rect.Min.y - padding),
            ImVec2(rect.Max.x + padding, rect.Max.y + padding)
        );

        // Optional: border
        draw_list->AddRect(padded.Min, padded.Max, IM_COL32(255, 255, 0, 255), 5.0f, 0, 3.0f);
    }

    static void HighlightItem() {
        const ImRect rect(GetItemRectMin(), GetItemRectMax());
        HighlightRect(rect);
    }

    static void Explanation(const std::string& text) {
        HighlightItem();

        const ImVec2 max = GetItemRectMax();
        SetNextWindowPos(max + ImVec2(20, 0));
        SetNextWindowSize(ImVec2(400, 0.0f));

        Begin("Explanation", nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar);

        TextWrapped("%s", text.c_str());

        if (Button("Next")) {
            switch (Config::Tutorial)
            {
            case Main:
                ImGuiHandler::GetInstance().mCurrentScreen = MainScreen::GetIdentifier();
                break;
            case Settings:
                ImGuiHandler::GetInstance().mCurrentScreen = SettingsScreen::GetIdentifier();
                break;
            case Extras:
                ImGuiHandler::GetInstance().mCurrentScreen = ExtrasScreen::GetIdentifier();
                break;
            }
            CONFIG_SET_VALUE(Tutorial, Config::Tutorial + 1);
        }

        End();
    }
}

#define EXPLANATION(state, text) \
    if (Config::Tutorial == TutorialState::state) \
        ImGui::Explanation(text)