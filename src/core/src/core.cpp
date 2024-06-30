#include "core.hpp"

TrimanaCore::Core::Core()
{
    Log::Init();

    if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)) != 0)
    {
        TRIMANA_CORE_CRITICAL("SOMETHING WENT WRONG WHILE INITIALIZING SDL >> {0}", SDL_GetError());
        mIsReady = false;
        return;
    }

    mIsReady = true;
}

TrimanaCore::Core::~Core()
{
    if (mIsReady)
    {
        SDL_Quit();
    }
}

bool TrimanaCore::Core::IsReady() const
{
    return mIsReady;
}