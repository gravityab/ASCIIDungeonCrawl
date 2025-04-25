///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// State.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all the states within a dungeon crawl game
enum class State : uint8_t
{
	INVALID,
	STATE_MAIN,        ///< The main menu screen
	STATE_NEXT_FLOOR,  ///< Increment the floor
	STATE_DOORS,       ///< Display the doors screen
	STATE_SHOP,        ///< Display the shop screen
	STATE_COMBAT,      ///< Display the comabat screen
	STATE_COMBAT_RESOLVE, ///< Resolve the combat turn
	STATE_TREASURE,    ///< Display the treasure screen
	STATE_FOUNTAIN,    ///< Display fairy room
	STATE_TRAP_SPIKES, ///< Spike trap
};

