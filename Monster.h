///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Monster.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Include Headers
#include "Actor.h"
#include "Animation.h"
#include "DamageTypes.h"
#include "Die.h"
#include "Weapon.h"

#include <vector>

/// Enumeration of a list of monster categories
enum class MonsterFamily : uint8_t
{
    INVALID,
    GELATINOUS, ///< Represents gelatinous family
    RODENT,     ///< Represents rodent family
    UNDEAD,     ///< Represents undead family
    ARACHNID,   ///< Represents arachnid family
    DRAGON,     ///< Represents dragon family
};

/// Boss-fight challenge modifiers. Applied to dragon bosses above Common rarity. The pool is
/// drawn from cyclically (one removal per draw, refilled when empty) so a player sees every
/// modifier before any repeats.
enum class BossModifier : uint8_t
{
    None,
    ResistMelee,        ///< +50% physical resistance (sword/dagger/glove/greatsword half-damage)
    ResistMagic,        ///< +50% spell resistance (wand/staff half-damage)
    FirstTurnImmune,    ///< Takes no damage on combat round 1
    DotAttacks,         ///< Hero hits gain a damage-over-time condition
    AlwaysSecondary,    ///< Always uses its multi-target attack (weapon2)
    StrongerEachTurn,   ///< Outgoing damage +25% per round survived
    ExtraHealth,        ///< 2x HP at spawn
    ElementImmune,      ///< Picks one element on spawn and takes 0 damage from it
    TauntImmune,        ///< PLATE_TAUNT weighting does not apply
    RetaliateMelee,     ///< Every melee hit on the dragon counter-attacks the hero for 1/10 dragon damage
};

/// A structure representing a monster
struct Monster : public Actor
{
public:
    /// Constructor for a monster
    Monster();

    /// Constructor for a monster with predefined settings
    Monster(std::string name, Die maxHp, Die maxMp, DamageType weakness, int experience, int level, Animation attack, Animation attack2, Animation idle, Animation spawn, Weapon weapon1, Weapon weapon2, Weapon weapon3, Weapon weapon4);

    /// Get the type of actor
    ActorType GetType() override;

public:
    /// The name of the monster
    std::string name;

    /// Whether or not this monster is considered elite
    Rarity rarity = Rarity::COMMON;

    /// The experience to give the player upon defeating
    int experience = 0;

    /// The monster attack animation
    Animation attack1;

    /// The monster secondary attack
    Animation attack2;

    /// The monster spawn animation
    Animation spawn;

    /// The random maximum hp
    Die rollMaxHp;

    /// The random maximum mp
    Die rollMaxMp;

    /// Monster family
    MonsterFamily family;

    /// The element type of the monster
    DamageType element;

    /// The weakness of the monster
    DamageType weakness;

    /// The strength of the monster
    DamageType strength;

    /// Monster is currently attacking
    bool attacking1 = false;

    /// Monster is currently using alt attack
    bool attacking2 = false;

    /// Time to display spawn
    Time spawning = Time::Zero;

    /// Whether the monster is stunned for the round
    bool stunned = false;

    /// Boss challenge modifiers (typically empty; populated for dragon bosses above Common).
    std::vector<BossModifier> modifiers;

    /// True while the monster has the FirstTurnImmune modifier and we haven't cleared round 1 yet.
    bool firstTurnImmune = false;

    /// Element this monster takes 0 damage from (set when ElementImmune modifier is applied).
    DamageType immuneElement = DamageType::INVALID;

    /// Rounds survived (incremented at start of each combat round). StrongerEachTurn uses this
    /// to scale outgoing damage by +25% per round.
    int roundsAlive = 0;
};

/// Convert rarity to string
std::string ToString(MonsterFamily family);

/// Short human-readable name for a boss modifier (used by the targeting hover dialog).
std::string ToString(BossModifier modifier);

/// Single-word adjective for a boss modifier, used to decorate a dragon boss's name.
/// For ElementImmune the adjective depends on which element the dragon is immune to.
std::string ToShortName(BossModifier modifier, DamageType immuneElement = DamageType::INVALID);