///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reward.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Reward.h"

// --------------------------------------------------------------------------------------------------------------------
std::string ToString(Reward reward)
{
    switch (reward)
    {
        case Reward::MONEY:
            return "Money";
        case Reward::RARE_WEAPON:
            return "Rare Weapon";
        case Reward::RARE_MONEY:
            return "Rare Money";
        case Reward::EPIC_WEAPON:
            return "Epic Weapon";
        case Reward::EPIC_MONEY:
            return "Epic Money";
        case Reward::LEGENDARY_WEAPON:
            return "Legendary Weapon";
        case Reward::LEGENDARY_MONEY:
            return "Legendary Money";
    }
    return "";
}