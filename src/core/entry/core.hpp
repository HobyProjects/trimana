#pragma once

#include <SDL3/SDL.h>

#include "utils/logger.hpp"
#include "renders/GL/gl.hpp"

#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)

namespace TrimanaCore
{
    class Core
    {
        public:
            Core() = default;
            ~Core() = default;

            bool CoreInitSuccess();
            bool InitCore();
            bool QuitCore();

        private:
            bool mCoreInitSuccess;
            std::shared_ptr<GLInfo> mGLInfo;
    };
}