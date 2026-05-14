///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EngineColor.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)
//
// Placeholder palette. Tune these RGB values by eye while playing; the engine reads them through
// ToColor() / ToBackgroundColor() in EngineColor.h.

#include "EngineColor.h"

namespace EngineColor
{
    color_t s_palette[16] =
    {
        color_from_argb(255,   0,   0,   0), // 0x0 Black
        color_from_argb(255,   0,   0, 170), // 0x1 Dark Blue
        color_from_argb(255,   0, 170,   0), // 0x2 Dark Green
        color_from_argb(255,   0, 170, 170), // 0x3 Dark Cyan
        color_from_argb(255, 170,   0,   0), // 0x4 Dark Red
        color_from_argb(255, 170,   0, 170), // 0x5 Dark Magenta
        color_from_argb(255, 170,  85,   0), // 0x6 Dark Yellow / Brown
        color_from_argb(255, 192, 192, 192), // 0x7 Light Gray (default fg)
        color_from_argb(255, 128, 128, 128), // 0x8 Dark Gray
        color_from_argb(255,  85,  85, 255), // 0x9 Bright Blue
        color_from_argb(255,  85, 255,  85), // 0xA Bright Green
        color_from_argb(255,  85, 255, 255), // 0xB Bright Cyan
        color_from_argb(255, 255,  85,  85), // 0xC Bright Red
        color_from_argb(255, 255,  85, 255), // 0xD Bright Magenta
        color_from_argb(255, 255, 255,  85), // 0xE Bright Yellow
        color_from_argb(255, 255, 255, 255), // 0xF Bright White
    };
}
