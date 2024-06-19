#pragma once

#include <SDL3/SDL.h>

#include "logger.hpp"
#include "gl.hpp"

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