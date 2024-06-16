#pragma once

#include <glm/glm.hpp>
#include "ecs.hpp"

namespace TrimanaCore
{
    class TransformComponent : public Component
    {
        public:
            TransformComponent() = default;
            virtual ~TransformComponent() = default;

        public:
            glm::vec3 Position {0.0f, 0.0f, 0.0f};
            float Angle{0.0f};
            float Scaler{0.0f};
    };
}