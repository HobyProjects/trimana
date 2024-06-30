#include "sdl_gui.hpp"

TrimanaCore::UI::UI(MainWindow *win, AppEvents *events, UIColor color)
{
    mWin = win;
    mWinEvent = events;

    if (color == UIColor::UI_DARK)
    {
        ImGUI_DarkColors();
        mUIColor = color;
    }
    else
    {
        ImGui::StyleColorsLight();
        mUIColor = color;
    }

    std::thread ui_worker(&UI::SceneCopy, this);
    mUIWorker = std::move(ui_worker);
}

TrimanaCore::UI::~UI()
{
    if (mUIWorker.joinable())
        mUIWorker.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void TrimanaCore::UI::Begin()
{
    mFrameBuffer->Bind();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void TrimanaCore::UI::End()
{
     ImGui::Begin("Scene");
    {
        ImGui::BeginChild("View");
        ImVec2 viewport_size = ImGui::GetContentRegionAvail();
        if (mFrameBuffer->GetFrameSize().x != viewport_size.x && mFrameBuffer->GetFrameSize().y!= viewport_size.y)
        {
            mFrameBuffer->FrameResize(glm::vec2(viewport_size.x, viewport_size.y));
        }
        ImGui::Image((ImTextureID)mFrameBuffer->GetFrameTexture(), viewport_size, ImVec2{1, 0}, ImVec2{0, 1});
    }

    ImGui::EndChild();
    ImGui::End();

    mFrameBuffer->UnBind();

    ImGui::Render();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

void TrimanaCore::UI::ImGUI_DarkColors()
{
    ImGui::StyleColorsDark();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 4.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

void TrimanaCore::UI::PollEvents()
{
    ImGui_ImplSDL3_ProcessEvent(&mWinEvent->Events.WindowEvents);
}