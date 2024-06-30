#pragma once

#include <thread>
#include <SDL3/SDL.h>

#include "logger.hpp"
#include "gl.hpp"
#include "sdl_window.hpp"


namespace TrimanaCore
{
    class Core
    {
        public:
            Core();
            ~Core();
            
            bool IsReady() const;

        private:
            bool mIsReady{false};
    };
}