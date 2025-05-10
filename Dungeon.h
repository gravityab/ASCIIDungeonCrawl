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
#include "Traps.h"
#include "Weapon.h"

/// Standard Template Library Headers
#include <list>
#include <map>
#include <vector>

/// Structure representing a door in a dungeon
struct Door
{
    /// Whether the door is opened
    bool opened = false;

    /// The label of the door to display overhead
    std::string label;

    /// The label associated with what is behind the door
    std::string stateLabel;

    /// The amount of monsters behind the door
    uint32_t monsterCount = 0;

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

    /// The reward gold
    int gold = 0;

    /// Whether this room is a boss or greater threat
    bool boss = false;

    /// The back of room
    DamageType roomAttribute;

    /// The type of trap
    TrapType trap = TrapType::INVALID;
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

/*
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

    /// The size of the dungeon
    int Size();

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
*/

class DungeonEx
{
public:
    /// How many floors have the same element
    static int ElementCount;

    /// How many floors before next boss
    static int BossCount;

public:
    /// Construct all required tables for dungeon generation
    void Initialize();

    /// Reset the dungeon generator
    void Reset();

    /// Pop floor from front or generate new dungeon
    void GetNextFloor(Floor&& floor);

    /// Get the size of the attributes
    int GetSize();

    /// Get the attributes of the given floor
    DamageType GetAttributes(int index);

    /// Get an instance of the database
    Database& GetDatabase();

private:
    /// Get the difficulty value of the floor
    int GetDifficulty() const;

    /// Roll for floor rarity based on depth
    Rarity RollFloorRarity(int difficulty);

    /// Roll for floor attribute
    DamageType RollAttribute();

    /// Roll the next attribute
    DamageType RollNextAttribute();

    /// Roll the next monster family
    MonsterFamily RollMonsterFamily();

    /// Roll rarity based on overall rarity
    Rarity RollRarity(Rarity rarity);

    /// Roll door state
    State RollState();

    /// Roll a random monster
    Monster RollMonster(DamageType type, Rarity rarity, MonsterFamily family);

    /// Roll a random reward
    Reward RollReward(Rarity rarity);

    /// Roll a weapon table
    std::vector<Weapon> RollWeaponTable(Rarity rarity);

    /// Generate a floor
    Floor GenerateFloor(int floor, Rarity rarity, DamageType attribute, MonsterFamily family);

    /// Generate a shop room
    void GenerateShop(Room& room, Rarity rarity, DamageType damageType, MonsterFamily family);

    /// Generate a trap room
    void GenerateTrap(Room& room, Rarity rarity, DamageType damageType, MonsterFamily family);

    /// Generate a room within a floor
    Room GenerateRoom(Rarity rarity, DamageType attribute, MonsterFamily family);

    /// Generate a door for a room
    Door GenerateDoor(Rarity rarity, DamageType damageType, MonsterFamily family);

    /// Generate an encounter for the room
    void GenerateEncounter(Room& room, Rarity rarity, DamageType damageType, MonsterFamily family);

    /// Generate the reward weapon or gold amount
    void GenerateReward(Weapon& weapon, int& gold, Reward reward, const std::vector<Monster>& monsters, Rarity rarity);

    /// Reset the attributes list
    void ResetAttributes();

    /// Return whether the floor has a boss
    bool IsBossFloor() const;

    /// Get the elemental index
    int GetIndex() const;

    /// Whether the current floor should update current family and element
    int Remaining() const;

    /// Convert rarity to door name
    std::string ToDoorLabel(Rarity rarity);

    /// Convert rarity to open animation
    Animation ToOpenAnimation(Rarity rarity, uint16_t primary, uint16_t secondary, uint16_t tertiary);

    /// Convert rarity to close animation
    Animation ToCloseAnimation(Rarity rarity, uint16_t primary, uint16_t secondary, uint16_t tertiary);

    /// Get the base monster
    Monster ToMonster(Rarity rarity, DamageType type, MonsterFamily family, Die primaryDie, Die secondaryDie);

    /// Provide string if player can see behind door
    std::string ToStateLabel(State state, int numMonsters);

private:
    /// The database
    Database m_db;

    /// The database file
    bool m_dbInitialized = false;

    /// The current monster family to use
    MonsterFamily m_currentFamily = MonsterFamily::GELATINOUS;

    /// The current damage type
    DamageType m_currentType = DamageType::NORMAL;

    /// The current floor to generate
    int m_floor = 0;

    /// List of next attributes
    std::list<DamageType> m_nextAttributes;

    /// Complete list of attributes for history and future
    std::vector<DamageType> m_attributes;

    /// Database of monster attacks
    std::map<std::string, Weapon> m_monsterWeapons;
};