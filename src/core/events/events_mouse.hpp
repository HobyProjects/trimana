#pragma once

#include <SDL3/SDL.h>
#include "events.hpp"

namespace TrimanaCore
{
    class TRIMANA_CORE MouseButtonPressEvent : public Events
    {
    public:
        MouseButtonPressEvent(int button) : mButton(button) {}
        virtual ~MouseButtonPressEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_MOUSE_BUTTON_PRESS, EventCategory::EVENT_MOUSE);
        EVENT_LOG(EventType::EVENT_MOUSE_BUTTON_PRESS, "{0} : {1}", GetEventString(), ((mButton == SDL_BUTTON_LEFT) ? "MOUSE_BUTTON_LEFT_PRESS" : (mButton == SDL_BUTTON_RIGHT) ? "MOUSE_BUTTON_RIGHT_PRESS": (mButton == SDL_BUTTON_MIDDLE)  ? "MOUSE_MIDDLE_BUTTON_PRESS" : "MOUSE_BUTTON_UKNOWN_PRESS_DETECTED"));

        int GetButton() const { return mButton; }

    private:
        int mButton{NULL};
    };

    class TRIMANA_CORE MouseButtonReleaseEvent : public Events
    {
    public:
        MouseButtonReleaseEvent(int button) : mButton(button) {}
        virtual ~MouseButtonReleaseEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_MOUSE_BUTTON_RELEASE, EventCategory::EVENT_MOUSE);
        EVENT_LOG(EventType::EVENT_MOUSE_BUTTON_RELEASE, "{0} : {1}", GetEventString(), ((mButton == SDL_BUTTON_LEFT) ? "MOUSE_BUTTON_LEFT_PRESS" : (mButton == SDL_BUTTON_RIGHT) ? "MOUSE_BUTTON_RIGHT_PRESS" : (mButton == SDL_BUTTON_MIDDLE)  ? "MOUSE_MIDDLE_BUTTON_PRESS" : "MOUSE_BUTTON_UKNOWN_PRESS_DETECTED"));

        int GetButton() const { return mButton; }

    private:
        int mButton{NULL};
    };

    class TRIMANA_CORE MouseScrollEvent : public Events
    {
    public:
        MouseScrollEvent(float offset_x, float offset_y) : mOffsetX(offset_x), mOffsetY(offset_y) {}
        virtual ~MouseScrollEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_MOUSE_SCROLL, EventCategory::EVENT_MOUSE);
        EVENT_LOG(EventType::EVENT_MOUSE_SCROLL, "{0} : {1}, {2}", GetEventString(), mOffsetX, mOffsetY);

        float GetOffsetX() const { return mOffsetX; }
        float GetOffsetY() const { return mOffsetY; }

    private:
        float mOffsetX{NULL};
        float mOffsetY{NULL};
    };

    class TRIMANA_CORE MousePosChangeEvent : public Events
    {
    public:
        MousePosChangeEvent(float pos_x, float pos_y) : mPosX(pos_x), mPosY(pos_y) {}
        virtual ~MousePosChangeEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_MOUSE_POS_CHANGE, EventCategory::EVENT_MOUSE);
        EVENT_LOG(EventType::EVENT_MOUSE_POS_CHANGE, "{0} : {1}, {2}", GetEventString(), mPosX, mPosY);

        float GetPosX() const { return mPosX; }
        float GetPosY() const { return mPosY; }

    private:
        float mPosX{0.0f};
        float mPosY{0.0f};
    };
}
