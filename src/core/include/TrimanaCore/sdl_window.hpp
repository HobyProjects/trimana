#pragma once

#include <string>
#include <type_traits>

#include <SDL3/SDL.h>

#include "window.hpp"
#include "events.hpp"
#include "events_keyboard.hpp"
#include "events_mouse.hpp"
#include "events_window.hpp"

namespace TrimanaCore
{
    class UI;
    class AppEvents;

    class MainWindow
    {
    public:
        MainWindow(const std::string &title);
        ~MainWindow();
        
        SDL_Window *GetWindow() const;

    public:
        WinProperties<SDL_Window> *Properties{nullptr};
        friend class UI;
        friend class AppEvents;
    };

    class AppEvents
    {
    public:
        AppEvents(MainWindow* window) : mWindow(window){}
        ~AppEvents() = default;

        void PollEvents();
        void SetEventHandlerFunc(const EventCallbackFunc &call_back);

    public:
        WinEventsHandler<SDL_Event> Events;

    private:
        EventCallbackFunc mCallBackFunc;
        MainWindow* mWindow{nullptr};

    public:
        friend class UI;
        friend class MainWindow;
    };

}
