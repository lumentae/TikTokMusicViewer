#pragma once
#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui
{
    inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
    {
        return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    static void HighlightRect(const ImRect& rect) {
        ImDrawList* draw_list = GetForegroundDrawList();

        // Fullscreen dark overlay
        const ImVec2 screen_min = GetMainViewport()->Pos;
        const ImVec2 screen_max = screen_min + GetMainViewport()->Size;
        draw_list->AddRectFilled(screen_min, screen_max, IM_COL32(0, 0, 0, 180));

        // Highlight border
        draw_list->AddRect(rect.Min, rect.Max, IM_COL32(255, 255, 0, 255), 5.0f, 0, 3.0f);
    }

    static void HighlightItem() {
        const ImRect rect(GetItemRectMin(), GetItemRectMax());
        HighlightRect(rect);
    }
}
