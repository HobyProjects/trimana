#pragma once

#include <string>
#include <type_traits>

#include <SDL3/SDL.h>

#include "window.hpp"
#include "events/events.hpp"
#include "events/events_keyboard.hpp"
#include "events/events_mouse.hpp"
#include "events/events_window.hpp"
#include "utils/platform.hpp"

namespace TrimanaCore
{
    template <typename API_EventHandler>
    class TRIMANA_CORE API_SDL_Event
    {
    public:
        API_SDL_Event() = default;
        ~API_SDL_Event() = default;

        template <typename API_Window>
        void PollEvents(API_Window *win_prop)
        {
            static_assert(std::is_same<API_EventHandler, SDL_Event>::value, "This is not a part of SDL API");
            SDL_WaitEvent(&mEvents);

            switch (mEvents.type)
            {

            /* Window Events handle */
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                WindowCloseEvent window_close{};
                mCallBackFunc(window_close);
                break;
            }
            case SDL_EVENT_WINDOW_MOVED:
            {
                win_prop->Position.PosX = mEvents.window.data1;
                win_prop->Position.PosY = mEvents.window.data2;
                WindowMoveEvent window_mov(window_posx, window_posy);
                mCallBackFunc(window_mov);
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                SDL_GetWindowSizeInPixels(win_prop->API.WindowSelf, &win_prop->ViewportSizes.ViewportW, &win_prop->ViewportSizes.ViewpportH);
                win_prop->ChangedSizes.Width = mEvents.window.data1;
                win_prop->ChangedSizes.Height = mEvents.window.data2;

                if ((win_prop->FixedSizes.MaxW && win_prop->FixedSizes.MaxH && win_prop->FixedSizes.MinW && win_prop->FixedSizes.MinH) != NULL)
                {
                    if (win_prop->ChangedSizes.Width <= win_prop->FixedSizes.MaxW && win_prop->ChangedSizes.Height <= win_prop->FixedSizes.MaxH &&
                        win_prop->ChangedSizes.Width >= win_prop->FixedSizes.MinW && win_prop->ChangedSizes.Height >= win_prop->FixedSizes.MinH)
                    {
                        win_prop->Attributes.WindowStatus = WindowState::NORMAL;
                    }
                }

                WindowResizeEvent window_resize(win_prop->ChangedSizes.Width, win_prop->ChangedSizes.Height);
                mCallBackFunc(window_resize);

                ViewportResizeEvent window_vp_resize(win_prop->ViewportSizes.ViewportW, win_prop->ViewportSizes.ViewpportH);
                mCallBackFunc(window_vp_resize);
                break;
            }
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
            {
                SDL_GetWindowSizeInPixels(win_prop->API.WindowSelf, &win_prop->ViewportSizes.ViewportW, &win_prop->ViewportSizes.ViewpportH);
                win_prop->ChangedSizes.Width = mEvents.window.data1;
                win_prop->ChangedSizes.Height = mEvents.window.data2;

                if ((win_prop->FixedSizes.MaxW && win_prop->FixedSizes.MaxH && win_prop->FixedSizes.MinW && win_prop->FixedSizes.MinH) != NULL)
                {
                    if (win_prop->ChangedSizes.Width <= win_prop->FixedSizes.MaxW && win_prop->ChangedSizes.Height <= win_prop->FixedSizes.MaxH &&
                        win_prop->ChangedSizes.Width >= win_prop->FixedSizes.MinW && win_prop->ChangedSizes.Height >= win_prop->FixedSizes.MinH)
                    {
                        win_prop->Attributes.WindowStatus = WindowState::NORMAL;
                    }
                }

                WindowResizeEvent window_resize(win_prop->ChangedSizes.Width, win_prop->ChangedSizes.Height);
                mCallBackFunc(window_resize);

                ViewportResizeEvent window_vp_resize(win_prop->ViewportSizes.ViewportW, win_prop->ViewportSizes.ViewpportH);
                mCallBackFunc(window_vp_resize);
                break;
            }
            case SDL_EVENT_WINDOW_MINIMIZED:
            {
                if (win_prop->Attributes.IsFocused)
                    win_prop->Attributes.IsFocused = !win_prop->Attributes.IsFocused;

                win_prop->Attributes.WindowStatus = WindowState::MINIMIZED;
                WindowMinimizeEvent window_minimize;
                mCallBackFunc(window_minimize);
                break;
            }
            case SDL_EVENT_WINDOW_MAXIMIZED:
            {
                if (!win_prop->Attributes.IsFocused)
                    win_prop->Attributes.IsFocused = !win_prop->Attributes.IsFocused;

                win_prop->Attributes.WindowStatus = WindowState::MAXIMIZED;
                WindowMaximizeEvent window_maximized;
                mCallBackFunc(window_maximized);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            {
                if (win_prop->Attributes.IsFocused)
                    win_prop->Attributes.IsFocused = !win_prop->Attributes.IsFocused;

                WindowFocusLostEvent window_focus;
                mCallBackFunc(window_focus);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            {
                if (!win_prop->Attributes.IsFocused)
                    win_prop->Attributes.IsFocused = !win_prop->Attributes.IsFocused;

                WindowFocusGainEvent window_foucus;
                mCallBackFunc(window_foucus);
                break;
            }

            /* Keyboard Events handle */
            case SDL_EVENT_KEY_DOWN:
            {
                int code = static_cast<int>(mEvents.key.keysym.sym);
                static bool assign_once = true;
                static int previous_code = 0;

                if (assign_once)
                {
                    previous_code = code;
                    assign_once = !assign_once;
                }

                if (previous_code == code)
                {
                    KeyboardKeyHold key_hold(static_cast<int>(code));
                    mCallBackFunc(key_hold);
                }
                else
                {
                    previous_code = code;
                    KeyboardKeyPressEvent key_pressed(static_cast<int>(code));
                    mCallBackFunc(key_pressed);
                }

                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                SDL_Keycode code = mEvents.key.keysym.sym;
                KeyboardKeyReleaseEvent key_release(code);
                mCallBackFunc(key_release);
                break;
            }

            /* Mouse event handle */
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseButtonPressEvent mouse_press(mEvents.button.state);
                mCallBackFunc(mouse_press);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButtonReleaseEvent mouse_release(mEvents.button.state);
                mCallBackFunc(mouse_release);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                MousePosChangeEvent mouse_pos((float)mEvents.motion.x, (float)mEvents.motion.y);
                mCallBackFunc(mouse_pos);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:
            {
                MouseScrollEvent mouse_scroll(mEvents.wheel.x, mEvents.wheel.y);
                mCallBackFunc(mouse_scroll);
                break;
            }
            default:
            {
                break;
            }
            };
        }

        static void SetEventHandlerFunc(const EventCallbackFunc &call_back)
        {
            mCallBackFunc = call_back;
        }

    private:
        WinEventsHandler<API_EventHandler> mEvents;
        EventCallbackFunc mCallBackFunc;
    };

    template <typename API_Window>
    class TRIMANA_CORE API_SDL_Window
    {
    public:
        API_SDL_Window(const std::string &title)
        {
            static_assert(std::is_same<API_Window, SDL_Window>::value, "This is not a part of SDL API");

            mWinProp = std::make_shared<WinProp<API_Window>>();
            SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
            const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display_id);
            if (mode != nullptr)
            {
                mWinProp->Attributes.WindowRefreshRate = mode->refresh_rate;
                mWinProp->FixedSizes.MaxW = mode->w;
                mWinProp->FixedSizes.MaxH = mode->h;
                mWinProp->FixedSizes.MinH = 720;
                mWinProp->FixedSizes.MinW = 1024;
            }
            else
            {
                TRIMANA_CORE_WARN("DISPLAY INFO WAS NOT DETECTED BY THE APPLICATION >> {0}", SDL_GetError());
                mWinProp->FixedSizes.MaxW = NULL;
                mWinProp->FixedSizes.MaxH = NULL;
                mWinProp->FixedSizes.MinH = NULL;
                mWinProp->FixedSizes.MinW = NULL;
            }

            mWinProp->Attributes.WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
            mWinProp->API.WindowSelf = SDL_CreateWindow(this->title.c_str(), mWinProp->FixedSizes.MinW, mWinProp->FixedSizes.MinH, mWinProp->Attributes.WindowFlags);
            if (mWinProp->API.WindowSelf != nullptr)
            {
                LOG_INFO("SDL WINDOW CREATED");
                SDL_SetWindowMinimumSize(mWinProp->API.WindowSelf, mWinProp->FixedSizes.MinW, mWinProp->FixedSizes.MinH);
                SDL_SetWindowMaximumSize(mWinProp->API.WindowSelf, mWinProp->FixedSizes.MaxW, mWinProp->FixedSizes.MaxH);
                SDL_GetWindowSizeInPixels(mWinProp->API.WindowSelfow_self, &window_viewport_width, &window_viewport_height);
                mWinProp->API.WindowContext = (SDL_GLContext)SDL_GL_CreateContext(mWinProp->API.WindowSelf);
                SDL_GL_SetSwapInterval(1);

                if (!mWinProp->API.WindowContext)
                    TRIMANA_CORE_WARN("CONTEXT FOR GL WAS NOT CREATED >> {0}", SDL_GetError());

                mWinProp->Attributes.IsActive = true;
                mWinProp->Attributes.IsChildWin = false;
                mWinProp->Attributes.IsFocused = true;
                mWinProp->Attributes.IsVsyncEnbled = true;
            }

            LOG_CORE_CRITICAL("SDL WINDOW WAS NOT CREATED >>  {0}", SDL_GetError());
        }

        ~API_SDL_Window()
        {
            if (!mWinProp->API.WindowContext)
                SDL_GL_DeleteContext(window_context);

            if (mWinProp->API.WindowSelf != nullptr)
                SDL_DestroyWindow(window_self);
        }

        API_Window *GetWindow() const
        {
            return mWinProp->API.WindowSelf;
        }

        std::shared_ptr<WinProp<API_Window>> *GetWindowProperties()
        {
            return mWinProp;
        }

    private:
        std::shared_ptr<WinProp<API_Window>> mWinProp{nullptr};
    };
}
