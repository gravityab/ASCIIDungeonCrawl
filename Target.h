///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Target.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all available targets for a weapon
enum class Target : uint8_t
{
	INVALID,
	NEWHERO,          ///< Targets new hero
	ENEMY,            ///< Targets only the enemy
	MONSTER_RECRUIT,  ///< Monster recruited
	MONSTER_CONDITION, ///< Monster gives condition
	MONSTER_SPLIT,    ///< Monster spawns lower level monster on death
	MONSTER_ALLENEMIES, /// Monster hits all enemies
	MONSTER_SWITCH,   ///< Monster switches positions
	MONSTER_DRAINMP,  ///< Monster drains mp
	ALLENEMIES,       ///< Targets all enemies
	PLAYERLEVEL,      ///< Targets hero level
	PLAYERLEVEL5,     ///< Targets hero level
	PLAYERAC_SPELL,   ///< Targets hero armor
	PLAYERAC_SPEED,   ///< Targets hero armor
	PLAYERAC_SLOW,    ///< Targets hero armor
	PLAYERHP_REUSE,   ///< Targets hero hp
	PLAYERHP_CONSUME, ///< Targets hero hp and is consumed on use
	PLAYERHP_REVIVE_CONSUME, ///< Targets dead heroes
	PLAYER_PROTECT,
	PLAYER_PROTECTALL,
	ALLPLAYERSHP,     ///< Targets all heroes hp
	PLAYERMP_REUSE,   ///< Targets hero mp
	PLAYERMP_CONSUME, ///< Targets hero mp and is consumed on use
	ALLPLAYERSMP,     ///< Targets all heroes mp
};