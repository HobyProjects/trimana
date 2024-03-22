#pragma once

#include <string>
#include <SDL3/SDL.h>

#include "events\events_window.hpp"
#include "events\events_keyboard.hpp"
#include "events\events_mouse.hpp"

#define WINDOW_STATE_NORMAL     0x1
#define WINDOW_STATE_MAXIMIZED  0x2
#define WINDOW_STATE_MINIMIZED  0x3

namespace trimana_core
{
    struct window_properties
    {
        window_properties() = default;
        ~window_properties() = default;

        std::string title{"Unknown"};
        int32_t window_state{NULL};
        uint32_t window_flags{NULL};
        void *window_context{nullptr};
        SDL_Window* window{nullptr};
        SDL_Event window_event{NULL};
        event_callback_func call_back;
        
        uint32_t window_min_height{NULL};
        uint32_t window_min_width{NULL};
        uint32_t window_max_height{NULL};
        uint32_t window_max_width{NULL};
        uint32_t window_normal_width{NULL};
        uint32_t window_normal_height{NULL};

        uint32_t window_posx{NULL};
        uint32_t window_posy{NULL};

        uint32_t window_viewport_width{NULL};
        uint32_t window_viewport_height{NULL};

        float window_refresh_rate{0.0f};
        bool window_vsync_enabled{false};
        bool is_window_focused{false};
        bool is_active{false};

    };

    window_properties *window_create(const std::string &window_title);
    void window_destroy(window_properties *window_prop);
    void get_window_events(window_properties *window_prop);
    void set_window_event_handler(window_properties *window_prop, const event_callback_func &call_back);
}
