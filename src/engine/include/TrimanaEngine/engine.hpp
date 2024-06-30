#pragma once

#include <thread>

#include <TrimanaCore/core.hpp>
#include <TrimanaCore/sdl_window.hpp>
#include <TrimanaCore/sdl_gui.hpp>

namespace TrimanaEngine
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Start();
        void Stop();
        void PollEvents();
        void OnEvents(TrimanaCore::Event& e);

    private:
        bool WindowCloseEvent(const TrimanaCore::Event& e);

    private:
        TrimanaCore::Core* mCore{nullptr};
        TrimanaCore::MainWindow* mMainWindow{nullptr};
        TrimanaCore::AppEvents* mEngineEvent{nullptr};
        TrimanaCore::UI* mUI{nullptr};
        std::thread mEventsHandler;
    };
}
