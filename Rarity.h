///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Rarity.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <stdint.h>
#include <string>

/// Enumeration of all objects with quality
enum class Rarity : uint8_t
{
	INVALID,
	COMMON,    ///< Common quality
	RARE,      ///< Rare quality
	EPIC,      ///< Epic quality
	LEGENDARY, ///< Legendary quality
};

/// Convert rarity to attribute
uint16_t ToAttribute(Rarity type);

/// Convert rarity to string
std::string ToString(Rarity rarity);