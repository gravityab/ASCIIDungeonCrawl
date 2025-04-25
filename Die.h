///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Die.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Include Headers
#include "DamageTypes.h"
#include "Rarity.h"

/// Engine Library Headers
#include "Entity.h"
#include "Random.h"

/// Structure for a polyhedral die
struct Die
{
public:
	/// Constructor for a die
	Die();

	/// Constructor for a die with values
	Die(int multiplier, int die, int constant = 0, DamageType type = DamageType::NORMAL);

	/// Perform a die roll and report a value back
	int Roll(DamageType* type = nullptr);

	/// Convert the polyhedra die to a string representation
	std::string ToString();

	/// Write the string representation of this die to a console entity
	int WriteData(ConsoleEntity& entity, int x, int y);

	/// Write the string representation of this die to a console entity overriding the color
	int WriteData(ConsoleEntity& entity, int x, int y, uint16_t attribute);

public:
	/// The type of polyhedral die
	int die = 0;

	/// The number of dice
	int multiplier = 0;

	/// The constant value to add post die roll
	int constant = 0;

	/// The damage type associated with the die
	DamageType type = DamageType::INVALID;
};

Die operator+(const Die& left, const Die& right);

/// Structure for creating random dice rolls
struct DieRange
{
public:
	/// Constructor for a die range
	DieRange();

	/// Constructor for a die range with predefined modifiers
	DieRange(
		int minMult,
		int maxMult,
		int minDie,
		int maxDie,
		int minConst,
		int maxConst,
		int minSpeed,
		int maxSpeed,
		int minMp,
		int maxMp,
		int minAc,
		int maxAc,
		Die cost,
		DamageType type = DamageType::NORMAL);

	/// Roll for a random die
	Die GetDie();

	/// Roll for a random die
	Die GetDie(DamageType type);

	/// Roll for a random speed die
	Die GetSpeed();

	/// Roll for a random mp cost
	Die GetMp();

	/// Roll for a random ac
	Die GetAC();

	/// Toll the cost
	Die GetCost();

	/// Get average cost
	int GetAvg();

	/// Upgrade dice by rarity
	DieRange As(Rarity rarity);

public:
	/// The minimum multiplier
	int minMult = 0;

	/// The maximum multiplier
	int maxMult = 0;

	/// The minimum die
	int minDie = 0;

	/// The maximum die
	int maxDie = 0;

	/// The min constant
	int minConst = 0;

	/// The max constant
	int maxConst = 0;

	/// The speed minimum
	int minSpeed = 0;

	/// The speed maximum
	int maxSpeed = 0;

	/// The mp minimum
	int minMp = 0;

	/// The mp maximum
	int maxMp = 0;

	/// The mp minimum
	int minAc = 0;

	/// The mp maximum
	int maxAc = 0;

	/// The random cost
	Die cost = Die(0, 0, 0);

	/// The damage type
	DamageType type;
};

/// Convert a die to a string
std::string ToString(Die die);

/// Helpers for rolling a die
#define ROLL(x, y, z) Die(x, y, z).Roll(nullptr);
#define ROLLT(x, y, z, t) Die(x, y, z).Roll(&t);
#define ROLLD4(x, z) Die(x, 4, z).Roll(nullptr);
#define ROLLD4T(x, z, t) Die(x, 4, z).Roll(&t);
#define ROLLTABLE(x) x[GetRandomValue(0, (int)x.size() - 1)]