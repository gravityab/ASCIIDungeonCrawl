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

	/// Whether this action is the hero's first swing of the turn against the
	/// target. One-per-turn on-hit passives (e.g. GLOVES_BASH, DAGGER_BLEED)
	/// only roll when this is true. Passives that replicate actions (BOXER,
	/// DUALWIELD, MULTIATTACK, FINESSE) clear it on their follow-up swings.
	bool firstSwing = true;
};