#pragma once

#include "events.hpp"

namespace TrimanaCore
{
    class KeyboardKeyPressEvent : public Events
    {
    public:
        KeyboardKeyPressEvent(int key_code) : mKeyCode(key_code) {}
        virtual ~KeyboardKeyPressEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_KEYBOARD_KEY_PRESS, EventCategory::EVENT_KEYBOARD);
        EVENT_LOG(EventType::EVENT_KEYBOARD_KEY_PRESS, "{0}", GetEventString());

        int GetKeyCode() const { return mKeyCode; }

    private:
        int mKeyCode{NULL};
    };

    class KeyboardKeyReleaseEvent : public Events
    {
    public:
        KeyboardKeyReleaseEvent(int key_code) : mKeyCode(key_code) {}
        virtual ~KeyboardKeyReleaseEvent() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_KEYBOARD_KEY_RELEASE, EventCategory::EVENT_KEYBOARD);
        EVENT_LOG(EventType::EVENT_KEYBOARD_KEY_RELEASE, "{0}", GetEventString());

        int GetKeyCode() const { return mKeyCode; }

    private:
        int mKeyCode{NULL};
    };

    class KeyboardKeyHold : public Events
    {
    public:
        KeyboardKeyHold(int key_code) : mKeyCode(key_code) {}
        virtual ~KeyboardKeyHold() = default;

        EVENT_TYPE_CATEGORY(EventType::EVENT_KEYBOARD_KEY_HOLD, EventCategory::EVENT_KEYBOARD);
        EVENT_LOG(EventType::EVENT_KEYBOARD_KEY_HOLD, "{0}", GetEventString());

        int GetKeyCode() const { return mKeyCode; }

    private:
        int mKeyCode{NULL};
    };
}
