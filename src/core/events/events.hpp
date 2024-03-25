#pragma once

#include <functional>
#include "utils/logger.hpp"

#define SET_ONEVENT_CALLBACK(func) std::bind(&func, this, std::placeholders::_1)

namespace trimana_core
{
    enum EVENT_CATEGORY : int
    {
        EVENT_WINDOW,
        EVENT_KEYBOARD,
        EVENT_MOUSE,
        EVENT_UNKNOWN
    };

    enum EVENT_TYPE : int
    {
        EVENT_WINDOW_RESIZE,
        EVENT_WINDOW_CLOSE,
        EVENT_WINDOW_POS_CHANGED,
        EVENT_WINDOW_MAXIMIZED,
        EVENT_WINDOW_MINIMIZED,
        EVENT_WINDOW_FOCUS_LOST,
        EVENT_WINDOW_FOCUS_GAIN,
        EVENT_WINDOW_VIEWPORT_RESIZE,

        EVENT_KEYBOARD_KEY_PRESS,
        EVENT_KEYBOARD_KEY_RELEASE,
        EVENT_KEYBOARD_KEY_HOLD,

        EVENT_MOUSE_BUTTON_PRESS,
        EVENT_MOUSE_BUTTON_RELEASE,
        EVENT_MOUSE_SCROLL,
        EVENT_MOUSE_POS_CHANGE
    };

#define EVENT_TYPE_CATEGORY(type, category)                                    \
    static EVENT_TYPE get_static_type() { return type; }                       \
    virtual EVENT_TYPE get_type() const override { return get_static_type(); } \
    virtual EVENT_CATEGORY get_category() const override { return category; }

#ifdef _DEBUG

#define EVENT_CAN_LOG             \
    virtual void log_event() = 0; \
    virtual const char *get_name() const = 0;

#define EVENT_LOG(name, ...)                                        \
    virtual const char *get_name() const override { return #name; } \
    virtual void log_event() override { LOG_INFO(__VA_ARGS__); }

#else
#define EVENT_CAN_LOG
#define EVENT_LOG(name, ...) // None
#endif

    class events
    {
    public:
        events() = default;
        virtual ~events() = default;
        virtual EVENT_TYPE get_type() const = 0;
        virtual EVENT_CATEGORY get_category() const = 0;
        EVENT_CAN_LOG

    public:
        bool is_handled{false};
    };

    class events_handler
    {
    public:
        events_handler(events &event) : m_event(event) {}
        ~events_handler() = default;

        template <typename T, typename FUNC>
        bool disspatch(const FUNC &func)
        {
            if (m_event.get_type() == T::get_static_type())
            {
                m_event.is_handled |= func(static_cast<T &>(m_event));
                return true;
            }

            return false;
        }

    private:
        events &m_event;
    };

    using event_callback_func = std::function<void(events &)>;
}
