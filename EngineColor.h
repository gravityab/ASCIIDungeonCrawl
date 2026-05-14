///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EngineColor.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)
//
// Placeholder mapping from the old Win32 console attribute scheme (16-color foreground in low
// nibble, background in high nibble, intensity/blink in upper bits) to BearLibTerminal ARGB
// color_t. Values here are best-effort defaults so the port runs visually similar to the original;
// tune them later by editing s_palette.

#pragma once

#include "BearLibTerminal.h"
#include <stdint.h>

typedef uint32_t color_t;

// Full-block glyph used to simulate background colors and blink underlay.
constexpr int FULL_BLOCK = 0x2588; // U+2588 FULL BLOCK

namespace EngineColor
{
    // 16-entry palette indexed by a Win32 console nibble (0..15). Defined in EngineColor.cpp
    // (as a plain extern array rather than a C++17 inline variable, to keep the project on its
    // default language standard).
    extern color_t s_palette[16];
}

// Map a 16-bit Win32 console attribute to an ARGB color for the foreground.
// Only the low nibble (bits 0..3) is considered; background/intensity ignored here.
inline color_t ToColor(uint16_t attribute)
{
    return EngineColor::s_palette[attribute & 0x0F];
}

// Map the background nibble (bits 4..7) of a Win32 attribute to an ARGB color.
inline color_t ToBackgroundColor(uint16_t attribute)
{
    return EngineColor::s_palette[(attribute >> 4) & 0x0F];
}

// True when the Win32 attribute has a non-zero background nibble. Callers that
// want a visible background should draw a FULL_BLOCK with ToBackgroundColor()
// before the actual glyph, because BearLibTerminal cells have no separate bg.
inline bool HasBackground(uint16_t attribute)
{
    return ((attribute >> 4) & 0x0F) != 0;
}

// True when the Win32 attribute has the "blink/reverse-video" bits set. The original BLINK(x)
// macro in DungeonCrawl.cpp ORs 0xC000 onto the attribute when in its "highlighted" half-cycle.
// We simulate this by drawing the FULL_BLOCK in the foreground color, masking the glyph.
inline bool HasBlink(uint16_t attribute)
{
    return (attribute & 0xC000) != 0;
}
