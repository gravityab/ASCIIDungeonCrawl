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

// The number of floors with matching elements
int DungeonEx::ElementCount = 5;

// The number of floors between bosses
int DungeonEx::BossCount = 10;

// --------------------------------------------------------------------------------------------------------------------
void DungeonEx::Initialize()
{
    Reset();
}

void DungeonEx::Reset()
{
    m_floor = 0;
    m_attributes.clear();
    ResetAttributes();
    RollAttribute();

    if (!m_dbInitialized)
    {
        m_db.Initialize();
        m_dbInitialized = true;
    }
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
    // Roll floor characteristics
    int difficulty = GetDifficulty();
    Rarity rarity = RollFloorRarity(difficulty);
    DamageType attribute = RollAttribute();
    MonsterFamily family = RollMonsterFamily();

    floor = std::move(GenerateFloor(m_floor, rarity, attribute, family));
    m_floor++;
}

int DungeonEx::GetSize()
{
    return m_attributes.size();
}

DamageType DungeonEx::GetAttributes(int index)
{
    return m_attributes[index];
}

Database& DungeonEx::GetDatabase()
{
    return m_db;
}

// --------------------------------------------------------------------------------------------------------------------
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
    if (m_attributes.size() == 0)
    {
        // Normal is always at the front
        m_attributes.push_back(DamageType::NORMAL);
        m_nextAttributes.pop_front();
    }

    // Queue up future attributes
    while (GetIndex() + 6 > (int)m_attributes.size())
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
        return m_currentFamily;
    }

    // Only switch monster family every specific amount of levels
    if (m_currentFamily != MonsterFamily::DRAGON)
    {
        if (Remaining() != 0)
        {
            return m_currentFamily;
        }
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
    double standardDevition = 1.3;
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
    const double part1 = 5 + (double(rarity) * 1);
    const double part2 = std::pow(double(double(int(m_floor) / 5)), double(2));
    const double part3 = double(rarity);
    const double result = (part1 * part2) + part3;
    const int expectedDamage = int(result);
    const bool dead = false;

    int constant  = expectedDamage / 2;
    if (constant == 0) constant = 1;
    const int die       = GetDie(family);
    const int mult      = constant / double(die / 2);
    const Die primary   = Die(mult / 1, die, constant);
    const Die secondary = Die(mult / 2, die, constant);
    const Die health    = Die(mult / 1, die, constant);
    const int armor     = (m_floor / 5) + (int(rarity) - 1);

    Monster monster = ToMonster(rarity, type, family, primary, secondary);
    monster.rollMaxHp = health;
    monster.totalHp = monster.rollMaxHp.Roll();
    monster.currentHp = dead ? 0 : monster.totalHp;
    monster.armor.armorClass = armor;
    monster.experience = constant;
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
    room.reward = Reward::INVALID;

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
    if (IsBossFloor())
        room.boss = true;

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
    if (IsBossFloor()) door.monsterCount = 1;
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

int DungeonEx::GetDie(MonsterFamily family) const
{
    switch (family)
    {
        case MonsterFamily::ARACHNID:
            return 6;
        case MonsterFamily::DRAGON:
            return 10;
        case MonsterFamily::GELATINOUS:
            return 4;
        case MonsterFamily::RODENT:
            return 4;
        case MonsterFamily::UNDEAD:
            return 8;
    }

    return 1;
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

    return std::string();
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