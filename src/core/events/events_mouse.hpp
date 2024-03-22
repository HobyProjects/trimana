#pragma once

#include <SDL3/SDL.h>
#include "events.hpp"

namespace trimana_core
{
    class event_mouse_button_press : public events
    {
    public:
        event_mouse_button_press(int32_t button) : m_button(button) {}
        virtual ~event_mouse_button_press() = default;

        EVENT_TYPE_CATEGORY(EVENT_MOUSE_BUTTON_PRESS, EVENT_MOUSE);
        EVENT_LOG(EVENT_MOUSE_BUTTON_PRESS, "{0} : {1}", get_name(), ((m_button == SDL_BUTTON_LEFT) ? "MOUSE_BUTTON_LEFT_PRESS" : (m_button == SDL_BUTTON_RIGHT) ? "MOUSE_BUTTON_RIGHT_PRESS"
                                                                                                                              : (m_button == SDL_BUTTON_MIDDLE)  ? "MOUSE_MIDDLE_BUTTON_PRESS"
                                                                                                                                                                 : "MOUSE_BUTTON_UKNOWN_PRESS_DETECTED"));

        int32_t get_button() const { return m_button; }

    private:
        int32_t m_button{NULL};
    };

    class event_mouse_button_release : public events
    {
    public:
        event_mouse_button_release(int32_t button) : m_button(button) {}
        virtual ~event_mouse_button_release() = default;

        EVENT_TYPE_CATEGORY(EVENT_MOUSE_BUTTON_RELEASE, EVENT_MOUSE);
        EVENT_LOG(EVENT_MOUSE_BUTTON_RELEASE, "{0} : {1}", get_name(), ((m_button == SDL_BUTTON_LEFT) ? "MOUSE_BUTTON_LEFT_PRESS" : (m_button == SDL_BUTTON_RIGHT) ? "MOUSE_BUTTON_RIGHT_PRESS"
                                                                                                                                : (m_button == SDL_BUTTON_MIDDLE)  ? "MOUSE_MIDDLE_BUTTON_PRESS"
                                                                                                                                                                   : "MOUSE_BUTTON_UKNOWN_PRESS_DETECTED"));

        int32_t get_button() const { return m_button; }

    private:
        int32_t m_button{NULL};
    };

    class event_mouse_button_hold : public events
    {
    public:
        event_mouse_button_hold(int32_t button) : m_button(button) {}
        virtual ~event_mouse_button_hold() = default;

        EVENT_TYPE_CATEGORY(EVENT_MOUSE_BUTTON_HOLD, EVENT_MOUSE);
        EVENT_LOG(EVENT_MOUSE_BUTTON_HOLD, "{0} : {1}", get_name(), ((m_button == SDL_BUTTON_LEFT) ? "MOUSE_BUTTON_LEFT_PRESS" : (m_button == SDL_BUTTON_RIGHT) ? "MOUSE_BUTTON_RIGHT_PRESS"
                                                                                                                             : (m_button == SDL_BUTTON_MIDDLE)  ? "MOUSE_MIDDLE_BUTTON_PRESS"
                                                                                                                                                                : "MOUSE_BUTTON_UKNOWN_PRESS_DETECTED"));

        int32_t get_button() const { return m_button; }

    private:
        int32_t m_button{NULL};
    };

    class event_mouse_scroll : public events
    {
    public:
        event_mouse_scroll(int32_t offset_x, int32_t offset_y) : m_offset_x(offset_x), m_offset_y(offset_y) {}
        virtual ~event_mouse_scroll() = default;

        EVENT_TYPE_CATEGORY(EVENT_MOUSE_SCROLL, EVENT_MOUSE);
        EVENT_LOG(EVENT_MOUSE_SCROLL, "{0} : {1}, {2}", get_name(), m_offset_x, m_offset_y);

        int32_t get_offset_x() const { return m_offset_x; }
        int32_t get_offset_y() const { return m_offset_y; }

    private:
        int32_t m_offset_x{NULL};
        int32_t m_offset_y{NULL};
    };

    class event_mouse_pos_change : public events
    {
    public:
        event_mouse_pos_change(float pos_x, float pos_y) : m_posx(pos_x), m_posy(pos_y) {}
        virtual ~event_mouse_pos_change() = default;

        EVENT_TYPE_CATEGORY(EVENT_MOUSE_POS_CHANGE, EVENT_MOUSE);
        EVENT_LOG(EVENT_MOUSE_POS_CHANGE, "{0} : {1}, {2}", get_name(), m_posx, m_posy);

        float get_pos_x() const { return m_posx; }
        float get_pos_y() const { return m_posy; }

    private:
        float m_posx{0.0f};
        float m_posy{0.0f};
    };
}
