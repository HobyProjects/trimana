#pragma once

#include <core/core.hpp>
#include <core/sdl_window.hpp>
#include <core/logger.hpp>

namespace TrimanaEngine
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        void Start();
        void Stop();
        void OnEvents(TrimanaCore::Events& e);

    private:
        bool WindowCloseEvent(const TrimanaCore::Events& e);

    private:
        TrimanaCore::API_Window<SDL_Window>* mWindow{nullptr};
        TrimanaCore::API_Event<SDL_Event>* mEvent{nullptr};
        TrimanaCore::Core* mCore{nullptr};
    };
}
