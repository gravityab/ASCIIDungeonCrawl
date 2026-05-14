///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entity.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Entity.h"

#include "BearLibTerminal.h"
#include "EngineColor.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

// Render one cell with foreground glyph and an optional background block underneath.
// '.' is treated as transparent (the original buffer used '.' as a "leave this cell alone" sentinel
// inside SetData/WriteData region writes; preserved here).
//
// Background and blink are both visualised by drawing a FULL_BLOCK behind the glyph, since
// BearLibTerminal cells don't have a separate background colour like the Win32 console did.
//   * background bits set  -> block in ToBackgroundColor(), glyph in ToColor()
//   * blink bits set       -> block in ToColor(),           glyph in palette[0] (black)
// (Blink takes precedence when both are present, matching the original BLINK macro's intent of
// drawing maximum-contrast highlight.)
static void PaintCell(int x, int y, int glyph, uint16_t attribute)
{
    if (glyph == (int)'.' || glyph == 0)
        return;

    if (HasBlink(attribute))
    {
        terminal_color(ToColor(attribute));
        terminal_put(x, y, FULL_BLOCK);
        terminal_color(EngineColor::s_palette[0]); // glyph in black against the highlight
        terminal_put(x, y, glyph);
        return;
    }

    if (HasBackground(attribute))
    {
        terminal_color(ToBackgroundColor(attribute));
        terminal_put(x, y, FULL_BLOCK);
    }
    terminal_color(ToColor(attribute));
    terminal_put(x, y, glyph);
}

// --------------------------------------------------------------------------------------------------------------------
ConsoleEntity::ConsoleEntity()
{}

ConsoleEntity::ConsoleEntity(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    SetData(data, x, y, w, h, attribute);
}

ConsoleEntity::ConsoleEntity(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    SetData(data, x, y, w, h, attribute);
}

uint32_t ConsoleEntity::GetX() const { return m_x; }
uint32_t ConsoleEntity::GetY() const { return m_y; }

void ConsoleEntity::SetData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_attribute = attribute;

    const int len = data ? (int)strlen(data) : 0;
    int pos = 0;
    for (uint32_t dy = 0; dy < h; dy++)
    {
        for (uint32_t dx = 0; dx < w; dx++)
        {
            int glyph = (pos < len) ? (int)(unsigned char)data[pos] : 0;
            pos++;
            PaintCell((int)(x + dx), (int)(y + dy), glyph, attribute);
        }
    }
}

void ConsoleEntity::SetData(const wchar_t* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_attribute = attribute;

    const int len = data ? (int)wcslen(data) : 0;
    int pos = 0;
    for (uint32_t dy = 0; dy < h; dy++)
    {
        for (uint32_t dx = 0; dx < w; dx++)
        {
            int glyph = (pos < len) ? (int)data[pos] : 0;
            pos++;
            PaintCell((int)(x + dx), (int)(y + dy), glyph, attribute);
        }
    }
}

void ConsoleEntity::SetData(char asciiChar, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_attribute = attribute;

    // Game uses this with ' ' to "clear" the screen at the top of each frame. For a literal
    // clear we could call terminal_clear() but going through PaintCell handles the
    // (extremely rare) case of clearing with a non-space char + non-zero background.
    if (asciiChar == ' ' && !HasBackground(attribute))
    {
        terminal_clear();
        return;
    }

    for (uint32_t dy = 0; dy < h; dy++)
    {
        for (uint32_t dx = 0; dx < w; dx++)
        {
            PaintCell((int)(x + dx), (int)(y + dy), (int)(unsigned char)asciiChar, attribute);
        }
    }
}

void ConsoleEntity::SetPosition(uint32_t x, uint32_t y)
{
    m_x = x;
    m_y = y;
}

void ConsoleEntity::SetAttribute(uint16_t attribute)
{
    m_attribute = attribute;
}

int ConsoleEntity::WriteData(const char* data, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t attribute)
{
    if (!data)
        return 0;
    const int len = (int)strlen(data);

    int pos = 0;
    for (uint32_t dy = 0; dy < h; dy++)
    {
        for (uint32_t dx = 0; dx < w; dx++)
        {
            int glyph = (pos < len) ? (int)(unsigned char)data[pos] : 0;
            pos++;
            PaintCell((int)(x + dx), (int)(y + dy), glyph, attribute);
        }
    }
    return len;
}

int ConsoleEntity::WriteData(uint32_t x, uint32_t y, uint16_t attribute, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[256] = { '\0' };
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    const int len = (int)strlen(buffer);
    for (int i = 0; i < len; i++)
    {
        PaintCell((int)x + i, (int)y, (int)(unsigned char)buffer[i], attribute);
    }
    return len;
}

bool ConsoleEntity::Draw(HANDLE /*unused*/) const
{
    terminal_refresh();
    return true;
}
