#pragma once

#include <string>
#include "events.hpp"

namespace TrimanaCore
{
    class Layer
    {
    public:
        Layer() = default;
        virtual ~Layer() = default;

        virtual bool Init() {}
        virtual void Update() {}
        virtual void OnEvents(Event& e);
    };
}