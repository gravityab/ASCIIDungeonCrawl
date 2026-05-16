///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Weapon.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "Weapon.h"

// --------------------------------------------------------------------------------------------------------------------
Weapon::Weapon()
{}

Weapon::Weapon(
    std::string name_,
    DamageType type_,
    DieRange dice_,
    Target target_,
    Rarity rarity_,
    Animation attack_,
    Animation idle_,
    WeaponType weapon_)
{
    name = name_;
    type = type_;
    dice = dice_;
    target = target_;
    rarity = rarity_;
    attack = attack_;
    idle = idle_;
    weaponType = weapon_;
}

Weapon::~Weapon()
{}

void Weapon::Randomize()
{
    gold = dice.GetCost().Roll();
    armorClass = dice.GetAC().Roll();
    speed = dice.GetSpeed().Roll();
    mpCost = dice.GetMp().Roll();
    die = dice.GetDie();
    avgGold = dice.GetAvg();

    // Baseline 10% markup on all rolled prices. Reward weapons reset gold to 0 after
    // Randomize() so they're unaffected; only shop / starter-screen items keep this bump.
    // The HAGGLER passive applies a reciprocal 10% discount at shop entry time.
    if (gold > 0)
        gold = (gold * 11) / 10;

    // Scale wand / staff MP cost by rarity so higher-tier spell weapons demand a deeper mana
    // pool. A level 1 hero rarely affords a Legendary staff cast - they have to level up first.
    // The scaled cost is baked into mpCost itself, so anywhere that reads weapon->mpCost
    // (shop display, GetEffectiveMpCost, hover panels) shows the rarity-adjusted value.
    //   Common: x1.0   Rare: x1.3   Epic: x2.6   Legendary: x3.0
    if (mpCost > 0 && (weaponType == WeaponType::WAND || weaponType == WeaponType::STAFF))
    {
        double mult = 1.0;
        switch (rarity)
        {
            case Rarity::RARE:      mult = 1.3; break;
            case Rarity::EPIC:      mult = 2.6; break;
            case Rarity::LEGENDARY: mult = 3.0; break;
            case Rarity::ARTIFACT:  mult = 4.3; break;
            default:                mult = 1.0; break;
        }
        mpCost = int(double(mpCost) * mult + 0.5);
    }

    std::vector<DamageType> damageTypes = { DamageType::COLD, DamageType::FIRE, DamageType::LIGHTNING, DamageType::NECROTIC, DamageType::POISON, DamageType::PSYCHIC };
    if (rarity >= Rarity::LEGENDARY)
        resistances.push_back(ROLLTABLE(damageTypes));
}

Weapon Weapon::As(DamageType type_, Rarity rarity_)
{
    Weapon newWeapon = *this;

    if (type_ != DamageType::NORMAL)
    {
        char newName[128];
        sprintf_s(newName, 128, "%s %s", ::ToString(type_).c_str(), name.c_str());
        newWeapon.name = newName;
    }

    newWeapon.type = type_;
    newWeapon.dice.type = type_;
    newWeapon.rarity = rarity_;

    if (target == Target::PLAYERAC_SPELL
        || target == Target::PLAYERAC_SPEED
        || target == Target::PLAYERAC_SLOW)
        newWeapon.resistances.push_back(type_);

    newWeapon.attack.SetAttributes(0, ToAttribute(type_));
    newWeapon.idle.SetAttributes(2, ToAttribute(type_));
    if (rarity_ == Rarity::RARE)
    {
        newWeapon.idle.SetAttributes(3, ToAttribute(type_));
    }
    if (rarity_ == Rarity::EPIC)
    {
        newWeapon.attack.SetStrobe(0, true);
        newWeapon.idle.SetStrobe(3, true);
    }
    if (rarity_ == Rarity::LEGENDARY)
    {
        newWeapon.attack.SetStrobe(0, true);
        newWeapon.idle.SetStrobe(3, true);
        newWeapon.idle.SetStrobe(2, true);
    }
    if (rarity_ == Rarity::ARTIFACT)
    {
        newWeapon.attack.SetTrailing(0, true);
        newWeapon.idle.SetTrailing(3, true);
        newWeapon.idle.SetTrailing(2, true);
        newWeapon.idle.SetTrailing(1, true);
        newWeapon.idle.SetTrailing(0, true);
    }

    return newWeapon;
}

std::string Weapon::ToString(std::string name_, DamageType damage_, Rarity rarity_)
{
    std::string rarity = ::ToString(rarity_);
    std::string damage = ::ToString(damage_);
        
    char newName[128];
    if (damage_ != DamageType::NORMAL)
    {
        sprintf_s(newName, 128, "%s %s %s", rarity.c_str(), damage.c_str(), name_.c_str());
    }
    else
    {
        sprintf_s(newName, 128, "%s %s", rarity.c_str(), name_.c_str());
    }

    return newName;
}

std::string Weapon::ToString(std::string name_, Rarity rarity_)
{
    std::string rarity = ::ToString(rarity_);

    char newName[128];
    sprintf_s(newName, 128, "%s %s", rarity.c_str(), name_.c_str());
    return newName;
}

std::string Weapon::ToString(std::string name_, DamageType damage_)
{
    std::string damage = ::ToString(damage_);

    char newName[128];
    sprintf_s(newName, 128, "%s %s", damage.c_str(), name_.c_str());
    return newName;
}

