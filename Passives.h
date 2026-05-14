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
    GLOVES_BASH,            // [x] Glove hits have a chance to stun (first swing per turn)
    GLOVES_PICKPOCKET,      // [x] Glove + leather: chance to steal gold on hit
    GLOVES_BOXER,           // [x] Two gloves: extra swings per turn that scale with level
    DAGGER_DUALWIELD,       // [x] Dual-wield daggers, swords, or a mix to swing both weapons
    DAGGER_BLEED,           // [x] Dagger hits have a chance to apply bleed (first swing per turn)
    DAGGER_MULTIATTACK,     // [x] Each queued dagger swing happens twice
    SWORD_PARRY,            // [x] Equipped sword: chance to negate an incoming hit
    SWORD_RIPOSTE,          // [x] Equipped sword: chance to counter-attack with a sword roll
    //SWORD_SHIELD,         // [ ] Sword + shield: doubles damage
    SWORD_BATTLEWIZARD,     // [x] Sword + staff equipped: doubles damage of both
    GREATSWORD_HEAVYSWING,  // [x] Great swords deal +(Level * Rarity) extra multi
    GREATSWORD_ALONE,       // [x] Wielding only a great sword doubles its multi
    GREATSWORD_HURT,        // [x] Each hit taken this turn adds +1 multi to your next great sword swing
    WAND_QUICKSPELL,        // [x] Wand and Staff casts are 2 speed faster
    WAND_ELEMENTALMASTER,   // [x] Wand or Staff worn with Robe adds +(Level * Rarity) to multi
    WAND_FINESSE,           // [x] Each queued wand swing has a 25% chance to attack again
    STAFF_ELEMENTALMASTER,  // [x] Staffs deal double multi but are slower and cost extra MP
    STAFF_IGNOREELEMENT,    // [x] Staff attacks ignore enemy elemental resistance
    STAFF_MAGICSHIELD,      // [x] Robe + Staff: incoming damage reduced by a staff roll
    PLATE_TAUNT,            // [x] Monsters favor plate wearers in single-target picks
    PLATE_HPBOOST,          // [x] Plate wearer doubles HP gained on level up
    PLATE_IGNOREDAMAGE,     // [x] Plate wearer has a chance to fully ignore incoming damage
    LEATHER_GOLD,           // [x] Leather wearer earns (Leather Rarity * Monster Rarity) gold per kill
    LEATHER_SCOUT,          // [x] Leather wearers reveal door contents (1:Shop, 2:Trap, 4:Fountain/Monsters)
    LEATHER_DAMAGE,         // [x] Leather + dual daggers/swords/mix: +(Level * Rarity) multi
    ROBE_BACKLINE,          // [x] Robe wearers are skipped by single-target monster picks when others are alive
    ROBE_GLASSCANNON,       // [x] Robe + Wand/Staff: deal double damage and take double damage; spells cost more MP
    ROBE_MPCOST,            // [x] Spell cost reduced by Robe Rarity
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