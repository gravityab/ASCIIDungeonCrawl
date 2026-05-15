///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "DungeonCrawl.h"

/// Engine Library Headers
#include "Random.h"

/// BearLibTerminal Headers
#include "BearLibTerminal.h"
#include "DllLoader.h"

/// Standard Template Library Headers
#include <clocale>
#include <stdio.h>
#include <string>
#include <vector>

// Windows Library Headers.
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// Link against Version.lib for GetFileVersionInfo / VerQueryValue.
#pragma comment(lib, "Version.lib")

// BearLibTerminal owns the game window after terminal_open(), so it shows BLT's default icon
// rather than the MAINICON we embedded for the .exe (Explorer / shortcut icon still works).
// Locate BLT's window in our process and apply MAINICON as both the small (title-bar) and
// big (taskbar / Alt-Tab) icons via WM_SETICON.
static HWND       s_dcWindow = nullptr;
static BOOL CALLBACK FindOurWindow(HWND hwnd, LPARAM)
{
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid != GetCurrentProcessId()) return TRUE;
    if (!IsWindowVisible(hwnd))       return TRUE;
    // Skip windows that have an owner (dialogs, tooltips); BLT's main window is top-level.
    if (GetWindow(hwnd, GW_OWNER))    return TRUE;
    s_dcWindow = hwnd;
    return FALSE; // stop enumeration
}

static void ApplyWindowIcon()
{
    HMODULE hMod = GetModuleHandleA(nullptr);
    if (!hMod) return;
    HICON hSmall = (HICON)LoadImageA(hMod, "MAINICON", IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    HICON hBig   = (HICON)LoadImageA(hMod, "MAINICON", IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR);
    if (!hSmall && !hBig) return;

    s_dcWindow = nullptr;
    EnumWindows(FindOurWindow, 0);
    if (!s_dcWindow) return;
    if (hSmall) SendMessageA(s_dcWindow, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hSmall));
    if (hBig)   SendMessageA(s_dcWindow, WM_SETICON, ICON_BIG,   reinterpret_cast<LPARAM>(hBig));
}

// Read the FileVersion string from our own .exe's VERSIONINFO resource (defined in
// DungeonCrawl.rc). Returns an empty string on failure so the caller can fall back.
static std::string GetFileVersion()
{
    char exePath[MAX_PATH] = { 0 };
    if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) == 0)
        return std::string();

    DWORD handle = 0;
    DWORD size   = GetFileVersionInfoSizeA(exePath, &handle);
    if (size == 0)
        return std::string();

    std::vector<char> data(size);
    if (!GetFileVersionInfoA(exePath, handle, size, data.data()))
        return std::string();

    // Walk the translation list to find which language/codepage block is present, then read
    // the FileVersion string entry from that block. We default to the first translation found.
    struct LangCp { WORD lang; WORD codePage; };
    LangCp* translations = nullptr;
    UINT    translationsBytes = 0;
    if (!VerQueryValueA(data.data(), "\\VarFileInfo\\Translation",
                        reinterpret_cast<LPVOID*>(&translations), &translationsBytes)
        || translations == nullptr || translationsBytes < sizeof(LangCp))
    {
        return std::string();
    }

    char subBlock[64];
    _snprintf_s(subBlock, _TRUNCATE,
        "\\StringFileInfo\\%04x%04x\\FileVersion",
        translations[0].lang, translations[0].codePage);

    char* versionStr   = nullptr;
    UINT  versionBytes = 0;
    if (!VerQueryValueA(data.data(), subBlock,
                        reinterpret_cast<LPVOID*>(&versionStr), &versionBytes)
        || versionStr == nullptr)
    {
        return std::string();
    }
    return std::string(versionStr);
}

// --------------------------------------------------------------------------------------------------------------------
// Subsystem:
//   Debug   -> SubSystem=Console (a console window appears alongside the game window for printf
//              logging). Configured in DungeonCrawl.vcxproj.
//   Release -> SubSystem=Windows + EntryPointSymbol=mainCRTStartup (no console; just the game
//              window). main() is still the entry point.
// --------------------------------------------------------------------------------------------------------------------
// Error reporting that works in both Console (Debug) and Windows (Release) subsystems:
// printf goes to the Debug console; the MessageBox makes the error visible in Release too.
static void ReportError(const char* msg)
{
    printf("ERROR: %s\n", msg);
    ::MessageBoxA(nullptr, msg, "ASCII Dungeon Crawl", MB_OK | MB_ICONERROR);
}

int main()
{
    SeedRandomizer();
    std::setlocale(LC_ALL, "en_US.UTF-8");

    // BearLibTerminal is embedded in this .exe as RCDATA and delay-loaded. Extract next to the
    // .exe + LoadLibrary here so the delay-load thunks find an already-loaded module on the first
    // terminal_* call.
    const char* loaderError = nullptr;
    if (!LoadEmbeddedBearLibTerminal(&loaderError))
    {
        char buf[512];
        snprintf(buf, sizeof(buf),
            "Failed to load embedded BearLibTerminal.dll:\n\n%s",
            loaderError ? loaderError : "(no detail)");
        ReportError(buf);
        return 1;
    }

    // The TTF font is embedded too. Extract to <exe-dir>/Resources/Consolas-Regular.ttf so
    // terminal_set("font: Resources/Consolas-Regular.ttf, ...") finds it without us shipping
    // a separate Resources/ folder. Non-fatal if it fails - BearLibTerminal will fall back to
    // its built-in bitmap font (glyphs just won't scale smoothly on window resize).
    const char* fontError = nullptr;
    if (!ExtractEmbeddedFont(&fontError))
    {
        // Warn but don't abort.
        char buf[512];
        snprintf(buf, sizeof(buf),
            "Failed to extract embedded Consolas-Regular.ttf (continuing with default font):\n\n%s",
            fontError ? fontError : "(no detail)");
        ReportError(buf);
    }

    // Open the BearLibTerminal window. The original game ran in a 120x30 console buffer, so
    // we ask for the same cell grid. Font is left at BearLibTerminal's built-in default for now;
    // copy a TTF into a Resources/ folder and add `terminal_set("font: Resources/Foo.ttf, size=8x16")`
    // here when you want a specific look.
    if (!terminal_open())
    {
        ReportError("terminal_open() failed. Is BearLibTerminal.dll next to the .exe?");
        return 1;
    }

    // Configure the window. Each call returns 0 on failure; track so we can report which one fails.
    const char* failed = nullptr;
    if (!terminal_set("window.size=120x30"))                 failed = "window.size";

    // Compose the window title with the version pulled from our embedded VERSIONINFO resource.
    // Falls back to the bare game name if the lookup fails for any reason.
    std::string title = "window.title='ASCII Dungeon Crawl";
    {
        std::string ver = GetFileVersion();
        if (!ver.empty())
        {
            title += " v";
            title += ver;
        }
        title += "'";
    }
    if (!terminal_set(title.c_str()))                        failed = "window.title";
    if (!terminal_set("window.resizeable=true"))             failed = "window.resizeable";
    // Use a TTF so glyphs scale smoothly when the user resizes the window. Sized 8x16 initially.
    // The TK_RESIZED handler in Input.cpp re-applies this font at a new size on every resize.
    if (!terminal_set("font: Resources/Consolas-Regular.ttf, size=8x16")) failed = "font (Resources/Consolas-Regular.ttf missing?)";
    // Include 'system' for TK_CLOSE and TK_RESIZED (neither is in the default filter).
    if (!terminal_set("input.filter={keyboard, system}"))    failed = "input.filter";
    if (failed)
    {
        char buf[256];
        snprintf(buf, sizeof(buf), "terminal_set() rejected option: %s", failed);
        ReportError(buf);
    }

    // Prime the BearLibTerminal window: terminal_open() creates it, but it isn't actually shown
    // until the first terminal_refresh(). In Release the timing was such that the first refresh
    // inside RunLoop didn't bring the window forward; an explicit refresh here ensures it does.
    terminal_clear();
    terminal_refresh();

    // Replace BLT's default window icon with our embedded MAINICON. Must run after the first
    // refresh so the window is actually visible and EnumWindows can find it.
    ApplyWindowIcon();

    DungeonCrawl dungeon;
    if (!dungeon.Initialize(60))
        ReportError("Failed to initialize dungeon crawler.");

    dungeon.RunLoop();
    dungeon.Shutdown();

    terminal_close();
    return 0;
}
