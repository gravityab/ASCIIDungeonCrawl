///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all input within game
enum class Button : uint16_t
{
	BUTTON_UP,     ///< Move up
	BUTTON_DOWN,   ///< Move down
	BUTTON_LEFT,   ///< Move left
	BUTTON_RIGHT,  ///< Move right
	BUTTON_SELECT, ///< Select option
	BUTTON_BACK,   ///< Back
	BUTTON_TOGGLE, ///< Toggle hero view
	BUTTON_COUNT   ///< Number of buttons
};
constexpr int s_ButtonCount = static_cast<int>(Button::BUTTON_COUNT);

/// Structure representing a button state
struct ButtonState
{
	/// Constructor for a button state
	ButtonState();

	/// Constructor for a button state
	ButtonState(int map);

public:
	/// Whether the button is considered pressed
	bool pressed = false;

	/// Whether the button is considered released this frame
	bool released = false;

	/// The mapping of the button to check
	int map;
};

/// Class representing input and button states
class Input
{
public:
	/// Initialize an input manager
	void Initialize();

	/// Poll the input
	void Poll();

	/// Check whether a button is being held down this frame
	bool Pressed(Button button);

	/// Check whether a button has been released this frame
	bool Released(Button button);

private:
	/// Poll the current button state
	void PollButtonState(ButtonState& state);

private:
	ButtonState m_buttons[s_ButtonCount] = {};
};