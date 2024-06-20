#include "core.hpp"

bool TrimanaCore::Core::InitCore()
{
    Log::Init();

    if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)) == 0)
    {
        mCoreInitSuccess = true;
        return mCoreInitSuccess;
    }

    TRIMANA_CORE_CRITICAL("SOMETHING WENT WRONG WHILE INITIALIZING SDL >> {0}", SDL_GetError());
    mCoreInitSuccess = false;
    return mCoreInitSuccess;
}

bool TrimanaCore::Core::CoreInitSuccess()
{
    return mCoreInitSuccess;
}

bool TrimanaCore::Core::QuitCore()
{
    if (mCoreInitSuccess)
    {
        SDL_Quit();
        return true;
    }

    return false;
}