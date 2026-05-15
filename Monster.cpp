///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Monster.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Monster.h"

// --------------------------------------------------------------------------------------------------------------------
Monster::Monster()
{}

Monster::Monster(
    std::string name_,
    Die maxHp_,
    Die maxMp_,
    DamageType weakness_,
    int experience_,
    int level_,
    Animation attack_,
    Animation attack2_,
    Animation idle_,
    Animation spawn_,
    Weapon weapon1_,
    Weapon weapon2_,
    Weapon weapon3_,
    Weapon weapon4_)
{
    name = name_;
    rollMaxHp = maxHp_;
    rollMaxMp = maxMp_;
    weakness = weakness_;
    experience = experience_;
    level = level_;
    attack1 = attack_;
    attack2 = attack2_;
    idle = idle_;
    spawn = spawn_;
    weapon1 = weapon1_;
    weapon2 = weapon2_;
    weapon3 = weapon3_;
    weapon4 = weapon4_;
    spawning = ToMilliseconds(GetRandomValue(1000, 1800));
}

ActorType Monster::GetType()
{
    return ActorType::ACTOR_MONSTER;
}

// --------------------------------------------------------------------------------------------------------------------
std::string ToString(MonsterFamily family)
{
    switch (family)
    {
        case MonsterFamily::ARACHNID:
            return "Arachnid";
        case MonsterFamily::DRAGON:
            return "Dragon";
        case MonsterFamily::GELATINOUS:
            return "Gelatinous";
        case MonsterFamily::RODENT:
            return "Rodent";
        case MonsterFamily::UNDEAD:
            return "Undead";
    }
    return "";
}

std::string ToString(BossModifier modifier)
{
    switch (modifier)
    {
        case BossModifier::ResistMelee:      return "Resists Melee  (sword/dagger/glove/great)";
        case BossModifier::ResistMagic:      return "Resists Magic  (wand/staff)";
        case BossModifier::FirstTurnImmune:  return "Invulnerable on the First Round";
        case BossModifier::DotAttacks:       return "Hits Apply a Damage-Over-Time";
        case BossModifier::AlwaysSecondary:  return "Always Uses Multi-Target Attack";
        case BossModifier::StrongerEachTurn: return "Damage Grows +25% Each Round";
        case BossModifier::ExtraHealth:      return "Extra Health (2x HP)";
        case BossModifier::ElementImmune:    return "Immune to One Element";
        case BossModifier::TauntImmune:      return "Ignores Plate Taunt";
        case BossModifier::RetaliateMelee:   return "Melee Hits Counter for 1/10 Dragon Damage";
        default:                             return "";
    }
}

static std::string ImmuneElementAdjective(DamageType type)
{
    switch (type)
    {
        case DamageType::NORMAL:    return "Friendly Neighborhood";
        case DamageType::COLD:      return "Frosty";
        case DamageType::FIRE:      return "Fiery";
        case DamageType::LIGHTNING: return "Charged";
        case DamageType::WATER:     return "Soaked";
        case DamageType::NECROTIC:  return "Putrid";
        case DamageType::POISON:    return "Virulent";
        case DamageType::PSYCHIC:   return "Maddening";
        case DamageType::DARK:      return "Shadowy";
        case DamageType::HOLY:      return "Anointed";
        case DamageType::STEEL:     return "Steely";
        default:                    return "Warded";
    }
}

std::string ToShortName(BossModifier modifier, DamageType immuneElement)
{
    switch (modifier)
    {
        case BossModifier::ResistMelee:      return "Armored";
        case BossModifier::ResistMagic:      return "Warded";
        case BossModifier::FirstTurnImmune:  return "Phasing";
        case BossModifier::DotAttacks:       return "Lingering";
        case BossModifier::AlwaysSecondary:  return "Sweeping";
        case BossModifier::StrongerEachTurn: return "Rising";
        case BossModifier::ExtraHealth:      return "Sturdy";
        case BossModifier::ElementImmune:    return ImmuneElementAdjective(immuneElement);
        case BossModifier::TauntImmune:      return "Elusive";
        case BossModifier::RetaliateMelee:   return "Spiked";
        default:                             return "";
    }
}