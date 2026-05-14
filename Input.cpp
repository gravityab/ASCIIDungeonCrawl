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
    // TK_CLOSE here; keystrokes are still read via GetAsyncKeyState above to preserve the original
    // behaviour. Other events are discarded.
    while (terminal_has_input())
    {
        int value = terminal_read();
        if (value == TK_CLOSE)
        {
            m_closed = true;
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
