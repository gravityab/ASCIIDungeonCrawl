///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dungeon.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Dungeon.h"

/// Standard Template Library Headers
#include <list>

// --------------------------------------------------------------------------------------------------------------------
Dungeon::Dungeon()
{}

void Dungeon::Initialize()
{
	m_db.Initialize();
}

void Dungeon::Generate()
{
	//std::vector<Floor> dungeon;
	std::list<DamageType> attributes = { DamageType::COLD, DamageType::FIRE, DamageType::LIGHTNING, DamageType::NECROTIC, DamageType::POISON, DamageType::PSYCHIC, DamageType::HOLY, DamageType::STEEL, DamageType::DARK, DamageType::WATER };
	std::vector<MonsterFamily> themes = { MonsterFamily::GELATINOUS, MonsterFamily::RODENT, MonsterFamily::UNDEAD, MonsterFamily::ARACHNID };

	// First levels always start with normal theme
	DamageType attribute = DamageType::NORMAL;
	MonsterFamily family = GetRandomValue(0, 1) == 0 ? MonsterFamily::GELATINOUS : MonsterFamily::RODENT;

	int floorCount = 1;
	int themeCount = 12;
	while (themeCount-- > 0)
	{
		int roomCount = 5;
		while (roomCount-- > 0)
		{
			bool boss = floorCount % 10 == 0;
			int difficulty = (floorCount / 5) + 1;
			std::vector<Rarity> floorRarity = { Rarity::COMMON };
			if (difficulty > 4) { floorRarity.push_back(Rarity::LEGENDARY); floorRarity.push_back(Rarity::EPIC); floorRarity.push_back(Rarity::RARE); floorRarity.push_back(Rarity::COMMON); }
			if (difficulty > 3) { floorRarity.push_back(Rarity::EPIC); floorRarity.push_back(Rarity::RARE); floorRarity.push_back(Rarity::COMMON); }
			if (difficulty > 2) { floorRarity.push_back(Rarity::RARE); floorRarity.push_back(Rarity::COMMON); }
			if (difficulty > 1) { floorRarity.push_back(Rarity::COMMON); }
			std::vector<bool> shop = { false, false, false, true };

			Floor floor = GenerateFloor(floorCount, attribute, family, ROLLTABLE(floorRarity), boss, ROLLTABLE(shop));
			floorCount++;

			m_dungeon.push_back(floor);
		}

		// Select a new random attribute
		std::list<DamageType>::iterator iter = attributes.begin();
		std::advance(iter, GetRandomValue(0, (int)attributes.size() - 1));
		attribute = *iter;
		attributes.erase(iter); // Remove the dungeon theme from the list

		// Reset the dungeon floor attributes
		if (attributes.size() == 0)
			attributes = { DamageType::COLD, DamageType::FIRE, DamageType::LIGHTNING, DamageType::NECROTIC, DamageType::POISON, DamageType::PSYCHIC, DamageType::HOLY, DamageType::STEEL, DamageType::DARK, DamageType::WATER };

		// Select a new random theme
		family = themes[GetRandomValue(0, (int)themes.size() - 1)];
	}
}

void Dungeon::Reset()
{
	m_dungeon.clear();
	m_db.Reset();
}

Floor& Dungeon::GetFloor(int floor)
{
	return m_dungeon[floor];
}

Database& Dungeon::GetDatabase()
{
	return m_db;
}

// --------------------------------------------------------------------------------------------------------------------
Floor Dungeon::GenerateFloor(int floor, DamageType attribute, MonsterFamily family, Rarity rarity, bool boss, bool shop)
{
	Floor newFloor;
	newFloor.floorNumber = floor;
	newFloor.rarity = rarity;
	newFloor.type = attribute;
	newFloor.family = family;
	newFloor.boss = boss;
	newFloor.shopTable = shop;

	// Select the door rarity
	std::list<Rarity> doorRarity = { Rarity::COMMON, Rarity::COMMON, Rarity::COMMON, Rarity::RARE };
	if (rarity >= Rarity::LEGENDARY) { doorRarity.push_back(Rarity::COMMON); doorRarity.push_back(Rarity::RARE); doorRarity.push_back(Rarity::EPIC); doorRarity.push_back(Rarity::LEGENDARY); }
	if (rarity >= Rarity::EPIC) { doorRarity.push_back(Rarity::COMMON); doorRarity.push_back(Rarity::COMMON); doorRarity.push_back(Rarity::RARE); doorRarity.push_back(Rarity::RARE); doorRarity.push_back(Rarity::EPIC); doorRarity.push_back(Rarity::EPIC); doorRarity.push_back(Rarity::LEGENDARY); }
	if (rarity >= Rarity::RARE) { doorRarity.push_back(Rarity::COMMON); doorRarity.push_back(Rarity::COMMON); doorRarity.push_back(Rarity::RARE); doorRarity.push_back(Rarity::RARE); doorRarity.push_back(Rarity::EPIC); }

	std::list<State> nextState = { State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_SHOP, State::STATE_TRAP };
	if (shop) nextState = { State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_COMBAT, State::STATE_SHOP, State::STATE_SHOP, State::STATE_SHOP, State::STATE_SHOP, State::STATE_SHOP, State::STATE_FOUNTAIN };
	std::vector<uint16_t> attributes = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F };
	std::vector<DamageType> damageTypes = { DamageType::COLD, DamageType::FIRE, DamageType::LIGHTNING, DamageType::NECROTIC, DamageType::NORMAL, DamageType::POISON, DamageType::PSYCHIC, DamageType::HOLY, DamageType::STEEL, DamageType::DARK, DamageType::WATER };
	std::vector<uint16_t> doorOffColor = { 0x0006, 0x0007, 0x0008, 0x000E, 0x000F };

	int doorCount = boss ? 1 : 3;
	while (doorCount-- > 0)
	{
		Room room;

		// Select a new random attribute
		std::list<Rarity>::iterator rarityIter = doorRarity.begin();
		std::advance(rarityIter, GetRandomValue(0, (int)doorRarity.size() - 1));
		room.door.rarity = *rarityIter;
		doorRarity.erase(rarityIter); // Remove the dungeon theme from the list

		room.door.attribute = GetRandomValue(0, 1) == 1 ? ToAttribute(attribute) : ROLLTABLE(attributes);
		room.door.label = room.door.rarity == Rarity::LEGENDARY ? "Lgndry Door"
			: room.door.rarity == Rarity::EPIC ? "Epic Door"
			: room.door.rarity == Rarity::RARE ? "Rare Door"
			: "Common Door";
		room.door.opened = false;

		std::list<State>::iterator stateIter = nextState.begin();
		int randVal = GetRandomValue(0, (int)nextState.size() - 1);
		std::advance(stateIter, randVal);
		room.door.state = *stateIter;
		nextState.erase(stateIter); // Remove the state from the list

		room.door.open = room.door.rarity == Rarity::LEGENDARY ? m_db.m_animationDb["legendary_door_opened"]
			: room.door.rarity == Rarity::EPIC ? m_db.m_animationDb["epic_door_opened"]
			: room.door.rarity == Rarity::RARE ? m_db.m_animationDb["rare_door_opened"]
			: m_db.m_animationDb["common_door_opened"];
		room.door.open.SetAttributes(0, room.door.attribute);
		room.door.open.SetAttributes(1, ROLLTABLE(doorOffColor));
		if (room.door.rarity == Rarity::LEGENDARY)
		{
			room.door.open.SetAttributes(2, ROLLTABLE(attributes));
			room.door.open.SetAttributes(3, ROLLTABLE(doorOffColor));
		}
		room.door.close = room.door.rarity == Rarity::LEGENDARY ? m_db.m_animationDb["legendary_door_closed"]
			: room.door.rarity == Rarity::EPIC ? m_db.m_animationDb["epic_door_closed"]
			: room.door.rarity == Rarity::RARE ? m_db.m_animationDb["rare_door_closed"]
			: m_db.m_animationDb["common_door_closed"];
		room.door.close.SetAttributes(0, room.door.attribute);
		room.door.close.SetAttributes(1, ROLLTABLE(doorOffColor));
		if (room.door.rarity == Rarity::LEGENDARY)
		{
			room.door.close.SetAttributes(2, ROLLTABLE(attributes));
			room.door.close.SetAttributes(3, ROLLTABLE(doorOffColor));
		}

		room.trap = TrapType::INVALID;
		if (room.door.state == State::STATE_TRAP)
		{
			room.trap = GetRandomValue(0, 1) == 0 ? TrapType::TRAP_SPIKES : TrapType::TRAP_SWINGINGAXE;

			newFloor.rooms.push_back(room);
			continue;
		}

		if (room.door.state == State::STATE_SHOP)
		{
			// 50% change to include a single potion
			int allWeapons = GetRandomValue(0, 1);

			std::vector<std::vector<Weapon>> weaponTable = { m_db.m_commonTable };
			if (room.door.rarity >= Rarity::LEGENDARY) { weaponTable.push_back(m_db.m_legendaryTable); weaponTable.push_back(m_db.m_epicTable); weaponTable.push_back(m_db.m_rareTable); weaponTable.push_back(m_db.m_commonTable); }
			if (room.door.rarity >= Rarity::EPIC) { weaponTable.push_back(m_db.m_epicTable); weaponTable.push_back(m_db.m_rareTable); weaponTable.push_back(m_db.m_commonTable); }
			if (room.door.rarity >= Rarity::RARE) { weaponTable.push_back(m_db.m_rareTable); weaponTable.push_back(m_db.m_commonTable); }
			std::vector<uint16_t> hiltColor = { 0x0006, 0x0008, 0x000E, 0x000F };

			// Roll 3 to 4 weapons
			int count = allWeapons ? 4 : 3;
			while (count-- > 0)
			{
				std::vector<Weapon> table = ROLLTABLE(weaponTable);
				Weapon weapon = ROLLTABLE(table);
				weapon.idle.SetAttributes(1, ROLLTABLE(hiltColor));
				weapon.Randomize();

				if (weapon.name.empty())
				{
					count++;
					continue;
				}

				room.shop.push_back(weapon);
			}

			// Roll a potion potentially
			count = allWeapons ? 0 : 1;
			while (count-- > 0)
			{
				Weapon weapon = ROLLTABLE(m_db.m_potionTable);
				weapon.idle.SetAttributes(1, ROLLTABLE(hiltColor));
				weapon.Randomize();

				if (weapon.name.empty())
				{
					count++;
					continue;
				}

				room.shop.push_back(weapon);
			}

			newFloor.rooms.push_back(room);
			continue;
		}

		std::list<int> numMonsters = { 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4 };
		if (room.door.rarity >= Rarity::LEGENDARY) { numMonsters.push_back(4); numMonsters.push_back(4); numMonsters.push_back(4); numMonsters.push_back(4); }
		if (room.door.rarity >= Rarity::EPIC) { numMonsters.push_back(3); numMonsters.push_back(3); numMonsters.push_back(3); }
		if (room.door.rarity >= Rarity::RARE) { numMonsters.push_back(2); numMonsters.push_back(2); }

		// Select a new random attribute
		std::list<int>::iterator numIter = numMonsters.begin();
		std::advance(numIter, GetRandomValue(0, (int)numMonsters.size() - 1));
		int doorNumMonsters = *numIter;
		numMonsters.erase(numIter); // Remove the dungeon theme from the list

		std::list<Rarity> monsterRarity = { Rarity::COMMON, Rarity::COMMON, Rarity::COMMON, Rarity::RARE };
		if (rarity >= Rarity::LEGENDARY) { monsterRarity.push_back(Rarity::COMMON); monsterRarity.push_back(Rarity::RARE); monsterRarity.push_back(Rarity::EPIC); monsterRarity.push_back(Rarity::LEGENDARY); }
		if (rarity >= Rarity::EPIC) { monsterRarity.push_back(Rarity::COMMON); monsterRarity.push_back(Rarity::COMMON); monsterRarity.push_back(Rarity::RARE); monsterRarity.push_back(Rarity::RARE); monsterRarity.push_back(Rarity::EPIC); monsterRarity.push_back(Rarity::EPIC); monsterRarity.push_back(Rarity::LEGENDARY); }
		if (rarity >= Rarity::RARE) { monsterRarity.push_back(Rarity::COMMON); monsterRarity.push_back(Rarity::COMMON); monsterRarity.push_back(Rarity::RARE); monsterRarity.push_back(Rarity::EPIC); monsterRarity.push_back(Rarity::EPIC); }

		room.monsters.reserve(4);
		int count = boss ? 1 : doorNumMonsters;
		int recruitCount = family == MonsterFamily::UNDEAD ? 4 - count : 0;
		while (count-- > 0)
		{
			auto createMonster = [&](DamageType element, Rarity monRar, int buff, bool dead)->Monster {
				const int constant = (int)std::pow(2, (floor / 5) + 1 + buff);
				Weapon weapon;
				weapon.die = Die(int((floor / 5) + 1 + buff), 4, constant, element);
				weapon.speed = 8;
				weapon.target = Target::ENEMY;

				Weapon recruit;
				recruit.die = Die(0, 0, 0);
				recruit.speed = 4;
				recruit.target = Target::MONSTER_RECRUIT;
				recruit.altAnimation = true;

				Weapon condition;
				condition.die = Die(int((floor / 5) + 1 + buff), 2, constant, element);
				condition.speed = 2;
				condition.target = Target::MONSTER_CONDITION;
				condition.altAnimation = true;

				//Weapon split;
				//split.die = Die(0, 0, 0);
				//split.speed = 2;
				//split.target = Target::MONSTER_SPLIT;

				Weapon allEnemies;
				allEnemies.die = Die(int((floor / 5) + 1 + buff), 2, constant, element);
				allEnemies.speed = 2;
				allEnemies.target = Target::MONSTER_ALLENEMIES;
				allEnemies.altAnimation = true;

				Weapon drain;
				drain.die = Die(int((floor / 5) + 1 + buff), 2, constant, element);
				drain.speed = 2;
				drain.target = Target::MONSTER_DRAINMP;
				drain.altAnimation = true;

				//Weapon swap;
				//swap.die = Die(0, 0, 0);
				//swap.speed = 2;
				//swap.target = Target::MONSTER_SWITCH;

				Monster blob = m_db.m_monsterDb["Blob"];
				Monster bat = m_db.m_monsterDb["Bat"];
				Monster spider = m_db.m_monsterDb["Spider"];
				Monster skeleton = m_db.m_monsterDb["Skeleton"];
				Monster dragon = m_db.m_monsterDb["Dragon"];
				Monster nothing = m_db.m_monsterDb["Nothing"];

				Monster newMonster = family == MonsterFamily::GELATINOUS ? blob
					: family == MonsterFamily::RODENT ? bat
					: family == MonsterFamily::ARACHNID ? spider
					: family == MonsterFamily::UNDEAD ? skeleton
					: nothing;
				if (boss)
					newMonster = dragon;

				newMonster.name = element == DamageType::NORMAL ? newMonster.name : ToString(element) + " " + newMonster.name;
				newMonster.family = family;
				newMonster.rollMaxHp = Die((floor / 5) + 1 + buff, 6, constant);
				newMonster.rollMaxMp = Die((floor / 5) + buff, 6, (int)std::pow(2, floor / 5));
				newMonster.totalHp = newMonster.rollMaxHp.Roll();
				newMonster.totalMp = newMonster.rollMaxMp.Roll();
				newMonster.currentHp = dead ? 0 : newMonster.totalHp;
				newMonster.currentMp = newMonster.totalMp;
				newMonster.idle.SetAttributes(0, ToAttribute(element));
				newMonster.attack1.SetAttributes(0, ToAttribute(element));
				newMonster.attack2.SetAttributes(0, ToAttribute(element));
				newMonster.weakness = ToWeakness(element);
				newMonster.weapon1 = weapon;
				newMonster.weapon2 = weapon;
				if (boss)
				{
					newMonster.weapon3 = allEnemies;
					newMonster.weapon4 = allEnemies;
				}
				else if (family == MonsterFamily::GELATINOUS)
				{
					newMonster.weapon3 = condition;
					newMonster.weapon4 = condition;
				}
				else if (family == MonsterFamily::RODENT)
				{
					newMonster.weapon3 = drain;
					newMonster.weapon4 = drain;
				}
				else if (family == MonsterFamily::ARACHNID)
				{
					newMonster.weapon3 = condition;
					newMonster.weapon4 = condition;
				}
				else if (family == MonsterFamily::UNDEAD)
				{
					newMonster.weapon3 = recruit;
					newMonster.weapon4 = recruit;
				}
				newMonster.armor.armorClass = buff > 0 ? (floor / 5) + buff : (floor / 5);
				newMonster.armor.speed = 0;
				if (element != DamageType::NORMAL)
					newMonster.armor.resistances.push_back(element);
				newMonster.experience = constant + buff;
				newMonster.rarity = monRar;
				return newMonster;
			};

			// Select a new random attribute
			std::list<Rarity>::iterator monsterRarityIter = monsterRarity.begin();
			std::advance(monsterRarityIter, GetRandomValue(0, (int)monsterRarity.size() - 1));
			Rarity monRarity = *monsterRarityIter;
			monsterRarity.erase(monsterRarityIter); // Remove the monster rarity from the list

			if (recruitCount != 0 && recruitCount-- > 0)
			{
				room.monsters.push_back(createMonster(attribute, Rarity::COMMON, 0, true));
			}

			int select = GetRandomValue(0, 3);
			if (select == 0 && !boss)
			{
				DamageType selected;
				do { selected = damageTypes[GetRandomValue(0, (int)damageTypes.size() - 1)]; } while (selected != attribute);

				if (monRarity >= Rarity::RARE && GetRandomValue(0, 4) == 0)
					selected = ROLLTABLE(damageTypes);

				room.monsters.push_back(createMonster(selected, monRarity, ((int)monRarity) - 1, false));
			}
			else
			{
				DamageType selected = attribute;
				if (monRarity >= Rarity::RARE && GetRandomValue(0, 4) == 0)
					selected = ROLLTABLE(damageTypes);

				room.monsters.push_back(createMonster(selected, monRarity, ((int)monRarity) - 1, false));
			}
		}

		std::vector<Reward> rewards = { Reward::MONEY };
		if (room.door.rarity >= Rarity::LEGENDARY) { rewards.push_back(Reward::LEGENDARY_WEAPON); rewards.push_back(Reward::LEGENDARY_WEAPON); rewards.push_back(Reward::LEGENDARY_MONEY); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_MONEY); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }
		if (room.door.rarity >= Rarity::EPIC) { rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_MONEY); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }
		if (room.door.rarity >= Rarity::RARE) { rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }

		int mod = 1;
		if (room.door.rarity >= Rarity::LEGENDARY) mod = 16;
		if (room.door.rarity >= Rarity::EPIC) mod = 8;
		if (room.door.rarity >= Rarity::RARE) mod = 2;

		room.reward = rewards[GetRandomValue(0, (int)rewards.size() - 1)];
		room.rewardWeapon = room.reward == Reward::RARE_WEAPON ? ROLLTABLE(m_db.m_rareTable)
			: room.reward == Reward::EPIC_WEAPON ? ROLLTABLE(m_db.m_epicTable)
			: ROLLTABLE(m_db.m_legendaryTable);
		room.rewardWeapon.Randomize();
		room.rewardWeapon.gold = 0;
		room.roomAttribute = attribute;

		const int gold = ROLL((floor / 5) + ((2 + mod) * doorNumMonsters), 4, (floor / 5) + 1 + doorNumMonsters);
		const int rareGold = ROLL((floor / 5) + ((3 + mod) * doorNumMonsters), 4, (floor / 5) + 2 + doorNumMonsters);
		const int epicGold = ROLL((floor / 5) + ((5 + mod) * doorNumMonsters), 4, (floor / 5) + 3 + doorNumMonsters);
		const int legendaryGold = ROLL((floor / 5) + ((7 + mod) * doorNumMonsters), 4, (floor / 5) + 5 + doorNumMonsters);
		room.gold = room.reward == Reward::LEGENDARY_MONEY ? legendaryGold
			: room.reward == Reward::EPIC_MONEY ? epicGold
			: room.reward == Reward::RARE_MONEY ? rareGold
			: gold;

		room.boss = boss;

		newFloor.rooms.push_back(room);
	}

	for (auto&& room : newFloor.rooms)
		newFloor.doors.push_back(&room.door);

	return newFloor;
}
