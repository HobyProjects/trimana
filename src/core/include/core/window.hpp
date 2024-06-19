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
        unsigned int Width;
        unsigned int Height;
        
    }WinChangedSize;

    typedef struct
    {
        unsigned int ViewportW;
        unsigned int ViewportH;

    } WinViewport;

    typedef struct
    {
        unsigned int PosX;
        unsigned int PosY;

    } WinPos;

    typedef struct 
    {
        bool IsVsyncEnbled;
        bool IsFocused;
        bool IsActive;
        bool IsChildWin;

        unsigned int WindowFlags;
        const char* Title;
        float WindowRefreshRate;
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