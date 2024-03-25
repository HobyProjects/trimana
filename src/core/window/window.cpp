#include "window.hpp"

int trimana_core::window::create(const std::string &title)
{
    if (core::core_init != INIT_FAILED)
    {
        this->title = title;

        SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display_id);
        if (mode)
        {
            window_refresh_rate = mode->refresh_rate;
            window_max_width = mode->w;
            window_max_height = mode->h;
            window_min_width = 800;
            window_min_height = 600;
            window_normal_width = mode->w - 200;
            window_normal_height = mode->h - 200;
        }
        else
        {

            LOG_WARN("DISPLAY INFO WAS NOT DETECTED BY THE APPLICATION >> {0}", SDL_GetError());
            window_max_width = NULL;
            window_max_height = NULL;
            window_min_width = NULL;
            window_min_height = NULL;
            window_normal_width = 1024;
            window_normal_height = 720;
        }

        window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        window_self = SDL_CreateWindow(this->title.c_str(), window_normal_width, window_normal_height, window_flags);
        if (window_self != nullptr)
        {
            LOG_INFO("SDL WINDOW CREATED");
            SDL_SetWindowMinimumSize(window_self, window_min_width, window_min_height);
            SDL_SetWindowMaximumSize(window_self, window_max_width, window_max_height);
            SDL_GetWindowSizeInPixels(window_self, &window_viewport_width, &window_viewport_height);
            window_context = SDL_GL_CreateContext(window_self);
            SDL_GL_SetSwapInterval(1);

            if (!window_context)
                LOG_WARN("CONTEXT FOR GL WAS NOT CREATED >> {0}", SDL_GetError());

            is_vsync_enabled = true;
            is_window_focused = true;
            is_active = true;

            return INIT_SUCCESS;
        }
        else
        {
            LOG_CRITICAL("SDL WINDOW WAS NOT CREATED >>  {0}", SDL_GetError());
            return INIT_FAILED;
        }
    }
    else
    {
        LOG_WARN("SDL IS NOT INITIALIZE. TRY AGAIN");
        return INIT_FAILED;
    }
}

void trimana_core::window::destroy()
{
    if (!window_context)
        SDL_GL_DeleteContext(window_context);

    if (window_self != nullptr)
        SDL_DestroyWindow(window_self);
}

void trimana_core::window::poll_events()
{
    SDL_WaitEvent(&window_event);

    switch (window_event.type)
    {
    /* Window events handle */
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    {
        event_window_close window_close{};
        call_back(window_close);
        break;
    }
    case SDL_EVENT_WINDOW_MOVED:
    {
        window_posx = window_event.window.data1;
        window_posy = window_event.window.data2;
        event_window_pos_change window_mov(window_posx, window_posy);
        call_back(window_mov);
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
    {
        SDL_GetWindowSizeInPixels(window_self, &window_viewport_width, &window_viewport_height);
        window_normal_width = window_event.window.data1;
        window_normal_height = window_event.window.data2;

        if ((window_max_width && window_max_height && window_min_width && window_min_height) != NULL)
        {
            if (window_normal_width <= window_max_width && window_normal_height <= window_max_height &&
                window_normal_width >= window_min_width && window_normal_height >= window_min_height)
            {
                window_state = WINDOW_STATE::NORMAL;
            }
        }

        event_window_resize window_resize(window_normal_width, window_normal_height);
        call_back(window_resize);

        event_window_viewport_resize window_vp_resize(window_viewport_width, window_viewport_height);
        call_back(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    {
        SDL_GetWindowSizeInPixels(window_self, &window_viewport_width, &window_viewport_height);
        window_normal_width = window_event.window.data1;
        window_normal_height = window_event.window.data2;

        if ((window_max_width && window_max_height && window_min_width && window_min_height) != NULL)
        {
            if (window_normal_width <= window_max_width && window_normal_height <= window_max_height &&
                window_normal_width >= window_min_width && window_normal_height >= window_min_height)
            {
                window_state = WINDOW_STATE::NORMAL;
            }
        }

        event_window_resize window_resize(window_normal_width, window_normal_height);
        call_back(window_resize);

        event_window_viewport_resize window_vp_resize(window_viewport_width, window_viewport_height);
        call_back(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_MINIMIZED:
    {
        if (is_window_focused)
            is_window_focused = !is_window_focused;

        window_state = WINDOW_STATE::MINIMIZED;
        event_window_minimize window_minimize;
        call_back(window_minimize);
        break;
    }
    case SDL_EVENT_WINDOW_MAXIMIZED:
    {
        if (!is_window_focused)
            is_window_focused = !is_window_focused;

        window_state = WINDOW_STATE::MAXIMIZED;
        event_window_maximize window_maximized;
        call_back(window_maximized);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    {
        if (is_window_focused)
            is_window_focused = !is_window_focused;

        event_window_focus_lost window_focus;
        call_back(window_focus);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    {
        if (!is_window_focused)
            is_window_focused = !is_window_focused;

        event_window_focus_gain window_foucus;
        call_back(window_foucus);
        break;
    }

    /* Keyboard events handle */
    case SDL_EVENT_KEY_DOWN:
    {
        int code = static_cast<int>(window_event.key.keysym.sym);
        static bool assign_once = true;
        static int previous_code = 0;

        if (assign_once)
        {
            previous_code = code;
            assign_once = !assign_once;
        }

        if (previous_code == code)
        {
            event_keyboard_key_hold key_hold(static_cast<int>(code));
            call_back(key_hold);
        }
        else
        {
            previous_code = code;
            event_keyboard_key_press key_pressed(static_cast<int>(code));
            call_back(key_pressed);
        }

        break;
    }
    case SDL_EVENT_KEY_UP:
    {
        SDL_Keycode code = window_event.key.keysym.sym;
        event_keyboard_key_release key_release(code);
        call_back(key_release);
        break;
    }

    /* Mouse event handle */
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        event_mouse_button_press mouse_press(window_event.button.state);
        call_back(mouse_press);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        event_mouse_button_release mouse_release(window_event.button.state);
        call_back(mouse_release);
        break;
    }
    case SDL_EVENT_MOUSE_MOTION:
    {
        event_mouse_pos_change mouse_pos((float)window_event.motion.x, (float)window_event.motion.y);
        call_back(mouse_pos);
        break;
    }
    case SDL_EVENT_MOUSE_WHEEL:
    {
        event_mouse_scroll mouse_scroll(window_event.wheel.x, window_event.wheel.y);
        call_back(mouse_scroll);
        break;
    }
    default:
    {
        break;
    }
    };
}

void trimana_core::window::set_handler(const event_callback_func &call_back)
{
    this->call_back = call_back;
}