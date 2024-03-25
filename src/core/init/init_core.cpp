#include "init_core.hpp"

int trimana_core::core::core_init = INIT_FAILED;

void trimana_core::core::init_core()
{
    if(sdl_init( SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != INIT_FAILED)
    {
        core::core_init = INIT_SUCCESS;
    }
}

int trimana_core::core::sdl_init(unsigned int flags)
{
    if(SDL_Init(flags) != INIT_FAILED)
    {
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

        return INIT_SUCCESS;
    }

    LOG_CRITICAL("SOMETHING WENT WRONG WHILE INITIALIZING SDL >> {0}", SDL_GetError());
    return INIT_FAILED;
}

void trimana_core::core::quit_core()
{
    if(core_init)
    {
        SDL_Quit();
    }
}