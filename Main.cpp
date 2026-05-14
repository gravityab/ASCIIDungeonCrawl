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

// Windows Library Headers.
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

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
    if (!terminal_set("window.title='ASCII Dungeon Crawl'")) failed = "window.title";
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

    DungeonCrawl dungeon;
    if (!dungeon.Initialize(60))
        ReportError("Failed to initialize dungeon crawler.");

    dungeon.RunLoop();
    dungeon.Shutdown();

    terminal_close();
    return 0;
}
