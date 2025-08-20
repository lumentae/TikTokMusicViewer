#pragma once
#include <complex.h>

#include "../tiktok/ApiManager.h"

class SoundComponent
{
public:
    static void Render(nlohmann::basic_json<> music, int index);
};