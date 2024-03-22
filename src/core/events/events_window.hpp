#pragma once

#include "events.hpp"

namespace trimana_core
{
    class event_window_resize : public events
    {
    public:
        event_window_resize(int32_t width, int32_t height) : m_width(width), m_height(height) {}
        virtual ~event_window_resize() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_RESIZE, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_RESIZE, "{0} : {1}, {2}", get_name(), m_width, m_height);

        int32_t get_width() const { return m_width; }
        int32_t get_height() const { return m_height; }

    private:
        int32_t m_width{NULL};
        int32_t m_height{NULL};
    };

    class event_window_viewport_resize : public events
    {
    public:
        event_window_viewport_resize(int width, int height) : m_width(width), m_height(height) {}
        virtual ~event_window_viewport_resize() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_VIEWPORT_RESIZE, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_VIEWPORT_RESIZE, "{0} : {1} , {2}", get_name(), m_width, m_height);

        int32_t get_width() const { return m_width; }
        int32_t get_height() const { return m_height; }

    private:
        int32_t m_width{NULL};
        int32_t m_height{NULL};
    };

    class event_window_close : public events
    {
    public:
        event_window_close() = default;
        virtual ~event_window_close() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_CLOSE, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_CLOSE, "{0}", get_name());
    };

    class event_window_pos_change : public events
    {
    public:
        event_window_pos_change(int32_t pos_x, int32_t pos_y) : m_posx(pos_x), m_posy(pos_y) {}
        virtual ~event_window_pos_change() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_POS_CHANGED, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_POS_CHANGED, EVENT_WINDOW);

        int32_t get_posx() const { return m_posx; }
        int32_t get_posy() const { return m_posy; }

    private:
        int32_t m_posx{NULL};
        int32_t m_posy{NULL};
    };

    class event_window_minimize : public events
    {
    public:
        event_window_minimize() = default;
        virtual ~event_window_minimize() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_MINIMIZED, EVENT_WINDOW)
        EVENT_LOG(EVENT_WINDOW_MINIMIZED, "{0}", get_name());
    };

    class event_window_maximize : public events
    {
    public:
        event_window_maximize() = default;
        virtual ~event_window_maximize() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_MAXIMIZED, EVENT_WINDOW)
        EVENT_LOG(EVENT_WINDOW_MAXIMIZED, "{0}", get_name());
    };

    class event_window_focus_lost : public events
    {
    public:
        event_window_focus_lost() = default;
        virtual ~event_window_focus_lost() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_FOCUS_LOST, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_FOCUS_LOST, "{0}", get_name());
    };

    class event_window_focus_gain : public events
    {
    public:
        event_window_focus_gain() = default;
        virtual ~event_window_focus_gain() = default;

        EVENT_TYPE_CATEGORY(EVENT_WINDOW_FOCUS_GAIN, EVENT_WINDOW);
        EVENT_LOG(EVENT_WINDOW_FOCUS_GAIN, "{0}", get_name());
    };
}
