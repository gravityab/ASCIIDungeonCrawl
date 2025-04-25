///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dungeon.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Animation.h"
#include "DamageTypes.h"
#include "Database.h"
#include "Die.h"
#include "Monster.h"
#include "Rarity.h"
#include "Reward.h"
#include "State.h"
#include "Weapon.h"

/// Standard Template Library Headers
#include <vector>

/// Structure representing a door in a dungeon
struct Door
{
	/// Whether the door is opened
	bool opened = false;

	/// The label of the door to display overhead
	std::string label;

	/// The attribute of the door to draw
	uint16_t attribute = 0x0000;

	/// The rarity of the door
	Rarity rarity = Rarity::INVALID;

	/// The state to transition to upon opening
	State state = State::INVALID;

	/// The animation for an open door
	Animation open;

	/// The animation for a closed door
	Animation close;
};

/// Structure representing the contents of a room behind a door
struct Room
{
	/// The door representing this room
	Door door;

	/// The list of monsters in the room
	std::vector<Monster> monsters;

	/// The list of shop weapons in the room
	std::vector<Weapon> shop;

	/// The reward type for beating the monsters
	Reward reward;

	/// The reward weapon
	Weapon rewardWeapon;

	/// The reward armor
	Weapon rewardArmor;

	/// The reward gold
	int gold = 0;

	/// Whether this room is a boss or greater threat
	bool boss = false;

	/// The back
	DamageType roomAttribute;
};

/// Structore representing a floor in a dungeon
struct Floor
{
	/// The floor damage theme
	DamageType type;

	/// The prevelant family of monsters on floor
	MonsterFamily family;

	/// Whether the floor contains a boss
	bool boss = false;

	/// Whether the floor has a higher chance for shops
	bool shopTable = false;

	/// The floor number of the floor
	int floorNumber = 0;

	/// The rarity of the floor all of its calculations were based on
	Rarity rarity = Rarity::INVALID;

	/// The rooms on the floor with doors
	std::vector<Room> rooms;

	/// The list of doors within the floor
	std::vector<Door*> doors;
};

/// Class representing a full playable dungeon
class Dungeon
{
public:
	/// Constructor for a dungeon
	Dungeon();

	/// Ininitalize the dungeo
	void Initialize();

	/// Generate a new dungeon
	void Generate();

	/// Reset the dungeon and the database
	void Reset();

	/// Get a floor within a dungeon
	Floor& GetFloor(int floor);

	/// Get an instance of the database
	Database& GetDatabase();

private:
	/// Generate a floor at an index
	Floor GenerateFloor(int floor, DamageType attribute, MonsterFamily family, Rarity rarity, bool boss, bool shop);

private:
	/// The number of floors
	std::vector<Floor> m_dungeon;

	/// The database
	Database m_db;
};