#pragma once

#include <functional>
#include "utils/logger.hpp"

#define ONEVENT_CALLBACK(func) std::bind(&func, this, std::placeholders::_1)

namespace TrimanaCore
{
    enum class EventCategory
    {
        EVENT_WINDOW,
        EVENT_KEYBOARD,
        EVENT_MOUSE,
        EVENT_UNKNOWN
    };

    enum class EventType
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
    static EventType GetStaticType() { return type; }                       \
    virtual EventType GetType() const override { return GetStaticType(); } \
    virtual EventCategory GetCategory() const override { return category; }

#ifdef _DEBUG

#define EVENT_ALLOW_TO_DISPLAY             \
    virtual void LogEvent() = 0; \
    virtual const char *GetEventString() const = 0;

#define EVENT_LOG(name, ...)                                        \
    virtual const char *GetEventString() const override { return #name; } \
    virtual void LogEvent() override { TRIMANA_CORE_INFO(__VA_ARGS__); }

#else
#define EVENT_ALLOW_TO_DISPLAY
#define EVENT_LOG(name, ...) // None
#endif

    class Events
    {
    public:
        Events() = default;
        virtual ~Events() = default;
        virtual EventType GetType() const = 0;
        virtual EventCategory GetCategory() const = 0;
        EVENT_ALLOW_TO_DISPLAY

    public:
        bool IsHandled{false};
    };

    class EventsHandler
    {
    public:
        EventsHandler(Events &event) : mEvent(event) {}
        ~EventsHandler() = default;

        template <typename T, typename FUNC>
        bool Disspatch(const FUNC &func)
        {
            if (mEvent.get_type() == T::get_static_type())
            {
                mEvent.IsHandled |= func(static_cast<T &>(mEvent));
                return true;
            }

            return false;
        }

    private:
        Events &mEvent;
    };

    using EventCallbackFunc = std::function<void(Events &)>;
}
