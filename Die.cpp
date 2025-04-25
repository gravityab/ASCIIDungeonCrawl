///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Die.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Die.h"

/// Dungeon Crawl Library Header
#include "Entity.h"
#include "Random.h"

// --------------------------------------------------------------------------------------------------------------------
Die operator+(const Die& left, const Die& right)
{
	Die newDie;
	newDie.multiplier = left.multiplier + right.multiplier;
	newDie.die = left.die;
	newDie.constant = left.constant + right.constant;
	newDie.type = left.type;
	return newDie;
}

// --------------------------------------------------------------------------------------------------------------------
DieRange::DieRange()
{}

/// Constructor for a die range with predefined modifiers
DieRange::DieRange(
	int minMult_,
	int maxMult_,
	int minDie_,
	int maxDie_,
	int minConst_,
	int maxConst_,
	int minSpeed_,
	int maxSpeed_,
	int minMp_,
	int maxMp_,
	int minAc_,
	int maxAc_,
	Die cost_,
	DamageType type_)
{
	minMult = minMult_;
	maxMult = maxMult_;
	minDie = minDie_;
	maxDie = maxDie_;
	minConst = minConst_;
	maxConst = maxConst_;
	minSpeed = minSpeed_;
	maxSpeed = maxSpeed_;
	minMp = minMp_;
	maxMp = maxMp_;
	minAc = minAc_;
	maxAc = maxAc_;
	cost = cost_;
	type = type_;
}

Die DieRange::GetDie()
{
	Die die;
	die.multiplier = GetRandomValue(minMult, maxMult);
	die.die = GetRandomValue(minDie, maxDie);
	die.constant = GetRandomValue(minConst, maxConst);
	die.type = type;
	return die;
}

Die DieRange::GetDie(DamageType type)
{
	Die die;
	die.multiplier = GetRandomValue(minMult, maxMult);
	die.die = GetRandomValue(minDie, maxDie);
	die.constant = GetRandomValue(minConst, maxConst);
	die.type = type;
	return die;
}

Die DieRange::GetSpeed()
{
	Die die;
	die.multiplier = 0;
	die.die = 0;
	die.constant = GetRandomValue(minSpeed, maxSpeed);
	return die;
}

Die DieRange::GetMp()
{
	Die die;
	die.multiplier = 0;
	die.die = 0;
	die.constant = GetRandomValue(minMp, maxMp);
	return die;
}

Die DieRange::GetAC()
{
	Die die;
	die.multiplier = 0;
	die.die = 0;
	die.constant = GetRandomValue(minAc, maxAc);
	return die;
}

Die DieRange::GetCost()
{
	return cost;
}

int DieRange::GetAvg()
{
	int total = 0;
	for (int index = 0; index < 100; index++)
		total += cost.Roll();
	return static_cast<int>((double)total / (double)100);
}

DieRange DieRange::As(Rarity rarity)
{
	DieRange newRange = *this;
	if (rarity >= Rarity::RARE)
	{
		newRange.minMult += 2;
		newRange.maxMult += 2;
		newRange.minConst += 2;
		newRange.maxConst += 2;
		newRange.cost.multiplier += 4;
		newRange.cost.constant += 4;
		newRange.minAc += 1;
		newRange.maxAc += 1;
		if (newRange.minMp != 0)
			newRange.minMp += 4;
		if (newRange.maxMp != 0)
			newRange.maxMp += 4;
	}
	if (rarity >= Rarity::EPIC)
	{
		newRange.minMult += 4;
		newRange.maxMult += 4;
		newRange.minConst += 4;
		newRange.maxConst += 4;
		newRange.cost.multiplier += 8;
		newRange.cost.constant += 8;
		newRange.minAc += 2;
		newRange.maxAc += 2;
		if (newRange.minMp != 0)
			newRange.minMp += 8;
		if (newRange.maxMp != 0)
			newRange.maxMp += 8;
	}
	if (rarity >= Rarity::LEGENDARY)
	{
		newRange.minMult += 8;
		newRange.maxMult += 8;
		newRange.minConst += 4;
		newRange.maxConst += 4;
		newRange.cost.multiplier += 16;
		newRange.cost.constant += 16;
		newRange.minAc += 4;
		newRange.maxAc += 4;
		if (newRange.minMp != 0)
			newRange.minMp += 16;
		if (newRange.maxMp != 0)
			newRange.maxMp += 16;
	}
	return newRange;
}

// --------------------------------------------------------------------------------------------------------------------
Die::Die()
	: multiplier(0), die(0), constant(0), type(DamageType::INVALID)
{}

Die::Die(int multiplier_, int die_, int constant_, DamageType type_)
	: multiplier(multiplier_), die(die_), constant(constant_), type(type_)
{}

int Die::Roll(DamageType* type_)
{
	if (type_)
		*type_ = type;
	if (die == 0)
		return constant;
	return (multiplier * GetRandomValue(1, die)) + constant;
}

std::string Die::ToString()
{
	char buffer[16] = { '\0' };
	if (die == 0 && constant == 0)
		snprintf(buffer, 100, "N/A");
	else if (die == 0)
		snprintf(buffer, 100, "%d", constant);
	else
		snprintf(buffer, 100, "%dd%d + %d", multiplier, die, constant);
	return buffer;
}

int Die::WriteData(ConsoleEntity& entity, int x, int y)
{
	return entity.WriteData(x, y, ToAttribute(type), ToString().data());
}

int Die::WriteData(ConsoleEntity& entity, int x, int y, uint16_t attribute)
{
	return entity.WriteData(x, y, attribute, ToString().data());
}

// --------------------------------------------------------------------------------------------------------------------
/// Convert a die to a string
std::string ToString(Die die)
{
	return die.ToString();
}