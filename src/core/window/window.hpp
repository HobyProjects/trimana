#pragma once

#include <string>
#include <array>
#include <SDL3/SDL.h>

#include "entry\init_core.hpp"
#include "events\events_window.hpp"
#include "events\events_keyboard.hpp"
#include "events\events_mouse.hpp"

namespace trimana_core
{
    enum class WINDOW_STATE
    {
        MAXIMIZED,
        MINIMIZED,
        NORMAL
    };

    enum DIALOG_RESULT : int
    {
        OK,
        OK_CANCEL,
        YES_NO,
        ABORT_IGNORE_CANCEL
    };

    class window_interface
    {
        protected:
            window_interface() = default;
            ~window_interface() = default;

            virtual int create(const std::string& title) = 0;
            virtual void destroy() = 0;
            virtual void poll_events() = 0;
            virtual void set_handler(const event_callback_func& callback) = 0;

        protected:
            bool is_vsync_enabled{false};
            bool is_window_focused{false};
            bool is_active{false};
            bool is_child_window{false};
            
            int window_min_height{NULL};
            int window_min_width{NULL};
            int window_max_height{NULL};
            int window_max_width{NULL};
            int window_normal_width{NULL};
            int window_normal_height{NULL};

            int window_posx{NULL};
            int window_posy{NULL};

            int window_viewport_width{NULL};
            int window_viewport_height{NULL};
            
            int window_flags{NULL};
            WINDOW_STATE window_state{NULL};
            DIALOG_RESULT dialog_result;
            float window_refresh_rate{0.0f};
            void *window_context{nullptr};
            SDL_Window* window_self{nullptr};
            std::string title{"Unknown"};
            SDL_Event window_event{NULL};
            event_callback_func call_back;
    };

    class window : public window_interface
    {
        public:
            window() = default;
            ~window() = default;

            int create(const std::string& title) override;
            void destroy() override;
            void poll_events() override;
            void set_handler(const event_callback_func& callback) override;
    };
}
