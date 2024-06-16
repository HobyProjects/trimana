#pragma once

#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl3.h>

#include "renders/GL/gl.hpp"
#include "window/window.hpp"

namespace TrimanaCore
{
    enum class UIColor
    {
        UI_LIGHT,
        UI_DARK
    };

    template <class API_Window, class API_EventHandler>
    class ImGUI
    {
    public:
        ImGUI(const std::string &glsl_ver, std::shared_ptr<WinProperties<API_Window>> win, WinEventsHandler<API_EventHandler> events, UIColor color)
        {
            mWin = win;

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
            io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
            // io.ConfigViewportsNoAutoMerge = true;
            // io.ConfigViewportsNoTaskBarIcon = true;

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

            ImGui_ImplSDL3_InitForOpenGL(mWin->Win->WindowSelf, (SDL_GLContext)mWin->Win->WindowContext);
            ImGui_ImplOpenGL3_Init(glsl_ver.c_str());
            ImGui_ImplSDL3_ProcessEvent(&events->GetEventHandler());

            glGenFramebuffers(1, &mFrameBufferObject);
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
            glGenTextures(1, &mFrameTexture);
            glBindTexture(GL_TEXTURE_2D, mFrameTexture);

            glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, mWin->ViewportSizes.ViewportW, mWin->ViewportSizes.ViewpportH, NULL, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, NULL);
            glBindTexture(GL_TEXTURE_2D, NULL);

            glGenTextures(1, &mFrameTextureDepth);
            glBindTexture(GL_TEXTURE_2D, mFrameTextureDepth);

            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mWin->ViewportSizes.ViewportW, mWin->ViewportSizes.ViewpportH);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mFrameTextureDepth, NULL);
            glBindTexture(GL_TEXTURE_2D, NULL);

            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(mFrameTexture, buffers);
            glBindFramebuffer(GL_FRAMEBUFFER, NULL);
        }

        ~ImGUI()
        {
            if (FrameBufferObject != NULL)
            {
                glDeleteFramebuffers(GL_FRAMEBUFFER, &FrameBufferObject);
                glDeleteTextures(1, &FrameTexture);
                glDeleteTextures(1, &FrameTextureDepth);

                FrameBufferObject = NULL;
                FrameTexture = NULL;
                FrameTextureDepth = NULL;

                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplSDL3_Shutdown();
                ImGui::DestroyContext();
            }
        }

        void Begin()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
        }

        void SceneCopy()
        {
            ImGui::Begin("Scene View");
            ImVec2 viewport_size;
            viewport_size.x = (float)mWin->ViewportSizes.ViewportW;
            viewport_size.y = (float)mWin->ViewportSizes.ViewportH;
            ImGui::Image(reinterpret_cast<ImTextureID>(mFrameBufferObject), viewport_size);
            ImGui::End();
        }

        void End()
        {
            // Viewport Start
            glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);

            glBindTexture(GL_TEXTURE_2D, mFrameTexture);
            glTexImage2D(GL_TEXTURE_2D, NULL, GL_RGB, mWin->ViewportSizes.ViewportW, mWin->ViewportSizes.ViewportH, NULL, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFrameTexture, NULL);
            glBindTexture(GL_TEXTURE_2D, NULL);

            glBindTexture(GL_TEXTURE_2D, mFrameTextureDepth);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mWin->ViewportSizes.ViewportW, mWin->ViewportSizes.ViewportH);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mFrameTextureDepth, NULL);
            glBindTexture(GL_TEXTURE_2D, NULL);

            GLenum buffers[4] = {GL_COLOR_ATTACHMENT0};
            glDrawBuffers(mFrameTexture, buffers);

            glViewport(NULL, NULL, mWin->ViewportSizes.ViewportW, mWin->ViewportSizes.ViewportH);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, GL_UNBIND);
        }

        void Render()
        {
            ImGuiIO &io = ImGui::GetIO();
            (void)io;
            ImGui::Render();
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

    private:
        void ImGUI_DarkColors()
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

    private:
        FrameBufferLoc mFrameBufferObject{NULL};
        TextureLocation mFrameTexture{NULL};
        TextureLocation mFrameTextureDepth{NULL};
        std::weak_ptr<WinProperties<API_Window>> mWin{nullptr};
        UIColor mUIColor;
    };
}