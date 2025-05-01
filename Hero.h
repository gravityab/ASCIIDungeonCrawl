///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hero.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Include Headers
#include "Actor.h"
#include "Animation.h"
#include "Die.h"

/// Structure representing a hero
struct Hero : public Actor
{
public:
	/// Constructor
	Hero();

	/// Get the actor type
	ActorType GetType() override;

public:
	/// The current index of the hero
	int index = 0;

	/// Experience of hero
	int experience = 0;

	/// Whether its the heroes turn
	bool isTurn = false;

	/// The damage condition sate
	Die condition = Die(0, 0, 0);

	/// Turns left to apply condition
	int conditionTurnsLeft = 0;

	/// Disabled animation
	Animation disabled;

	/// The natural AC gained from bonuses
	int bonusAC = 0;
};
