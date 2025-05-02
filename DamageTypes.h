///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DamageType.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>
#include <string>

/// Enumeration of all damage types available
enum class DamageType : uint8_t
{
	INVALID,
	NORMAL,    ///< Non-elemental damage MORE damage to STEEL           <-|
	COLD,      ///< Cold damage          MORE damage to LIGHTNING         | <-|
	FIRE,      ///< Fire damage          MORE damage to COLD              | <-|
	LIGHTNING, ///< Lightning damage     MORE damage to WATER             | <-|
	WATER,     ///< Water damage         MORE damage to FIRE              | <-|
	NECROTIC,  ///< Necrotic damage      MORE damage to HOLY        <-|   |
	PSYCHIC,   ///< Psychic damage       MORE damage to DARK    <-|   |   |
	DARK,      ///< Dark damage          MORE damage to PSYCHIC <-|   |   |
	POISON,    ///< Poison damage        MORE damage tp NORMAL        | <-|
	STEEL,     ///< Steel damage         MORE damage to POISON        | <-|
	HOLY,      ///< Holy damage          MORE damage to NECROTIC    <-|
	HEALING,   ///< Healing
	PROTECT,   ///< Protection
};

/// Convert a damage type to an attribute color
uint16_t ToAttribute(DamageType type);

/// Convert a damage type to the weakness of damage type
DamageType ToWeakness(DamageType type);

/// Convert a damage type to a condition string
std::string ToConditionString(DamageType type);

/// Convert a damage type to a string
std::string ToString(DamageType type);
