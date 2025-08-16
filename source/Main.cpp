#include <iostream>

#include "ImGuiHandler.h"
#include "tiktok/ApiManager.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "utils/Config.h"
#include "utils/DataStore.h"

#define ENABLE_EXCEPTION_HANDLER

int main()
{
    std::cout << "Tiktok Music Viewer" << std::endl;
#ifdef ENABLE_EXCEPTION_HANDLER
    try
    {
#endif
        Config::Init();
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
