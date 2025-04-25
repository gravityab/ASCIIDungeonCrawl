///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rarity.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "Rarity.h"

// --------------------------------------------------------------------------------------------------------------------
uint16_t ToAttribute(Rarity type)
{
	uint16_t attribute = 0x0007;
	switch (type)
	{
		case Rarity::COMMON: attribute = 0x0007; break;
		case Rarity::RARE: attribute = 0x0009; break;
		case Rarity::EPIC: attribute = 0x0005; break;
		case Rarity::LEGENDARY: attribute = 0x0004; break;
		default: attribute = 0x0007; break;
	}
	return attribute;
}

std::string ToString(Rarity rarity)
{
	if (rarity == Rarity::COMMON)
		return "Common";
	else if (rarity == Rarity::RARE)
		return "Rare";
	else if (rarity == Rarity::EPIC)
		return "Epic";
	else if (rarity == Rarity::LEGENDARY)
		return "Legendary";
	else
		return std::string();
}