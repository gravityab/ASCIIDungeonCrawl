///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Passives.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

// Dungeon Crawl Library Headers
#include "Animation.h"
#include "Rarity.h"

// Standard Template Library Headers
#include <string>
#include <stdint.h>

enum class PassiveType : int32_t
{
    INVALID = -1,
    GLOVES_BASH, // Gloves have change to stun opponent
    GLOVES_PICKPOCKET, // Gloves have chance to steal gold on hit when equipped with leather
    GLOVES_BOXER, // Gloves swing once per level if equipped with two gloves
    DAGGER_DUALWIELD, // Swing with both weapons so as long as they are both swords or daggers or any combination
    DAGGER_BLEED, // Daggers have chance to bleed
    DAGGER_MULTIATTACK, // Daggers swing twice
    SWORD_PARRY, // Swords have chance to parry incoming attacks
    SWORD_RIPOSTE, // Use equipped swords when attacked
    SWORD_SHIELD, // Swords deal twice as much damage with a shield
    SWORD_BATTLEWIZARD, // Equiping sword with staff deals additional damage
    GREATSWORD_HEAVYSWING, // Great sword speed decreased, great axes deal extra multi per level
    GREATSWORD_ALONE, // Having only a great sword deals 2 times multi damage
    GREATSWORD_HURT, // Every attack received this turn to hero equipping great swords doubles multi
    WAND_QUICKSPELL, // Wands and Staves gain -2 speed
    WAND_ELEMENTALMASTER, // Wands add 2x level to multi when eqipped with Robe
    WAND_FINESSE, // Wands have chance to attack twice
    STAFF_ELEMENTALMASTER, // Staffs deal twice as much multi but are slower
    STAFF_IGNOREELEMENT, // Staffs ignore elemental weaknesses
    STAFF_MAGICSHIELD, // Equipping robes with staff adds staff damage to mitigation as protective bubble
    PLATE_TAUNT, // Add higher chance for monsters to aim for plate wearers
    PLATE_HPBOOST, // Double the heroes hp per level when equipped with plate
    PLATE_IGNOREDAMAGE, // Change to ignore damage when equipping plate
    LEATHER_GOLD, // Add X gold per monster slain. X is rarity of leather equipped
    LEATHER_SCOUT, // See behind doors depending on how many heroes equip leather [1:Shop, 2:Traps, 4:Fountains and Monsters]
    LEATHER_DAMAGE, // Daggers and Swords deal X extra multi. X is Level times Rarity.
    ROBE_BACKLINE, // Robes make the wearer less threatening to monsters
    ROBE_GLASSCANNON, // Magical damage is doubled. Damage recieved is doubled.
    ROBE_MPCOST, // Magical cost is reduced by X. X is Rarity.
    COUNT,
};

/// A class representing a passive bonus to a dungeon crawl run
class Passive
{
public:
    /// Constructor for a passive bonus
    Passive();

    /// Constructor for a passive bonus with predefined settings
    Passive(const char* name, Rarity rarity, Animation animation, int x, int y, int tabIndex, const char* description);

public:
    /// The name of the passive bonus
    std::string name;

    /// The description of a passibe when hovered over
    std::string description;

    /// The rarity of the passive bonus
    Rarity rarity;

    /// The animation of the passive in game
    Animation animation;

    /// Whether or not the passive has been acquired during a run
    bool owned;

    /// Whether the passive has been recently acquired
    bool bNew;

    /// Grid x position
    int x;

    /// Grid y position
    int y;

    /// The tab index of the passive
    int tabIndex;
};