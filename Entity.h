///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entity.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)
//
// ConsoleEntity is the screen-region renderer used by Frame / Image / Animation and the game.
// It originally wrapped a Win32 CHAR_INFO buffer flushed via WriteConsoleOutputW; this port
// keeps the same public API but routes all writes through BearLibTerminal so existing call sites
// continue to compile and behave the same way. The class name is a historical carry-over; if you
// wanted to rename to TEntity, this is the spot, but every game call site uses ConsoleEntity.

#pragma once

#include <Windows.h>
#include <stdint.h>
#include <vector>

class ConsoleEntity
{
public:
    ConsoleEntity();
    ConsoleEntity(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    ConsoleEntity(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);

    uint32_t GetX() const;
    uint32_t GetY() const;

    // Paint immediately at the time of call (no internal buffer; goes to the BearLibTerminal
    // back layer). Originally these populated a CHAR_INFO buffer that was flushed in Draw().
    void SetData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    void SetData(char asciiChar, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    void SetData(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    int  WriteData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute);
    int  WriteData(uint32_t x, uint32_t y, uint16_t attribute, const char* format, ...);
    void SetPosition(uint32_t x, uint32_t y);
    void SetAttribute(uint16_t attribute);

    // Flush the back layer to the window. HANDLE parameter retained for compatibility but unused
    // under BearLibTerminal. The game calls this once per frame.
    bool Draw(HANDLE consoleOutput) const;

private:
    uint32_t m_x = 0;
    uint32_t m_y = 0;
    uint32_t m_w = 0;
    uint32_t m_h = 0;

    // The last attribute set via SetAttribute(). Kept so that calls like
    //     m_console.SetAttribute(...) followed by something that relies on
    // a "current" attribute still work, but in practice the attribute is
    // always passed explicitly by the WriteData() callers.
    uint16_t m_attribute = 0x0007;
};
