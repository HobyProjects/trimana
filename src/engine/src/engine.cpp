#include "engine.hpp"

TrimanaEngine::Engine::Engine()
{
    mCore = new TrimanaCore::Core();

    if (mCore->IsReady())
    {
        mMainWindow = new TrimanaCore::MainWindow("Trimana Engine");
        mEngineEvent = new TrimanaCore::AppEvents(mMainWindow);
        TrimanaCore::GL::Load();

        mEngineEvent->SetEventHandlerFunc(ONEVENT_CALLBACK(Engine::OnEvents));

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;
        // io.WantCaptureKeyboard = true;
        // io.WantCaptureMouse = true;

        ImGui_ImplSDL3_InitForOpenGL(mMainWindow->Properties->Win.WindowSelf, (SDL_GLContext)mMainWindow->Properties->Win.WindowContext);
        ImGui_ImplOpenGL3_Init(TrimanaCore::GL::GetGLSLVersion().c_str());
        ImGui_ImplSDL3_ProcessEvent(&mEngineEvent->Events.WindowEvents);

        mUI = new TrimanaCore::UI(mMainWindow, mEngineEvent, TrimanaCore::UIColor::UI_DARK);

        std::thread handle_events(&Engine::PollEvents, this);
        mEventsHandler = std::move(handle_events);
    }
}

TrimanaEngine::Engine::~Engine()
{
    delete mUI;
    delete mEngineEvent;
    delete mMainWindow;
    delete mCore;
}

void TrimanaEngine::Engine::Start()
{   
    if (mCore->IsReady())
    {
        while (mMainWindow->Properties->Attributes.IsActive)
        {
            PollEvents();
            
            if (mMainWindow->Properties->Attributes.IsFocused)
            {
                mUI->Begin();



                mUI->End();
            }

            if (mMainWindow->Properties->Attributes.IsVsyncEnbled)
                SDL_GL_SwapWindow(mMainWindow->Properties->Win.WindowSelf);
        }
    }
}

void TrimanaEngine::Engine::Stop()
{
    mMainWindow->Properties->Attributes.IsActive = false;
}

void TrimanaEngine::Engine::PollEvents()
{
    mEngineEvent->PollEvents();
    mUI->PollEvents();
}

void TrimanaEngine::Engine::OnEvents(TrimanaCore::Event &e)
{
    TrimanaCore::EventsHandler event_handler(e);
    event_handler.Disspatch<TrimanaCore::WindowCloseEvent>(ONEVENT_CALLBACK(Engine::WindowCloseEvent));

#if defined(EVENT_ALLOW_TO_DISPLAY)
    e.LogEvent();
#endif
}

bool TrimanaEngine::Engine::WindowCloseEvent(const TrimanaCore::Event &e)
{
    Stop();

    if (mEventsHandler.joinable())
        mEventsHandler.join();

    return true;
}