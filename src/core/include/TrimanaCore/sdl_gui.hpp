#pragma once

#include <thread>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "gl.hpp"
#include "sdl_window.hpp"

namespace TrimanaCore
{
    enum class UIColor
    {
        UI_LIGHT,
        UI_DARK
    };

    class UI
    {
    public:
        UI(MainWindow *win, AppEvents *events, UIColor color);
        UI(UI &other) = delete;
        UI &operator=(UI &other) = delete;
        ~UI();

        void Begin();
        void End();
        void PollEvents();
        void SceneCopy();

    private:
        void ImGUI_DarkColors();

    private:
        UIColor mUIColor{UIColor::UI_DARK};
        MainWindow *mWin{nullptr};
        AppEvents *mWinEvent{nullptr};
        FrameBuffer* mFrameBuffer{nullptr};

        std::thread mUIWorker;
    };
}
