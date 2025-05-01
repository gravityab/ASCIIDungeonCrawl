///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Monster.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Include Headers
#include "Actor.h"
#include "Animation.h"
#include "DamageTypes.h"
#include "Die.h"
#include "Weapon.h"

/// Enumeration of a list of monster categories
enum class MonsterFamily : uint8_t
{
	INVALID,
	GELATINOUS, ///< Represents gelatinous family
	RODENT,     ///< Represents rodent family
	UNDEAD,     ///< Represents undead family
	ARACHNID,   ///< Represents arachnid family
};

/// A structure representing a monster
struct Monster : public Actor
{
public:
	/// Constructor for a monster
	Monster();

	/// Constructor for a monster with predefined settings
	Monster(std::string name, Die maxHp, Die maxMp, DamageType weakness, int experience, int level, Animation attack, Animation attack2, Animation idle, Animation spawn, Weapon weapon1, Weapon weapon2, Weapon weapon3, Weapon weapon4);

	/// Get the type of actor
	ActorType GetType() override;

public:
	/// The name of the monster
	std::string name;

	/// Whether or not this monster is considered elite
	Rarity rarity = Rarity::COMMON;

	/// The experience to give the player upon defeating
	int experience = 0;

	/// The monster attack animation
	Animation attack1;

	/// The monster secondary attack
	Animation attack2;

	/// The monster spawn animation
	Animation spawn;

	/// The random maximum hp
	Die rollMaxHp;

	/// The random maximum mp
	Die rollMaxMp;

	/// Monster family
	MonsterFamily family;

	/// The element type of the monster
	DamageType element;

	/// The weakness of the monster
	DamageType weakness;

	/// Monster is currently attacking
	bool attacking1 = false;

	/// Monster is currently using alt attack
	bool attacking2 = false;

	/// Time to display spawn
	Time spawning = Time::Zero;
};