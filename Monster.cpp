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
