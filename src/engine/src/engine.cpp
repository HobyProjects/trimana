#include "engine.hpp"

TrimanaEngine::Engine::Engine()
{
    mCore = new TrimanaCore::Core();
    mCore->InitCore();

    if (mCore->CoreInitSuccess())
    {
        mWindow = new TrimanaCore::API_Window<SDL_Window>("Trimana Engine");
        mEvent = new TrimanaCore::API_Event<SDL_Event>();
        mEvent->SetEventHandlerFunc(ONEVENT_CALLBACK(Engine::OnEvents));
        TrimanaCore::GL::GL_Load();
    }
}

TrimanaEngine::Engine::~Engine()
{
    delete mEvent;
    delete mWindow;
    delete mCore;
}

void TrimanaEngine::Engine::Start()
{
    if (mCore->CoreInitSuccess())
    {
        while (mWindow->GetWindowProperties()->Attributes.IsActive)
        {
            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);




            glClear(GL_COLOR_BUFFER_BIT);
            
            if (mWindow->GetWindowProperties()->Attributes.IsVsyncEnbled)
                SDL_GL_SwapWindow(mWindow->GetWindowProperties()->Win.WindowSelf);

            mEvent->PollEvents(mWindow->GetWindowProperties());
        }
    }
}

void TrimanaEngine::Engine::Stop()
{
    mWindow->GetWindowProperties()->Attributes.IsActive = false;
}

void TrimanaEngine::Engine::OnEvents(TrimanaCore::Events &e)
{
    TrimanaCore::EventsHandler event_handler(e);
    event_handler.Disspatch<TrimanaCore::WindowCloseEvent>(ONEVENT_CALLBACK(Engine::WindowCloseEvent));

#if defined(EVENT_ALLOW_TO_DISPLAY) && defined(_DEBUG)
    e.LogEvent();
#endif
}

bool TrimanaEngine::Engine::WindowCloseEvent(const TrimanaCore::Events &e)
{
    Stop();
    return false;
}