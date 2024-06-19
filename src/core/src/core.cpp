#include "core.hpp"

bool TrimanaCore::Core::InitCore()
{
    Log::Init();

    if (SDL_Init((SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER)) != 0)
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

        glEnable(GL_DEPTH_TEST);
        mCoreInitSuccess = true;

        mGLInfo = std::make_shared<GLInfo>();

        mGLInfo->GL_LoadSuccess = true;
        mGLInfo->GLVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
        mGLInfo->GLRenderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
        mGLInfo->GLVendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
        mGLInfo->GLSLVersion = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        char GL_MAJOR = mGLInfo->GLVersion[0];
        char GL_MINOR = mGLInfo->GLVersion[2];
        char GL_PATCH = mGLInfo->GLVersion[4];

        std::stringstream ss;
        ss << "#version " << GL_MAJOR << "." << GL_MINOR << "." << GL_PATCH << " core";
        mGLInfo->GLSLVersion = ss.str().c_str();

        return true;
    }

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
    if (mCoreInitSuccess)
    {
        SDL_Quit();
        return true;
    }

    return false;
}