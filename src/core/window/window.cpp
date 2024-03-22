#include "window.hpp"

trimana_core::window_properties *trimana_core::window_create(const std::string &window_title)
{
    uint32_t sdl_init_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER;
    if (SDL_Init(sdl_init_flags) == NULL)
    {
        LOG_INFO("INITIALIZING SDL.....");
        trimana_core::window_properties *properties = new trimana_core::window_properties();
        properties->title = window_title;

        SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
        const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display_id);
        if (mode)
        {
            properties->window_refresh_rate = mode->refresh_rate;
            properties->window_max_width = mode->w;
            properties->window_max_height = mode->h;
            properties->window_min_width = 800;
            properties->window_min_height = 600;
            properties->window_normal_width = mode->w - 200;
            properties->window_normal_height = mode->h - 200;
        }
        else
        {

            LOG_WARN("DISPLAY INFO WAS NOT DETECTED BY THE APPLICATION >> {0}", SDL_GetError());
            properties->window_max_width = NULL;
            properties->window_max_height = NULL;
            properties->window_min_width = NULL;
            properties->window_min_height = NULL;
            properties->window_normal_width = 1024;
            properties->window_normal_height = 720;
        }

        properties->window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        properties->window = SDL_CreateWindow(window_title.c_str(), properties->window_normal_width, properties->window_normal_height, properties->window_flags);
        if (properties->window != nullptr)
        {
            LOG_INFO("SDL WINDOW CREATED");
            SDL_SetWindowMinimumSize(properties->window, properties->window_min_width, properties->window_min_height);
            SDL_SetWindowMaximumSize(properties->window, properties->window_max_width, properties->window_max_height);
            SDL_GetWindowSizeInPixels(properties->window, &properties->window_viewport_width, &properties->window_viewport_height);
            properties->window_context = SDL_GL_CreateContext(properties->window);
            SDL_GL_SetSwapInterval(1);

            if (!properties->window_context)
                LOG_WARN("CONTEXT FOR GL WAS NOT CREATED >> {0}", SDL_GetError());

            properties->window_vsync_enabled = true;
            properties->is_window_focused = true;
            properties->is_active = true;

            return properties;
        }
        else
        {
            LOG_CRITICAL("SDL WINDOW WAS NOT CREATED >>  {0}", SDL_GetError());
            return nullptr;
        }
    }
    else
    {
        LOG_CRITICAL("SOMETHING WENT WRONG WHILE INITIALIZING SDL >> {0}", SDL_GetError());
        return nullptr;
    }

    return nullptr;
}

void trimana_core::window_destroy(trimana_core::window_properties *properties)
{
    if (properties != nullptr)
    {
        SDL_GL_DeleteContext(properties->window_context);
        SDL_DestroyWindow(properties->window);

        properties->window_context = nullptr;
        properties->window = nullptr;
    }

    SDL_Quit();
}

void trimana_core::get_window_events(trimana_core::window_properties *properties)
{
    SDL_WaitEvent(&properties->window_event);

    switch (properties->window_event.type)
    {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    {
        event_window_close window_close{};
        properties->call_back(window_close);
        break;
    }
    case SDL_EVENT_WINDOW_MOVED:
    {
        properties->window_posx = properties->window_event.window.data1;
        properties->window_posy = properties->window_event.window.data2;
        event_window_pos_change window_mov(properties->window_posx, properties->window_posy);
        properties->call_back(window_mov);
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
    {
        SDL_GetWindowSizeInPixels(properties->window, &properties->window_viewport_width, &properties->window_viewport_height);
        properties->window_normal_width = properties->window_event.window.data1;
        properties->window_normal_height = properties->window_event.window.data2;

        if ((properties->window_max_width && properties->window_max_height && properties->window_min_width && properties->window_min_height) != NULL)
        {
            if (properties->window_normal_width <= properties->window_max_width && properties->window_normal_height <= properties->window_max_height &&
                properties->window_normal_width >= properties->window_min_width && properties->window_normal_height >= properties->window_min_height)
            {
                properties->State = WINDOW_STATE_NORMAL;
            }
        }

        event_window_resize window_resize(properties->window_normal_width, properties->window_normal_height);
        properties->call_back(window_resize);

        event_window_viewport_resize window_vp_resize(properties->window_viewport_width, properties->window_viewport_height);
        properties->call_back(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    {
        SDL_GetWindowSizeInPixels(properties->window, &properties->window_viewport_width, &properties->window_viewport_height);
        properties->window_normal_width = properties->window_event.window.data1;
        properties->window_normal_height = properties->window_event.window.data2;

        if ((properties->window_max_width && properties->window_max_height && properties->window_min_width && properties->window_min_height) != NULL)
        {
            if (properties->window_normal_width <= properties->window_max_width && properties->window_normal_height <= properties->window_max_height &&
                properties->window_normal_width >= properties->window_min_width && properties->window_normal_height >= properties->window_min_height)
            {
                properties->State = WINDOW_STATE_NORMAL;
            }
        }

        event_window_resize window_resize(properties->window_normal_width, properties->window_normal_height);
        properties->call_back(window_resize);

        event_window_viewport_resize window_vp_resize(properties->window_viewport_width, properties->window_viewport_height);
        properties->call_back(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_MINIMIZED:
    {
        if(properties->is_window_focused)
            properties->is_window_focused = !properties->is_window_focused;

        properties->window_state = WINDOW_STATE_MINIMIZED;
        event_window_minimize window_minimize;
        properties->call_back(window_minimize);
        break;
    }
    case SDL_EVENT_WINDOW_MAXIMIZED:
    {
        if(!properties->is_window_focused)
            properties->is_window_focused = !properties->is_window_focused;

        properties->window_state = WINDOW_STATE_MAXIMIZED;
        event_window_maximize window_maximized;
        properties->call_back(window_maximized);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    {
        if(properties->is_window_focused)
            properties->is_window_focused = !properties->is_window_focused;
        
        event_window_focus_lost window_focus;
        properties->call_back(window_focus);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    {
        if(!properties->is_window_focused)
            properties->is_window_focused = !properties->is_window_focused;

        event_window_focus_gain window_foucus;
        properties->call_back(window_foucus);
        break;
    }
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_KeyCode code = properties->window_event.key.keysym.sym;

        static bool assign_once = true;
        static SDL_KeyCode previous_code = 0;
        if(assign_once){
            previous_code = code;
            assign_once = !assign_once;
        }

        if(previous_code == code){
            event_keyboard_key_hold key_hold(static_cast<int32_t>(code));
            properties->call_back(key_hold);
        }
        else{
            previous_code = code;
            event_keyboard_key_press key_pressed(static_cast<int32_t>(code));
            properties->call_back(key_pressed);
        }

        break;
    }
    case SDL_EVENT_KEY_UP:
    {
        SDL_Keycode code = properties->window_event.key.keysym.sym;
        event_keyboard_key_release key_release(code);
        properties->call_back(key_release);
        break;
    }
    
    

    };
}