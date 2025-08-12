#include "ImGuiHandler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include "imgui_stdlib.h"
#include <cstdio>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

#include "tiktok/ApiManager.h"
#include "components/SoundComponent.h"
#include "utils/File.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(const int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
void ImGuiHandler::Init()
{
    const auto& instance = GetInstance();
    instance.mShouldUpdateMusicList = true;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    // GL 3.0 + GLSL 130
    const auto glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    const float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    // Valid on GLFW 3.3+ only
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(1280 * main_scale), static_cast<int>(800 * main_scale),
                                          "Tiktok Music Viewer", nullptr, nullptr);
    if (window == nullptr)
        return;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    const ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);

    // TODO: Emojis
    //io.Fonts->AddFontFromFileTTF("assets/NotoSans-Regular.ttf", 16.0f);
    //static ImFontConfig cfg;
    //cfg.MergeMode = true;
    //cfg.FontLoaderFlags |= ImGuiFreeTypeLoaderFlags_LoadColor;
    //io.Fonts->AddFontFromFileTTF("assets/", 16.0f, &cfg);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    constexpr auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        Render();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void ImGuiHandler::Render()
{
    const auto& io = ImGui::GetIO();
    const auto& instance = GetInstance();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

    ImGui::Begin("MainWindow", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoSavedSettings |
                 ImGuiWindowFlags_NoFocusOnAppearing
    );

    auto& api = ApiManager::GetInstance();
    auto& datastore = DataStore::GetInstance();
    std::string cookie = api.GetCookie();

    ImGui::Text("Cookie:");
    if (ImGui::InputText("##", &cookie))
    {
        api.SetCookie(cookie);
    }
    if (const auto looptext = datastore.GetLooping() ? "Stop Looping" : "Loop"; ImGui::Button(looptext))
        datastore.Loop();

    ImGui::Separator();

    auto musicList = instance.mMusicList;
    if (instance.mShouldUpdateMusicList)
    {
        musicList = api.MusicList();
        instance.mMusicList = musicList;
        instance.mShouldUpdateMusicList = false;
    }

    //int cursor = musicList["cursor"].get<int>();
    for (const auto& music : musicList["musicList"])
    {
        SoundComponent::Render(music);
    }

    ImGui::End();
}