///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CursorStack.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Actor.h"
#include "Animation.h"
#include "Weapon.h"

/// Standard Template Libray Header
#include <vector>

/// An enumeration of all the valid cursor states
enum class CursorState
{
	ROOT,
	DOOR,        ///< Select a doors
	HERO,        ///< Select a hero
	HERO_ITEM,   ///< Select an item
	ATTRIBUTES,  ///< Select a hero / display attributes
	INVENTORY_1, ///< Select a weapon
	INVENTORY_2, ///< Select a weapon
	USE_ITEM,    ///< Use a consumable item or item that consumes MP
	SHOP,        ///< Select a shop item
	SHOP_HERO,   ///< Select a hero for a shop item
	SHOP_HERO_ITEM, ///< Select a weapon to replace
	COMBAT,      ///< Select an item from current hero
	COMBAT_MONSTERS, ///< Select a monster
	COMBAT_HERO,
	CHEST,       ///< Select opening a chest
	REWARD,      ///< Select accepting a reward
	REWARD_HERO,
	REWARD_HERO_ITEM,
	FOUNTAIN,    ///< Select the fountain
	TRAP,        ///< Select roll for trap
	TRAP_INITIATED,
	ITEM_USED,   ///< Animation to display item result
};

enum class CursorIndexDirection
{
	HORIZONTAL,
	VERTICAL,
};

// Context of a a UI state
struct CursorContext
{
	/// The state of the context
	CursorState state = CursorState::ROOT;

	/// The animation of the cursor
	Animation cursor;

	/// The source actor
	Actor* source = nullptr;

	/// The target actor
	Actor* target = nullptr;

	/// The target weapon
	Weapon* weapon = nullptr;

	/// The last known index of the state
	int index = 0;

	/// The max index to stay clamped within
	int maxIndex = 0;

	/// The min index to stay clamped within
	int minIndex = 0;

	/// List of indexes to exclude or skip
	std::vector<int> excludeIndexes{};

	/// State direction
	CursorIndexDirection direction = CursorIndexDirection::HORIZONTAL;

	/// Amount of time to display
	Time displayTime = Time::Zero;

	/// Display attribute
	uint16_t displayAttribute = 0x0007;

	/// Display value rolled
	int displayAmount = 0;
};

// A class representing a stack of UI menu states
// When UI requires a list of stacking decision trees, keep track of the last states
// to allow the player to move forwards and backwards through the UI while retaining context
// of where the player has navigated to before
class CursorStack
{
public:
	CursorStack();

	/// Get the latest context in the stack
	CursorContext& GetContext();

	/// Get the latest context state
	CursorState GetState();

	/// Get the lastest cursor animation
	Animation& GetAnimation();

	/// Get the direction of the index
	CursorIndexDirection GetDirection();

	/// Get the cursor index
	int GetCursorIndex();

	/// Increment the cursor index 
	void IncrementCursorIndex();

	/// Decrement the cursor index
	void DecrementCursorIndex();

	/// Push a context back at the end
	void PushBack(CursorContext context);

	/// Pop a context from the end
	void PopBack(int limit);

	/// Pop back until root
	void PopBackTo(int index);

	/// Clear the cursor stack
	void Clear();

private:
	/// Clamp cursor index by min max and valid options
	void ClampCursorIndex(int direction);

private:
	/// The decision stack of cursors
	std::vector<CursorContext> m_contexts;
};

/// Convert a cursor state to string
std::string ToString(CursorState state);