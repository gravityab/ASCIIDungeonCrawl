///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DamageType.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "DamageTypes.h"

uint16_t ToAttribute(DamageType type)
{
	uint16_t attribute = 0x0007;
	switch (type)
	{
		case DamageType::COLD:      attribute = 0x000B; break;
		case DamageType::FIRE:      attribute = 0x0004; break;
		case DamageType::LIGHTNING: attribute = 0x0009; break;
		case DamageType::NECROTIC:  attribute = 0x000A; break;
		case DamageType::NORMAL:    attribute = 0x0007; break;
		case DamageType::POISON:    attribute = 0x0002; break;
		case DamageType::PSYCHIC:   attribute = 0x000D; break;
		case DamageType::HEALING:   attribute = 0x0004; break;
		case DamageType::PROTECT:   attribute = 0x000C; break;
		case DamageType::HOLY:      attribute = 0x000E; break;
		case DamageType::STEEL:     attribute = 0x000F; break;
		case DamageType::DARK:      attribute = 0x0008; break;
		case DamageType::WATER:     attribute = 0x0001; break;
		default:
			attribute = 0x0007;
			break;
	}
	return attribute;
}

DamageType ToWeakness(DamageType type)
{
	switch (type)
	{
		case DamageType::COLD:      return DamageType::FIRE;
		case DamageType::FIRE:      return DamageType::WATER;
		case DamageType::LIGHTNING: return DamageType::COLD;
		case DamageType::NECROTIC:  return DamageType::HOLY;
		case DamageType::POISON:    return DamageType::STEEL;
		case DamageType::PSYCHIC:   return DamageType::DARK;
		case DamageType::HEALING:   return DamageType::HEALING;
		case DamageType::PROTECT:   return DamageType::PROTECT;
		case DamageType::HOLY:      return DamageType::NECROTIC;
		case DamageType::STEEL:     return DamageType::NORMAL;
		case DamageType::DARK:      return DamageType::PSYCHIC;
		case DamageType::WATER:     return DamageType::LIGHTNING;
	default:
		return DamageType::NORMAL;
	}
}

std::string ToConditionString(DamageType type)
{
	switch (type)
	{
		case DamageType::COLD:      return "FREEZING";
		case DamageType::FIRE:      return "BURNING";
		case DamageType::LIGHTNING: return "SHOCKED";
		case DamageType::NECROTIC:  return "DECAYING";
		case DamageType::POISON:    return "POISONED";
		case DamageType::PSYCHIC:   return "CONFUSED";
		case DamageType::HEALING:   return "HEALING";
		case DamageType::PROTECT:   return "PROTECT";
		case DamageType::HOLY:      return "SMITTEN";
		case DamageType::STEEL:     return "BLEEDING";
		case DamageType::DARK:      return "CORRUPTED";
		case DamageType::WATER:     return "DROWNING";
		default:
			return "";
	}
}

std::string ToString(DamageType type)
{
	switch (type)
	{
		case DamageType::COLD:      return "Frost";
		case DamageType::FIRE:      return "Fire";
		case DamageType::LIGHTNING: return "Lightning";
		case DamageType::NECROTIC:  return "Necrotic";
		case DamageType::POISON:    return "Poison";
		case DamageType::PSYCHIC:   return "Psychic";
		case DamageType::HEALING:   return "Healing";
		case DamageType::PROTECT:   return "Protect";
		case DamageType::HOLY:      return "Holy";
		case DamageType::STEEL:     return "Steel";
		case DamageType::DARK:      return "Dark";
		case DamageType::WATER:     return "Hydro";
		default:
			return "None";
	}
}
