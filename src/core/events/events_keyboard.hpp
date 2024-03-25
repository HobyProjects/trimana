#pragma once

#include "events.hpp"

namespace trimana_core
{
    class event_keyboard_key_press : public events
    {
    public:
        event_keyboard_key_press(int key_code) : m_key_code(key_code) {}
        virtual ~event_keyboard_key_press() = default;

        EVENT_TYPE_CATEGORY(EVENT_KEYBOARD_KEY_PRESS, EVENT_KEYBOARD);
        EVENT_LOG(EVENT_KEYBOARD_KEY_PRESS, "{0}", get_name());

        int get_key_code() const { return m_key_code; }

    private:
        int m_key_code{NULL};
    };

    class event_keyboard_key_release : public events
    {
    public:
        event_keyboard_key_release(int key_code) : m_key_code(key_code) {}
        virtual ~event_keyboard_key_release() = default;

        EVENT_TYPE_CATEGORY(EVENT_KEYBOARD_KEY_RELEASE, EVENT_KEYBOARD);
        EVENT_LOG(EVENT_KEYBOARD_KEY_RELEASE, "{0}", get_name());

        int get_key_code() const { return m_key_code; }

    private:
        int m_key_code{NULL};
    };

    class event_keyboard_key_hold : public events
    {
    public:
        event_keyboard_key_hold(int key_code) : m_key_code(key_code) {}
        virtual ~event_keyboard_key_hold() = default;

        EVENT_TYPE_CATEGORY(EVENT_KEYBOARD_KEY_HOLD, EVENT_KEYBOARD);
        EVENT_LOG(EVENT_KEYBOARD_KEY_HOLD, "{0}", get_name());

        int get_key_code() const { return m_key_code; }

    private:
        int m_key_code{NULL};
    };
}
