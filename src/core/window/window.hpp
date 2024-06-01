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
        unsigned int ViewpportH;

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
    struct WinAPI
    {
        API_Window* WindowSelf;
        void* WindowContext;
    };

    template<typename API_EventHandler>
    struct WinEventsHandler
    {
        WinEventsHandler() = default;
        ~WinEventsHandler() = default;

        API_EventHandler WindowEvents;
    };

    template<typename API>
    struct WinProp
    {
        WinProp() = default;
        ~WinProp() = default;

        WinAPI<API> API;
        WinAttributes Attributes;
        WinPos Position;
        WinFixedSize FixedSizes;
        WinChangedSize ChangedSizes;
        WinViewport ViewportSizes;
    };
    
}