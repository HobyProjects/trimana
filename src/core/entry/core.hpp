#pragma once

#include <SDL3/SDL.h>
#include "utils/logger.hpp"
#include "utils/platform.hpp"

#ifdef TRIMANA_PARIMARY_API__SDL
#define SDL_INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)
#endif

namespace TrimanaCore
{
    class TRIMANA_CORE Core
    {
        public:
            Core() = default;
            ~Core() = default;

            bool CoreInitSuccess();
            bool InitCore();
            bool QuitCore();

        private:
            bool mCoreInitSuccess;
    };
}