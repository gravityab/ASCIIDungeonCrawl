///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Weapon.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Animation.h"
#include "Die.h"
#include "Target.h"
#include "Rarity.h"

/// Standard Template Library Headers
#include <string>

enum class WeaponType : int32_t
{
    INVALID = -1,
    GLOVES,
    DAGGER,
    SWORD,
    GREATSWORD,
    WAND,
    STAFF,

    ROBE,
    LEATHER,
    PLATE,

    POTION,
    NEWHERO,
    LEVELUP,
    LEVELUP5,
};

/// Structure for a weapon
struct Weapon
{
public:
    /// Constructor for a weapon
    Weapon();

    /// Constructor for a weapon with predefined properties
    Weapon(
        std::string name_,
        DamageType type_,
        DieRange dice_,
        Target target_,
        Rarity rarity_,
        Animation attack_,
        Animation idle_,
        WeaponType weapon_);

    /// Constructor for monsters predefined properties
    Weapon(
        int speed_,
        Target target_,
        bool altAnimation_)
    {
        speed = speed_;
        target = target_;
        altAnimation = altAnimation_;
    }

    /// Destructor for a weapon
    virtual ~Weapon();

    /// Generate the weapons potential dies for specific statistics
    void Randomize();

    /// Convert the weapon to a different rarity and damage type
    Weapon As(DamageType damage, Rarity rarity);

    /// Generate a string name for the item based on specific categories
    static std::string ToString(std::string name_, DamageType damage_, Rarity rarity_);
    static std::string ToString(std::string name_, Rarity rarity_);
    static std::string ToString(std::string name_, DamageType damage_);

public:
    /// Name of the weapon
    std::string name;

    /// The dice of the weapon
    DieRange dice;

    /// The elemental type of weapon
    DamageType type = DamageType::NORMAL;

    /// The type pf weapon
    WeaponType weaponType = WeaponType::INVALID;

    /// The die associated with the result of the weapon use
    Die die;

    /// The target of the weapon
    Target target = Target::INVALID;

    /// The rarity of the weapon
    Rarity rarity = Rarity::COMMON;

    /// The armor class of the armor
    int armorClass = 0;

    /// The list of damage resistances the armor provides
    std::vector<DamageType> resistances;

    /// The attack animation of using the weapon
    Animation attack;

    /// The idle animation of displaying the weapon in a store
    Animation idle;

    /// The speed of the weapon in combat
    int speed = 0;

    /// The mp cost of using the weapon
    int mpCost = 0;

    /// The gold value of weapon to purchase
    int gold = 0;

    /// Average gold cost
    int avgGold = 0;

    /// Whether the item was purchased from the store
    bool purchased = false;

    /// Whether the item was selected to be used on a target
    bool selected = false;

    /// Whether the monster animation of this item uses the alternate animation
    bool altAnimation = false;
};