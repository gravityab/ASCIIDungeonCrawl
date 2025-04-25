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

/// Structure for a weapon
struct Weapon
{
public:
	/// Constructor for a weapon
	Weapon();
	//Weapon(
	//	std::string name_,
	//	Die die_,
	//	Target target_,
	//	Rarity rarity_,
	//	Animation attack_,
	//	Animation idle_,
	//	Die cost_,
	//	int speed_,
	//	int mpCost_);

	Weapon(
		std::string name_,
		DamageType type_,
		DieRange dice_,
		Target target_,
		Rarity rarity_,
		Animation attack_,
		Animation idle_);

	/*Weapon(
		std::string name_,
		Target target_,
		Rarity rarity_,
		Animation idle_,
		Die cost,
		int armorClass,
		DamageType resistance);*/
	virtual ~Weapon();

	void Randomize();

	Weapon As(DamageType damage, Rarity rarity);
	static std::string ToString(std::string name_, DamageType damage_, Rarity rarity_);
	static std::string ToString(std::string name_, Rarity rarity_);
	static std::string ToString(std::string name_, DamageType damage_);

public:
	/// Name of the weapon
	std::string name;

	/// The dice of the weapon
	DieRange dice;

	/// The type of weapon
	DamageType type = DamageType::NORMAL;

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
};