#pragma once

#include "events.hpp"

namespace TrimanaCore
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(int width, int height) : mWidth(width), mHeight(height) {}
        virtual ~WindowResizeEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_RESIZE, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_RESIZE, "{0} : {1}, {2}", GetEventString(), mWidth, mHeight);

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }

    private:
        int mWidth{NULL};
        int mHeight{NULL};
    };

    class ViewportResizeEvent : public Event
    {
    public:
        ViewportResizeEvent(int width, int height) : mWidth(width), mHeight(height) {}
        virtual ~ViewportResizeEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_VIEWPORT_RESIZE, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_VIEWPORT_RESIZE, "{0} : {1} , {2}", GetEventString(), mWidth, mHeight);

        int GetWidth() const { return mWidth; }
        int GetHeight() const { return mHeight; }

    private:
        int mWidth{NULL};
        int mHeight{NULL};
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;
        virtual ~WindowCloseEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_CLOSE, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_CLOSE, "{0}", GetEventString());
    };

    class WindowMoveEvent : public Event
    {
    public:
        WindowMoveEvent(int pos_x, int pos_y) : mPosX(pos_x), mPosY(pos_y) {}
        virtual ~WindowMoveEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_POS_CHANGED, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_POS_CHANGED, "{0} : {1}, {2}", GetEventString(), mPosX, mPosY);

        int GetPosX() const { return mPosX; }
        int GetPosY() const { return mPosY; }

    private:
        int mPosX{NULL};
        int mPosY{NULL};
    };

    class WindowMinimizeEvent : public Event
    {
    public:
        WindowMinimizeEvent() = default;
        virtual ~WindowMinimizeEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_MINIMIZED, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_MINIMIZED, "{0}", GetEventString());
    };

    class WindowMaximizeEvent : public Event
    {
    public:
        WindowMaximizeEvent() = default;
        virtual ~WindowMaximizeEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_MAXIMIZED, EventCategory::EVENT_WINDOW)
        EVENT_LOG(EventType::EVENT_WINDOW_MAXIMIZED, "{0}", GetEventString());
    };

    class WindowFocusLostEvent : public Event
    {
    public:
        WindowFocusLostEvent() = default;
        virtual ~WindowFocusLostEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_FOCUS_LOST, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EventType::EVENT_WINDOW_FOCUS_LOST, "{0}", GetEventString());
    };

    class WindowFocusGainEvent : public Event
    {
    public:
        WindowFocusGainEvent() = default;
        virtual ~WindowFocusGainEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_WINDOW_FOCUS_GAIN, EventCategory::EVENT_WINDOW);
        EVENT_LOG(EvenetType::EVENT_WINDOW_FOCUS_GAIN, "{0}", GetEventString());
    };
}
