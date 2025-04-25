///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Action.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Actor.h"
#include "Weapon.h"

/// A structure representing a combat action
struct Action
{
	/// The source of the combat action
	Actor* source = nullptr;

	/// The weapon used as the combat action
	Weapon* weapon = nullptr;

	/// The target or targets of the combat action
	std::vector<Actor*> targets;

	/// The amount of time to animate attacks
	Time timeLeft = ToMilliseconds(500);
};