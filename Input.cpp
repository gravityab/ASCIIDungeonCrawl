///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "Input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include "BearLibTerminal.h"

// --------------------------------------------------------------------------------------------------------------------
ButtonState::ButtonState()
{}

ButtonState::ButtonState(int map_)
{
    map = map_;
}

// --------------------------------------------------------------------------------------------------------------------
void Input::Initialize()
{
    m_closed = false;
    m_buttons[(int)Button::BUTTON_UP].map = VK_UP;
    m_buttons[(int)Button::BUTTON_DOWN].map = VK_DOWN;
    m_buttons[(int)Button::BUTTON_LEFT].map = VK_LEFT;
    m_buttons[(int)Button::BUTTON_RIGHT].map = VK_RIGHT;
    m_buttons[(int)Button::BUTTON_SELECT].map = VK_RETURN;
    m_buttons[(int)Button::BUTTON_BACK].map = VK_BACK;
    m_buttons[(int)Button::BUTTON_TOGGLE].map = VK_RSHIFT;
    m_buttons[(int)Button::BUTTON_PASSIVES].map = VK_RCONTROL;
    m_buttons[(int)Button::BUTTON_MENU].map = VK_ESCAPE;
}

void Input::Poll()
{
    for (int index = 0; index < (int)Button::BUTTON_COUNT; index++)
    {
        PollButtonState(m_buttons[index]);
    }

    // Drain BearLibTerminal's event queue so window close / OS events propagate. We only need
    // TK_CLOSE and TK_RESIZED here; keystrokes are still read via GetAsyncKeyState above to
    // preserve the original behavior. Other events are discarded.
    while (terminal_has_input())
    {
        int value = terminal_read();
        if (value == TK_CLOSE)
        {
            m_closed = true;
        }
        else if (value == TK_RESIZED)
        {
            // Keep the game's 120x30 logical grid by rescaling glyph cells to exactly fill the
            // user's new window size. terminal_state(TK_WIDTH/HEIGHT) reports the new grid in
            // cells (BearLibTerminal grew/shrank it to fit the dragged window at the old cell
            // pixel size); multiply by the current pixel-per-cell to get the raw pixel
            // dimensions of the window, then divide by 120x30 to get the new cell pixel size.
            //
            // Important: setting window.cellsize alone keeps the cell *area* large but lets the
            // (bitmap) glyph stay small. To actually scale the glyph we re-set the TTF font at
            // the new size; BearLibTerminal renders the TTF at whatever cell size we request.
            const int cellsW   = terminal_state(TK_WIDTH);
            const int cellsH   = terminal_state(TK_HEIGHT);
            const int cellPxW  = terminal_state(TK_CELL_WIDTH);
            const int cellPxH  = terminal_state(TK_CELL_HEIGHT);
            const int pixelW   = cellsW * cellPxW;
            const int pixelH   = cellsH * cellPxH;

            int desiredCellW = pixelW / 120;
            int desiredCellH = pixelH / 30;
            if (desiredCellW < 2) desiredCellW = 2;
            if (desiredCellH < 2) desiredCellH = 2;

            char buf[128];
            _snprintf_s(buf, _TRUNCATE,
                "font: Resources/Consolas-Regular.ttf, size=%dx%d",
                desiredCellW, desiredCellH);
            terminal_set(buf);
            terminal_set("window.size=120x30");
        }
    }

    // (Previously had a GetForegroundWindow/PID guard here to ignore inputs when the window isn't
    //  in focus. It was incorrectly returning "not active" for the BearLibTerminal window, which
    //  blocked all input. Removed; phantom inputs from background focus are a minor issue we can
    //  add back later if it bites.)
}

bool Input::Pressed(Button button)
{
    return m_buttons[(int)button].pressed;
}

bool Input::Released(Button button)
{
    return m_buttons[(int)button].released;
}

// --------------------------------------------------------------------------------------------------------------------
void Input::PollButtonState(ButtonState& state)
{
    // Get the async button state of the mapped button
    if (GetAsyncKeyState(state.map) & 0x8000)
    {
        state.pressed = true;
    }
    if (state.released)
    {
        state.released = false;
    }
    if (state.pressed && !(GetAsyncKeyState(state.map) & 0x8000))
    {
        state.pressed = false;
        state.released = true;
    }
}
