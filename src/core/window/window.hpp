#pragma once
namespace TrimanaCore
{
    enum class WindowState
    {
        MAXIMIZED,
        MINIMIZED, 
        NORMAL     
    };

    enum class DialogResult
    {
        OK,
        OK_CANCEL,
        YES_NO,
    };

    typedef struct
    {
        unsigned int MinW;
        unsigned int MinH;
        unsigned int MaxW;
        unsigned int MaxH;

    } WinFixedSize;

    typedef struct 
    {
        unsigned int Width{0};
        unsigned int Height{0};
        
    }WinChangedSize;

    typedef struct
    {
        unsigned int ViewportW{0};
        unsigned int ViewportH{0};

    } WinViewport;

    typedef struct
    {
        unsigned int PosX{0};
        unsigned int PosY{0};

    } WinPos;

    typedef struct 
    {
        bool IsVsyncEnbled{false};
        bool IsFocused{false};
        bool IsActive{false};
        bool IsChildWin{false};

        unsigned int WindowFlags{0};
        const char* Title{nullptr};
        float WindowRefreshRate{0.0f};
        WindowState WindowStatus;

    }WinAttributes;

    template<typename API_Window>
    struct Window
    {
        API_Window* WindowSelf{nullptr};
        void* WindowContext{nullptr};
        API_Window* WindowPrevious{nullptr};
    };

    template<typename API_EventHandler>
    struct WinEventsHandler
    {
        WinEventsHandler() = default;
        ~WinEventsHandler() = default;

        API_EventHandler WindowEvents;
    };

    template<typename API_Window>
    struct WinProperties
    {
        WinProperties() = default;
        ~WinProperties() = default;

        Window<API_Window> Win;
        WinAttributes Attributes;
        WinPos Position;
        WinFixedSize FixedSizes;
        WinChangedSize ChangedSizes;
        WinViewport ViewportSizes;
    };
    
}