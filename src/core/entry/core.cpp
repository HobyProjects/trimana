#include "core.hpp"

bool TrimanaCore::Core::InitCore()
{
    Log::Init();

#if defined(TRIMANA_PLATFORM_WINDOWS)

    if (SDL_Init(SDL_INIT_FLAGS) != 0)
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

        glewExperimental = true;
        if (glewInit() != GL_TRUE)
        {
            TRIMANA_CORE_CRITICAL("UNABLE TO LOAD GL FUNCTIONS");
            return false;
        }

        mCoreInitSuccess = true;
        return true;
    }

#endif

    TRIMANA_CORE_CRITICAL("SOMETHING WENT WRONG WHILE INITIALIZING SDL >> {0}", SDL_GetError());
    mCoreInitSuccess = false;
    return false;
}

bool TrimanaCore::Core::CoreInitSuccess()
{
    return mCoreInitSuccess;
}

bool TrimanaCore::Core::QuitCore()
{
#ifdef TRIMANA_PLATFORM_WINDOWS
    if (mCoreInitSuccess)
    {
        SDL_Quit();
        return true;
    }
#endif
    return false;
}