#include <iostream>

#include "ImGuiHandler.h"
#include "tiktok/ApiManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_MP4
#include "miniaudio.h"
#include "utils/DataStore.h"

#define ENABLE_EXCEPTION_HANDLER

int main()
{
    std::cout << "Tiktok Music Viewer" << std::endl;
#ifdef ENABLE_EXCEPTION_HANDLER
    try
    {
#endif
        DataStore::GetInstance().Init();
        ApiManager::GetInstance().Init();
        ImGuiHandler::Init();
#ifdef ENABLE_EXCEPTION_HANDLER
    }
    catch (std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
    }
#endif
    return 0;
}
