///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Traps.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all the trap types available
enum class TrapType : int8_t
{
	INVALID = -1,
	TRAP_SPIKES,       ///< Spikes come from the ground
	TRAP_SWINGINGAXE,  ///< An axe swings
	//TRAP_GAS,        ///< Gas comes out of the walls
};