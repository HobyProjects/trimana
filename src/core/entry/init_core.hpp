#pragma once

#include <SDL3/SDL.h>
#include "utils/logger.hpp"

#define INIT_SUCCESS 0
#define INIT_FAILED  1

namespace trimana_core
{
    class core
    {
        public:
            static void init_core();
            static void quit_core();

        private:
            core() = default;
            ~core() = default;

            static int sdl_init(unsigned int flags);


        public:
            static int core_init;
    };
}