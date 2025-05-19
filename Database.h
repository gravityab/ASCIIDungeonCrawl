///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Database.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Animation.h"
#include "Image.h"
#include "Frame.h"
#include "Weapon.h"
#include "Monster.h"
#include "Passives.h"

/// Standard Template Library Headers
#include <map>

/// Class representing a series of databases for a dungeon crawl game
class Database
{
public:
    /// Initialize the database table
    void Initialize();

    /// Reset the database
    void Reset();

public:
    /// The image database
    std::map<std::string, Image> m_imageDb;

    /// The frame database
    std::map<std::string, Frame> m_frameDb;

    /// The animation database
    std::map<std::string, Animation> m_animationDb;

    /// The die range database
    std::map<std::string, DieRange> m_dieRangeDb;

    /// The weapon database
    std::map<std::string, Weapon> m_weaponDb;

    /// The monster table
    std::map<std::string, Monster> m_monsterDb;

    /// The passive table
    std::map<PassiveType, Passive> m_passiveDb;

    /// The common weapons table
    std::vector<Weapon> m_commonTable;

    /// The rare weapons table
    std::vector<Weapon> m_rareTable;

    /// The epic weapons table
    std::vector<Weapon> m_epicTable;

    /// The legendary table
    std::vector<Weapon> m_legendaryTable;

    /// The potion table
    std::vector<Weapon> m_potionTable;
};