#pragma once
#include <cstdint>
#include <imgui.h>
#include <string>
#include <vector>

class Text
{
public:
    static bool IsEmoji(const uint32_t cp)
    {
        return (cp >= 0x1F300 && cp <= 0x1FAFF) ||
            (cp >= 0x2600 && cp <= 0x26FF) ||
            (cp >= 0x2700 && cp <= 0x27BF);
    }

    static std::vector<ImWchar> CollectEmojiCodepoints(const std::string& text)
    {
        std::vector<ImWchar> cps;
        auto string = reinterpret_cast<const unsigned char*>(text.c_str());

        while (*string)
        {
            // magic
            uint32_t codepoint;
            if (*string < 0x80) { codepoint = *string++; }
            else if ((*string & 0xE0) == 0xC0)
            {
                codepoint = ((*string & 0x1F) << 6) | (string[1] & 0x3F);
                string += 2;
            }
            else if ((*string & 0xF0) == 0xE0)
            {
                codepoint = ((*string & 0x0F) << 12) | ((string[1] & 0x3F) << 6) | (string[2] & 0x3F);
                string += 3;
            }
            else if ((*string & 0xF8) == 0xF0)
            {
                codepoint = ((*string & 0x07) << 18) | ((string[1] & 0x3F) << 12) | ((string[2] & 0x3F) << 6) | (string[
                    3] & 0x3F);
                string += 4;
            }
            else
            {
                string++;
                continue;
            }

            if (IsEmoji(codepoint))
                cps.push_back(static_cast<ImWchar>(codepoint));
        }

        cps.push_back(0);
        return cps;
    }
};
