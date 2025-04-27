///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actor.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Weapon.h"

/// Standard Template Library Headers
#include <stdint.h>

/// Enumeration of all types that inherit from actor
enum class ActorType : uint8_t
{
	ACTOR_INVALID,
	ACTOR_HERO,    ///< A hero actor
	ACTOR_MONSTER, ///< A monster actor
};

/// Structure representing a base actor class
struct Actor
{
public:
	/// Get the type of actor
	virtual ActorType GetType() = 0;

public:
	/// The x position of the actor
	int x;

	/// The y position of the actor
	int y;

	/// The current hp of the actor
	int currentHp = 0;

	/// The total hp of the actor
	int totalHp = 0;

	/// The current mp of the actor
	int currentMp = 0;

	/// The total mp of the actor
	int totalMp = 0;

	/// The level of the actor
	int level = 1;

	/// The idle animation
	Animation idle;

	/// The dead animation
	Animation dead;

	/// THe level animation
	Animation levelUp;

	/// The selected animation
	Animation selected;

	/// The primary weapon of the actor
	Weapon weapon1;

	/// The secondary weapon of the actor
	Weapon weapon2;

	/// The third weapon of the actor
	Weapon weapon3;

	/// The fourth weapon of the actor
	Weapon weapon4;

	/// The armor the actor is wearing
	Weapon armor;

	/// Amount of time to display level up animation
	Time levelUpTimeLeft = Time::Zero;
};
