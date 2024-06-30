#include "sdl_window.hpp"

TrimanaCore::MainWindow::MainWindow(const std::string &title)
{
    Properties = new WinProperties<SDL_Window>();
    SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display_id);
    if (mode != nullptr)
    {
        Properties->Attributes.WindowRefreshRate = mode->refresh_rate;
        Properties->FixedSizes.MaxW = mode->w;
        Properties->FixedSizes.MaxH = mode->h;
        Properties->FixedSizes.MinH = 720;
        Properties->FixedSizes.MinW = 1024;
    }
    else
    {
        TRIMANA_CORE_WARN("DISPLAY INFO WAS NOT DETECTED BY THE APPLICATION >> {0}", SDL_GetError());
        Properties->FixedSizes.MaxW = NULL;
        Properties->FixedSizes.MaxH = NULL;
        Properties->FixedSizes.MinH = NULL;
        Properties->FixedSizes.MinW = NULL;
    }

    Properties->Attributes.Title = title.c_str();
    Properties->Attributes.WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    Properties->Win.WindowSelf = SDL_CreateWindow(title.c_str(), Properties->FixedSizes.MinW, Properties->FixedSizes.MinH, Properties->Attributes.WindowFlags);
    if (Properties->Win.WindowSelf != nullptr)
    {
        TRIMANA_CORE_INFO("SDL WINDOW CREATED");
        SDL_SetWindowMinimumSize(Properties->Win.WindowSelf, Properties->FixedSizes.MinW, Properties->FixedSizes.MinH);
        SDL_SetWindowMaximumSize(Properties->Win.WindowSelf, Properties->FixedSizes.MaxW, Properties->FixedSizes.MaxH);
        SDL_GetWindowSizeInPixels(Properties->Win.WindowSelf, &Properties->Viewport.Width, &Properties->Viewport.Height);
        Properties->Win.WindowContext = (SDL_GLContext)SDL_GL_CreateContext(Properties->Win.WindowSelf);
        SDL_GL_MakeCurrent(Properties->Win.WindowSelf, Properties->Win.WindowContext);
        SDL_GL_SetSwapInterval(1);

        if (!Properties->Win.WindowContext)
            TRIMANA_CORE_WARN("CONTEXT FOR GL WAS NOT CREATED >> {0}", SDL_GetError());

        Properties->Attributes.IsActive = true;
        Properties->Attributes.IsChildWin = false;
        Properties->Attributes.IsFocused = true;
        Properties->Attributes.IsVsyncEnbled = true;
        return;
    }

    TRIMANA_CORE_CRITICAL("SDL WINDOW WAS NOT CREATED >>  {0}", SDL_GetError());
}

TrimanaCore::MainWindow::~MainWindow()
{
    if (!Properties->Win.WindowContext)
        SDL_GL_DeleteContext(Properties->Win.WindowContext);

    if (Properties->Win.WindowSelf != nullptr)
        SDL_DestroyWindow(Properties->Win.WindowSelf);

    delete Properties;
}

SDL_Window *TrimanaCore::MainWindow::GetWindow() const
{
    return Properties->Win.WindowSelf;
}

void TrimanaCore::AppEvents::PollEvents()
{
    SDL_WaitEvent(&Events.WindowEvents);

    switch (Events.WindowEvents.type)
    {

    /* Window Events handle */
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    {
        WindowCloseEvent window_close{};
        mCallBackFunc(window_close);
        break;
    }
    case SDL_EVENT_WINDOW_MOVED:
    {
        mWindow->Properties->Position.PosX = Events.WindowEvents.window.data1;
        mWindow->Properties->Position.PosY = Events.WindowEvents.window.data2;
        WindowMoveEvent window_mov(mWindow->Properties->Position.PosX, mWindow->Properties->Position.PosY);
        mCallBackFunc(window_mov);
        break;
    }
    case SDL_EVENT_WINDOW_RESIZED:
    {
        SDL_GetWindowSizeInPixels(mWindow->Properties->Win.WindowSelf, &mWindow->Properties->Viewport.Width, &mWindow->Properties->Viewport.Height);
        mWindow->Properties->ChangedSizes.Width = Events.WindowEvents.window.data1;
        mWindow->Properties->ChangedSizes.Height = Events.WindowEvents.window.data2;

        if ((mWindow->Properties->FixedSizes.MaxW && mWindow->Properties->FixedSizes.MaxH && mWindow->Properties->FixedSizes.MinW && mWindow->Properties->FixedSizes.MinH) != NULL)
        {
            if (mWindow->Properties->ChangedSizes.Width <= mWindow->Properties->FixedSizes.MaxW && mWindow->Properties->ChangedSizes.Height <= mWindow->Properties->FixedSizes.MaxH &&
                mWindow->Properties->ChangedSizes.Width >= mWindow->Properties->FixedSizes.MinW && mWindow->Properties->ChangedSizes.Height >= mWindow->Properties->FixedSizes.MinH)
            {
                mWindow->Properties->Attributes.WindowStatus = WindowState::NORMAL;
            }
        }

        WindowResizeEvent window_resize(mWindow->Properties->ChangedSizes.Width, mWindow->Properties->ChangedSizes.Height);
        mCallBackFunc(window_resize);

        ViewportResizeEvent window_vp_resize(mWindow->Properties->Viewport.Width, mWindow->Properties->Viewport.Height);
        mCallBackFunc(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    {
        SDL_GetWindowSizeInPixels(mWindow->Properties->Win.WindowSelf, &mWindow->Properties->Viewport.Width, &mWindow->Properties->Viewport.Height);
        mWindow->Properties->ChangedSizes.Width = Events.WindowEvents.window.data1;
        mWindow->Properties->ChangedSizes.Height = Events.WindowEvents.window.data2;
       
        if ((mWindow->Properties->FixedSizes.MaxW && mWindow->Properties->FixedSizes.MaxH && mWindow->Properties->FixedSizes.MinW && mWindow->Properties->FixedSizes.MinH) != NULL)
        {
            if (mWindow->Properties->ChangedSizes.Width <= mWindow->Properties->FixedSizes.MaxW && mWindow->Properties->ChangedSizes.Height <= mWindow->Properties->FixedSizes.MaxH &&
                mWindow->Properties->ChangedSizes.Width >= mWindow->Properties->FixedSizes.MinW && mWindow->Properties->ChangedSizes.Height >= mWindow->Properties->FixedSizes.MinH)
            {
                mWindow->Properties->Attributes.WindowStatus = WindowState::NORMAL;
            }
        }

        WindowResizeEvent window_resize(mWindow->Properties->ChangedSizes.Width, mWindow->Properties->ChangedSizes.Height);
        mCallBackFunc(window_resize);

        ViewportResizeEvent window_vp_resize(mWindow->Properties->Viewport.Width, mWindow->Properties->Viewport.Height);
        mCallBackFunc(window_vp_resize);
        break;
    }
    case SDL_EVENT_WINDOW_MINIMIZED:
    {
        if (mWindow->Properties->Attributes.IsFocused)
            mWindow->Properties->Attributes.IsFocused = !mWindow->Properties->Attributes.IsFocused;

        mWindow->Properties->Attributes.WindowStatus = WindowState::MINIMIZED;

        WindowMinimizeEvent window_minimize;
        mCallBackFunc(window_minimize);

        WindowFocusLostEvent window_focus;
        mCallBackFunc(window_focus);
        break;
    }
    case SDL_EVENT_WINDOW_MAXIMIZED:
    {
        if (!mWindow->Properties->Attributes.IsFocused)
            mWindow->Properties->Attributes.IsFocused = !mWindow->Properties->Attributes.IsFocused;

        mWindow->Properties->Attributes.WindowStatus = WindowState::MAXIMIZED;
        WindowMaximizeEvent window_maximized;
        mCallBackFunc(window_maximized);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_LOST:
    {
        if (mWindow->Properties->Attributes.IsFocused)
            mWindow->Properties->Attributes.IsFocused = !mWindow->Properties->Attributes.IsFocused;

        WindowFocusLostEvent window_focus;
        mCallBackFunc(window_focus);
        break;
    }
    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    {
        if (!mWindow->Properties->Attributes.IsFocused)
            mWindow->Properties->Attributes.IsFocused = !mWindow->Properties->Attributes.IsFocused;

        WindowFocusGainEvent window_foucus;
        mCallBackFunc(window_foucus);
        break;
    }

    /* Keyboard Events handle */
    case SDL_EVENT_KEY_DOWN:
    {
        int code = static_cast<int>(Events.WindowEvents.key.keysym.sym);
        static bool assign_once = true;
        static int previous_code = 0;

        if (assign_once)
        {
            previous_code = code;
            assign_once = !assign_once;
        }

        if (previous_code == code)
        {
            KeyboardKeyHold key_hold(static_cast<int>(code));
            mCallBackFunc(key_hold);
        }
        else
        {
            previous_code = code;
            KeyboardKeyPressEvent key_pressed(static_cast<int>(code));
            mCallBackFunc(key_pressed);
        }

        break;
    }
    case SDL_EVENT_KEY_UP:
    {
        SDL_Keycode code = Events.WindowEvents.key.keysym.sym;
        KeyboardKeyReleaseEvent key_release(code);
        mCallBackFunc(key_release);
        break;
    }

    /* Mouse event handle */
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    {
        MouseButtonPressEvent mouse_press(Events.WindowEvents.button.state);
        mCallBackFunc(mouse_press);
        break;
    }
    case SDL_EVENT_MOUSE_BUTTON_UP:
    {
        MouseButtonReleaseEvent mouse_release(Events.WindowEvents.button.state);
        mCallBackFunc(mouse_release);
        break;
    }
    case SDL_EVENT_MOUSE_MOTION:
    {
        MousePosChangeEvent mouse_pos((float)Events.WindowEvents.motion.x, (float)Events.WindowEvents.motion.y);
        mCallBackFunc(mouse_pos);
        break;
    }
    case SDL_EVENT_MOUSE_WHEEL:
    {
        MouseScrollEvent mouse_scroll(Events.WindowEvents.wheel.x, Events.WindowEvents.wheel.y);
        mCallBackFunc(mouse_scroll);
        break;
    }
    default:
    {
        break;
    }
    };
}

void TrimanaCore::AppEvents::SetEventHandlerFunc(const EventCallbackFunc &call_back)
{
    mCallBackFunc = call_back;
}
