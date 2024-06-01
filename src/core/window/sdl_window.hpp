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
    template <typename API_Window, typename API_EventHandler>
    class TRIMANA_CORE API_SDL_Window
    {
    public:
        API_SDL_Window(const std::string &title)
        {
            static_assert(std::is_same<API_Window, SDL_Window>::value, "This is not a part of SDL API");
            static_assert(std::is_same<API_EventHandler, SDL_Event>::value, "This is not a part of SDL API");

            mWinProp = new WinProp<API_Window>();
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
                LOG_WARN("DISPLAY INFO WAS NOT DETECTED BY THE APPLICATION >> {0}", SDL_GetError());
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
                    LOG_WARN("CONTEXT FOR GL WAS NOT CREATED >> {0}", SDL_GetError());

                mWinProp->Attributes.IsActive = true;
                mWinProp->Attributes.IsChildWin = false;
                mWinProp->Attributes.IsFocused = true;
                mWinProp->Attributes.IsVsyncEnbled = true;
            }

            LOG_CRITICAL("SDL WINDOW WAS NOT CREATED >>  {0}", SDL_GetError());
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

        WinProp<API_Window> *GetWindowProperties()
        {
            return mWinProp;
        }

        void PollEvents()
        {

            SDL_WaitEvent(&mWindowEventHandler);

            switch (mWindowEventHandler.type)
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
                mWinProp->Position.PosX = mWindowEventHandler.window.data1;
                mWinProp->Position.PosY = mWindowEventHandler.window.data2;
                WindowMoveEvent window_mov(window_posx, window_posy);
                mCallBackFunc(window_mov);
                break;
            }
            case SDL_EVENT_WINDOW_RESIZED:
            {
                SDL_GetWindowSizeInPixels(mWinProp->API.WindowSelf, &mWinProp->ViewportSizes.ViewportW, &mWinProp->ViewportSizes.ViewpportH);
                mWinProp->ChangedSizes.Width = window_event.window.data1;
                mWinProp->ChangedSizes.Height = window_event.window.data2;

                if ((mWinProp->FixedSizes.MaxW && mWinProp->FixedSizes.MaxH && mWinProp->FixedSizes.MinW && mWinProp->FixedSizes.MinH) != NULL)
                {
                    if (mWinProp->ChangedSizes.Width <= mWinProp->FixedSizes.MaxW && mWinProp->ChangedSizes.Height <= mWinProp->FixedSizes.MaxH &&
                        mWinProp->ChangedSizes.Width >= mWinProp->FixedSizes.MinW && mWinProp->ChangedSizes.Height >= mWinProp->FixedSizes.MinH)
                    {
                        mWinProp->Attributes.WindowStatus = WindowState::NORMAL;
                    }
                }

                WindowResizeEvent window_resize(mWinProp->ChangedSizes.Width, mWinProp->ChangedSizes.Height);
                mCallBackFunc(window_resize);

                ViewportResizeEvent window_vp_resize(mWinProp->ViewportSizes.ViewportW, mWinProp->ViewportSizes.ViewpportH);
                mCallBackFunc(window_vp_resize);
                break;
            }
            case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
            {
                SDL_GetWindowSizeInPixels(mWinProp->API.WindowSelf, &mWinProp->ViewportSizes.ViewportW, &mWinProp->ViewportSizes.ViewpportH);
                mWinProp->ChangedSizes.Width = window_event.window.data1;
                mWinProp->ChangedSizes.Height = window_event.window.data2;

                if ((mWinProp->FixedSizes.MaxW && mWinProp->FixedSizes.MaxH && mWinProp->FixedSizes.MinW && mWinProp->FixedSizes.MinH) != NULL)
                {
                    if (mWinProp->ChangedSizes.Width <= mWinProp->FixedSizes.MaxW && mWinProp->ChangedSizes.Height <= mWinProp->FixedSizes.MaxH &&
                        mWinProp->ChangedSizes.Width >= mWinProp->FixedSizes.MinW && mWinProp->ChangedSizes.Height >= mWinProp->FixedSizes.MinH)
                    {
                        mWinProp->Attributes.WindowStatus = WindowState::NORMAL;
                    }
                }

                WindowResizeEvent window_resize(mWinProp->ChangedSizes.Width, mWinProp->ChangedSizes.Height);
                mCallBackFunc(window_resize);

                ViewportResizeEvent window_vp_resize(mWinProp->ViewportSizes.ViewportW, mWinProp->ViewportSizes.ViewpportH);
                mCallBackFunc(window_vp_resize);
                break;
            }
            case SDL_EVENT_WINDOW_MINIMIZED:
            {
                if (mWinProp->Attributes.IsFocused)
                    mWinProp->Attributes.IsFocused = !mWinProp->Attributes.IsFocused;

                mWinProp->Attributes.WindowStatus = WindowState::MINIMIZED;
                WindowMinimizeEvent window_minimize;
                mCallBackFunc(window_minimize);
                break;
            }
            case SDL_EVENT_WINDOW_MAXIMIZED:
            {
                if (!mWinProp->Attributes.IsFocused)
                    mWinProp->Attributes.IsFocused = !mWinProp->Attributes.IsFocused;

                mWinProp->Attributes.WindowStatus = WindowState::MAXIMIZED;
                WindowMaximizeEvent window_maximized;
                mCallBackFunc(window_maximized);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            {
                if (mWinProp->Attributes.IsFocused)
                    mWinProp->Attributes.IsFocused = !mWinProp->Attributes.IsFocused;

                WindowFocusLostEvent window_focus;
                mCallBackFunc(window_focus);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            {
                if (!mWinProp->Attributes.IsFocused)
                    mWinProp->Attributes.IsFocused = !mWinProp->Attributes.IsFocused;

                WindowFocusGainEvent window_foucus;
                mCallBackFunc(window_foucus);
                break;
            }

            /* Keyboard Events handle */
            case SDL_EVENT_KEY_DOWN:
            {
                int code = static_cast<int>(mWindowEventHandler.key.keysym.sym);
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
                SDL_Keycode code = mWindowEventHandler.key.keysym.sym;
                KeyboardKeyReleaseEvent key_release(code);
                mCallBackFunc(key_release);
                break;
            }

            /* Mouse event handle */
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseButtonPressEvent mouse_press(mWindowEventHandler.button.state);
                mCallBackFunc(mouse_press);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButtonReleaseEvent mouse_release(mWindowEventHandler.button.state);
                mCallBackFunc(mouse_release);
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {
                MousePosChangeEvent mouse_pos((float)mWindowEventHandler.motion.x, (float)mWindowEventHandler.motion.y);
                mCallBackFunc(mouse_pos);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:
            {
                MouseScrollEvent mouse_scroll(mWindowEventHandler.wheel.x, mWindowEventHandler.wheel.y);
                mCallBackFunc(mouse_scroll);
                break;
            }
            default:
            {
                break;
            }
            };
        }

        void SetEventHandlerFunc(const EventCallbackFunc &call_back)
        {
            mCallBackFunc = call_back;
        }

    private:
        WinProp<API_Window> *mWinProp{nullptr};
        WinEventsHandler<API_EventHandler> mWindowEventHandler;
        EventCallbackFunc mCallBackFunc;
    };
}
