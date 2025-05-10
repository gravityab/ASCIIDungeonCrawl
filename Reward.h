///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reward.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all reward types
enum class Reward
{
    INVALID,
    MONEY,            ///< Standard amount of gold
    RARE_WEAPON,      ///> Rare weapon
    RARE_MONEY,       ///> A larger sum of gold
    EPIC_WEAPON,      ///> Epic weapon
    EPIC_MONEY,       ///> A very large sum of gold
    LEGENDARY_WEAPON, ///> Legendary weapon
    LEGENDARY_MONEY,  ///> A horde of treasure
};
