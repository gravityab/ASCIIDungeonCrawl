///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dungeon.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Dungeon.h"

/// Standard Template Library Headers
#include <list>

#define IMAGE(x)     m_db.m_imageDb[x]
#define FRAME(x)     m_db.m_frameDb[x]
#define ANIMATION(x) m_db.m_animationDb[x]
#define WEAPON(x)    m_db.m_weaponDb[x] 
#define MWEAPON(x)   m_monsterWeapons[x]
#define MONSTER(x)   m_db.m_monsterDb[x]

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

int Dungeon::Size()
{
    return m_dungeon.size();
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
            if (room.door.rarity >= Rarity::LEGENDARY) { weaponTable.push_back(m_db.m_legendaryTable); weaponTable.push_back(m_db.m_legendaryTable); weaponTable.push_back(m_db.m_legendaryTable); weaponTable.push_back(m_db.m_epicTable); }
            if (room.door.rarity >= Rarity::EPIC) { weaponTable.push_back(m_db.m_epicTable); weaponTable.push_back(m_db.m_epicTable); weaponTable.push_back(m_db.m_rareTable); weaponTable.push_back(m_db.m_commonTable); }
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
                std::vector<uint16_t> hiltColor = { 0x0006, 0x0008, 0x000E, 0x000F };
                uint16_t eyeColor = ROLLTABLE(hiltColor); //ROLLTABLE(attributes);
                newMonster.idle.SetAttributes(0, ToAttribute(element));
                newMonster.idle.SetAttributes(1, eyeColor);
                newMonster.attack1.SetAttributes(0, ToAttribute(element));
                newMonster.attack1.SetAttributes(1, eyeColor);
                newMonster.attack2.SetAttributes(0, ToAttribute(element));
                newMonster.attack2.SetAttributes(1, eyeColor);
                newMonster.spawn.SetAttributes(0, ToAttribute(element));
                newMonster.spawn.SetAttributes(1, eyeColor);
                newMonster.element = element;
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

// --------------------------------------------------------------------------------------------------------------------

// The number of floors with matching elements
int DungeonEx::ElementCount = 5;

// The number of floors between bosses
int DungeonEx::BossCount = 10;

// --------------------------------------------------------------------------------------------------------------------
void DungeonEx::Initialize()
{
    m_db.Initialize();

    ResetAttributes();

    m_monsterWeapons.clear();
    {
        m_monsterWeapons["attack_primary"] = Weapon(8, Target::ENEMY, false);
        m_monsterWeapons["attack_secondary"] = Weapon(8, Target::ENEMY, false);
        m_monsterWeapons["attack_all"] = Weapon(2, Target::MONSTER_ALLENEMIES, true);
        m_monsterWeapons["condition"] = Weapon(4, Target::MONSTER_CONDITION, true);
        m_monsterWeapons["recruit"] = Weapon(4, Target::MONSTER_RECRUIT, true);
        m_monsterWeapons["split"] = Weapon(2, Target::MONSTER_SPLIT, true);
        m_monsterWeapons["drain"] = Weapon(2, Target::MONSTER_DRAINMP, true);
    }
}

void DungeonEx::GetNextFloor(Floor&& floor)
{
    // If we ran out of floors, generate a new set
    if (m_dungeon.size() == 0)
        GenerateDungeon();

    floor = std::move(m_dungeon.front());
    m_dungeon.pop_front();
}

// --------------------------------------------------------------------------------------------------------------------
void DungeonEx::GenerateDungeon()
{
    // Roll floor characteristics
    int difficulty = GetDifficulty();
    Rarity rarity = RollFloorRarity(difficulty);
    DamageType attribute = RollAttribute();
    MonsterFamily family = RollMonsterFamily();

    Floor floor = GenerateFloor(m_dungeon.size(), rarity, attribute, family);
    m_dungeon.push_back(floor);
}

int DungeonEx::GetDifficulty() const
{
    return GetIndex() + 1;
}

Rarity DungeonEx::RollFloorRarity(int difficulty)
{
    if (difficulty < 1)
        difficulty = 1;
    if (difficulty > 4)
        difficulty = 4;

    // Create a weighted table of rarity
    std::vector<Rarity> table;
    int rarity = 1;
    do
    {
        for (int index = 1; index <= rarity; index++)
        {
            table.push_back(Rarity(index));
        }
        rarity++;
    } while (rarity <= difficulty);
    return ROLLTABLE(table);
}

DamageType DungeonEx::RollAttribute()
{
    // Queue up future attributes
    while (GetIndex() + 5 > (int)m_attributes.size())
    {
        m_attributes.push_back(RollNextAttribute());
    }

    // Assign the current type
    m_currentType = m_attributes[GetIndex()];
    return m_currentType;
}

DamageType DungeonEx::RollNextAttribute()
{
    // Lists are like cards within a deck. When the card is pulled, the rest of the odds are
    std::list<DamageType>::iterator iter = m_nextAttributes.begin();
    std::advance(iter, GetRandomValue(0, (int)m_nextAttributes.size() - 1));
    DamageType attribute = *iter;
    m_nextAttributes.erase(iter);

    // Reset the attributes list if we've depleted all of the elements
    if (m_nextAttributes.size() == 0)
        ResetAttributes();

    // Return the random attribute
    return attribute;
}

MonsterFamily DungeonEx::RollMonsterFamily()
{
    // Always return boss if boss floor
    if (IsBossFloor())
    {
        m_currentFamily = MonsterFamily::DRAGON;
    }

    // Only switch monster family every specific amount of levels
    if (Remaining() != 0)
    {
        return m_currentFamily;
    }

    // Select a new monster family
    std::vector<MonsterFamily> family =
    {
        MonsterFamily::ARACHNID,
        MonsterFamily::GELATINOUS,
        MonsterFamily::RODENT,
        MonsterFamily::UNDEAD
    };
    m_currentFamily = ROLLTABLE(family);
    return m_currentFamily;
}

Rarity DungeonEx::RollRarity(Rarity rarity)
{
    //             Room Rarity 
    //             1   2   3   4
    //         1  73% 61% 50% 41%
    //  Door   2  19% 20% 21% 25%
    //  Rarity 3  6%  14% 24% 18%
    //         4  2%  5%  5%  16%

    const double mean = 0.80 + (int)rarity * .20;
    const double standardDeviation = 0.75 + (int)rarity * .25;
    return (Rarity)GetNormalValue((int)Rarity::COMMON, (int)Rarity::LEGENDARY, mean, standardDeviation);
}

State DungeonEx::RollState()
{
    if (IsBossFloor())
        return State::STATE_COMBAT;

    double mean = 1.5;
    double standardDevition = 1.4;
    int value = GetNormalValue(1, 4, mean, standardDevition);
    switch (value)
    {
        case 1:
            return State::STATE_COMBAT; // 50%
        case 2:
            return State::STATE_SHOP; // 25%
        case 3:
            return State::STATE_TRAP; // 16%
        case 4:
            return State::STATE_FOUNTAIN; // 7%
        default:
            return State::STATE_COMBAT;
    }
}

Monster DungeonEx::RollMonster(DamageType type, Rarity rarity, MonsterFamily family)
{
    int buff = 1;
    bool dead = false;
    const int constant = (int)std::pow(2, (m_floor / 5) + 1 + buff);
    const Die primary =   Die(int((m_floor / 5) + 1 + buff), 4, constant, type);
    const Die secondary = Die(int((m_floor / 5) + 1 + buff), 2, constant, type);
    const Die health =    Die(int((m_floor / 5) + 1 + buff), 6, constant, type);

    Monster monster = ToMonster(rarity, type, family, primary, secondary);
    monster.rollMaxHp = health;
    monster.totalHp = monster.rollMaxHp.Roll();
    monster.currentHp = dead ? 0 : monster.totalHp;
    monster.armor.armorClass = buff > 0 ? (m_floor / 5) + buff : (m_floor / 5);
    monster.experience = constant + buff;
    return monster;
}

Reward DungeonEx::RollReward(Rarity rarity)
{
    std::vector<Reward> rewards = { Reward::MONEY };
    if (rarity >= Rarity::LEGENDARY) { rewards.push_back(Reward::LEGENDARY_WEAPON); rewards.push_back(Reward::LEGENDARY_WEAPON); rewards.push_back(Reward::LEGENDARY_MONEY); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_MONEY); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }
    if (rarity >= Rarity::EPIC) { rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_WEAPON); rewards.push_back(Reward::EPIC_MONEY); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }
    if (rarity >= Rarity::RARE) { rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_WEAPON); rewards.push_back(Reward::RARE_MONEY); }

    return ROLLTABLE(rewards);
}

std::vector<Weapon> DungeonEx::RollWeaponTable(Rarity rarity)
{
    std::vector<Weapon> weaponTable;

    Rarity tableRarity = RollRarity(rarity);
    switch (tableRarity)
    {
        case Rarity::COMMON:
            weaponTable = m_db.m_commonTable;
            break;
        case Rarity::RARE:
            weaponTable = m_db.m_rareTable;
            break;
        case Rarity::EPIC:
            weaponTable = m_db.m_epicTable;
            break;
        case Rarity::LEGENDARY:
            weaponTable = m_db.m_legendaryTable;
            break;
    }

    return weaponTable;
}

Floor DungeonEx::GenerateFloor(int numFloor, Rarity rarity, DamageType attribute, MonsterFamily family)
{
    m_floor = numFloor;

    Floor floor;
    floor.type = attribute;
    floor.family = family;
    floor.floorNumber = numFloor;
    floor.rarity = rarity;
    floor.shopTable = false;

    int numDoors = IsBossFloor() ? 1 : 3;
    while (numDoors-- > 0)
    {
        Room room = GenerateRoom(RollRarity(rarity), attribute, family);
        floor.doors.push_back(&room.door);
        floor.rooms.push_back(room);
    }

    return floor;
}

Room DungeonEx::GenerateRoom(Rarity rarity, DamageType damageType, MonsterFamily family)
{
    Room room;
    room.door = GenerateDoor(rarity, damageType, family);
    room.roomAttribute = damageType;
    room.trap = TrapType::INVALID;

    if (room.door.state == State::STATE_COMBAT)
    {
        GenerateEncounter(room, rarity, damageType, family);
    }
    if (room.door.state == State::STATE_SHOP)
    {
        GenerateShop(room, rarity, damageType, family);
    }
    if (room.door.state == State::STATE_TRAP)
    {
        GenerateTrap(room, rarity, damageType, family);
    }

    return room;
}

void DungeonEx::GenerateEncounter(Room& room, Rarity rarity, DamageType type, MonsterFamily family)
{
    room.boss = family == MonsterFamily::DRAGON;

    // Generate list of monsters
    int numMonsters = room.door.monsterCount;
    while (numMonsters-- > 0)
    {
        Monster monster = RollMonster(type, rarity, family);
        room.monsters.push_back(monster);
    }
    
    // Generate the reward from the encounter
    room.reward = RollReward(rarity);
    GenerateReward(room.rewardWeapon, room.gold, room.reward, room.monsters, rarity);
}

void DungeonEx::GenerateReward(Weapon& weapon, int& gold, Reward reward, const std::vector<Monster>& monsters, Rarity rarity)
{
    const int numMonsters = monsters.size();

    int mod = 1;
    if (rarity >= Rarity::LEGENDARY) mod = 16;
    if (rarity >= Rarity::EPIC) mod = 8;
    if (rarity >= Rarity::RARE) mod = 2;

    gold = 0;
    if (reward == Reward::RARE_WEAPON)
    {
        weapon = ROLLTABLE(m_db.m_rareTable);
        weapon.Randomize();
    }
    else if (reward == Reward::EPIC_WEAPON)
    {
        weapon = ROLLTABLE(m_db.m_epicTable);
        weapon.Randomize();
    }
    else if (reward == Reward::LEGENDARY_WEAPON)
    {
        weapon = ROLLTABLE(m_db.m_legendaryTable);
        weapon.Randomize();
    }
    else if (reward == Reward::MONEY)
    {
        gold = ROLL((m_floor / 5) + ((2 + mod) * numMonsters), 4, (m_floor / 5) + 2 + numMonsters);
    }
    else if (reward == Reward::RARE_MONEY)
    {
        gold = ROLL((m_floor / 5) + ((3 + mod) * numMonsters), 4, (m_floor / 5) + 3 + numMonsters);
    }
    else if (reward == Reward::EPIC_MONEY)
    {
        gold = ROLL((m_floor / 5) + ((5 + mod) * numMonsters), 4, (m_floor / 5) + 5 + numMonsters);
    }
    else if (reward == Reward::LEGENDARY_WEAPON)
    {
        gold = ROLL((m_floor / 5) + ((7 + mod) * numMonsters), 4, (m_floor / 5) + 7 + numMonsters);
    }
}

void DungeonEx::GenerateShop(Room& room, Rarity rarity, DamageType damageType, MonsterFamily family)
{
    std::vector<uint16_t> hiltColor = { 0x0006, 0x0008, 0x000E, 0x000F };

    int count = 4;
    while (count-- > 0)
    {
        std::vector<Weapon> table = RollWeaponTable(rarity);
        Weapon weapon = ROLLTABLE(table);
        weapon.idle.SetAttributes(1, ROLLTABLE(hiltColor));
        weapon.Randomize();
        room.shop.push_back(weapon);
    }
}

void DungeonEx::GenerateTrap(Room& room, Rarity rarity, DamageType damageType, MonsterFamily family)
{
    TrapType type = (TrapType)GetRandomValue(0, 1);
    room.trap = type;
}

Door DungeonEx::GenerateDoor(Rarity rarity, DamageType damageType, MonsterFamily family)
{
    std::vector<uint16_t> attributes = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F };
    std::vector<uint16_t> secondaryAttributes = { 0x0006, 0x0007, 0x0008, 0x000E, 0x000F };

    uint16_t primary = GetRandomValue(0, 1) == 1 ? ToAttribute(damageType) : ROLLTABLE(attributes);
    uint16_t secondary = ROLLTABLE(secondaryAttributes);
    uint16_t tertiary = ROLLTABLE(attributes);

    Door door;
    door.attribute = primary;
    door.label = ToDoorLabel(rarity);
    door.opened = false;
    door.open = ToOpenAnimation(rarity, primary, secondary, tertiary);
    door.close = ToCloseAnimation(rarity, primary, secondary, tertiary);
    door.rarity = rarity;
    door.state = RollState();
    door.monsterCount = GetNormalValue(1, 4, 1.5, 1.5);
    door.stateLabel = ToStateLabel(door.state, door.monsterCount);
    return door;
}

void DungeonEx::ResetAttributes()
{
    m_nextAttributes = {
        DamageType::NORMAL,
        DamageType::COLD,
        DamageType::FIRE,
        DamageType::LIGHTNING,
        DamageType::WATER,
        DamageType::NECROTIC,
        DamageType::PSYCHIC,
        DamageType::DARK,
        DamageType::POISON,
        DamageType::STEEL,
        DamageType::HOLY,
    };
}

bool DungeonEx::IsBossFloor() const
{
    if (m_floor == 0)
        return false;

    return (m_floor % BossCount == 0);
}

int DungeonEx::GetIndex() const
{
    return (m_floor / ElementCount);
}

int DungeonEx::Remaining() const
{
    return (m_floor % ElementCount);
}

std::string DungeonEx::ToDoorLabel(Rarity rarity)
{
    switch (rarity)
    {
        case Rarity::COMMON:    return "Common Door";
        case Rarity::RARE:      return "Rare Door";
        case Rarity::EPIC:      return "Epic Door";
        case Rarity::LEGENDARY: return "Lgndry Door";
    }
}

Animation DungeonEx::ToOpenAnimation(Rarity rarity, uint16_t primary, uint16_t secondary, uint16_t tertiary)
{
    Animation open;
    switch (rarity)
    {
        case Rarity::COMMON:
            open = ANIMATION("common_door_opened");
            break;
        case Rarity::RARE:
            open = ANIMATION("rare_door_opened");
            break;
        case Rarity::EPIC:
            open = ANIMATION("epic_door_opened");
            break;
        case Rarity::LEGENDARY:
            open = ANIMATION("legendary_door_opened");
            break;
        default:
            open = ANIMATION("common_door_opened");
    }

    open.SetAttributes(0, primary);
    open.SetAttributes(1, secondary);
    if (rarity == Rarity::LEGENDARY)
    {
        open.SetAttributes(2, primary);
        open.SetAttributes(3, secondary);
    }
    return open;
}

Animation DungeonEx::ToCloseAnimation(Rarity rarity, uint16_t primary, uint16_t secondary, uint16_t tertiary)
{
    Animation close;
    switch (rarity)
    {
        case Rarity::COMMON:
            close = ANIMATION("common_door_closed");
            break;
        case Rarity::RARE:
            close = ANIMATION("rare_door_closed");
            break;
        case Rarity::EPIC:
            close = ANIMATION("epic_door_closed");
            break;
        case Rarity::LEGENDARY:
            close = ANIMATION("legendary_door_closed");
            break;
        default:
            close = ANIMATION("common_door_closed");
    }

    close.SetAttributes(0, primary);
    close.SetAttributes(1, secondary);
    if (rarity == Rarity::LEGENDARY)
    {
        close.SetAttributes(2, primary);
        close.SetAttributes(3, secondary);
    }
    return close;
}

Monster DungeonEx::ToMonster(Rarity rarity, DamageType type, MonsterFamily family, Die primaryDie, Die secondaryDie)
{
    std::vector<uint16_t> eyeColors = { 0x0006, 0x0008, 0x000E, 0x000F };
    uint16_t eyeColor = ROLLTABLE(eyeColors);

    Monster monster;
    switch (family)
    {
        case MonsterFamily::ARACHNID:
            monster = MONSTER("Spider");
            monster.weapon3 = MWEAPON("condition");
            monster.weapon3.die = secondaryDie;
            monster.weapon4 = MWEAPON("condition");
            monster.weapon4.die = secondaryDie;
            break;
        case MonsterFamily::DRAGON:
            monster = MONSTER("Dragon");
            monster.weapon3 = MWEAPON("attack_all");
            monster.weapon3.die = secondaryDie;
            monster.weapon4 = MWEAPON("attack_all");
            monster.weapon4.die = secondaryDie;
            break;
        case MonsterFamily::GELATINOUS:
            monster = MONSTER("Blob");
            monster.weapon3 = MWEAPON("condition");
            monster.weapon3.die = secondaryDie;
            monster.weapon4 = MWEAPON("condition");
            monster.weapon4.die = secondaryDie;
            break;
        case MonsterFamily::RODENT:
            monster = MONSTER("Bat");
            monster.weapon3 = MWEAPON("drain");
            monster.weapon3.die = secondaryDie;
            monster.weapon4 = MWEAPON("drain");
            monster.weapon4.die = secondaryDie;
            break;
        case MonsterFamily::UNDEAD:
            monster = MONSTER("Skeleton");
            monster.weapon3 = MWEAPON("recruit");
            monster.weapon3.die = Die(0, 0, 0);
            monster.weapon4 = MWEAPON("recruit");
            monster.weapon4.die = Die(0, 0, 0);
            break;
    }

    monster.weapon1 = MWEAPON("attack_primary");
    monster.weapon1.die = primaryDie;
    monster.weapon2 = MWEAPON("attack_secondary");
    monster.weapon1.die = secondaryDie;

    monster.name = type == DamageType::NORMAL ? monster.name : ToString(type) + " " + monster.name;
    monster.family = family;
    monster.element = type;
    monster.weakness = ToWeakness(type);
    monster.armor.speed = 0;
    monster.armor.resistances.push_back(type);
    monster.rarity = rarity;
    monster.idle.SetAttributes(0, ToAttribute(type));
    monster.idle.SetAttributes(1, eyeColor);
    monster.attack1.SetAttributes(0, ToAttribute(type));
    monster.attack1.SetAttributes(1, eyeColor);
    monster.attack2.SetAttributes(0, ToAttribute(type));
    monster.attack2.SetAttributes(1, eyeColor);
    monster.spawn.SetAttributes(0, ToAttribute(type));
    monster.spawn.SetAttributes(1, eyeColor);

    return monster;
}

std::string DungeonEx::ToStateLabel(State state, int numMonsters)
{
    if (state == State::STATE_SHOP)
        return "Shop";
    else if (state == State::STATE_FOUNTAIN)
        return "Fountain";
    else if (state == State::STATE_TRAP)
        return "Trap";
    else if (state == State::STATE_COMBAT && numMonsters == 1)
        return "1 Monster";
    else if (state == State::STATE_COMBAT && numMonsters == 2)
        return "2 Monsters";
    else if (state == State::STATE_COMBAT && numMonsters == 3)
        return "3 Monsters";
    else if (state == State::STATE_COMBAT && numMonsters == 4)
        return "4 Monsters";
    return "";
}