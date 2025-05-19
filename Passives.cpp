///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Passives.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

// Dungeon Crawl Library Headers
#include "Passives.h"

// --------------------------------------------------------------------------------------------------------------------

Passive::Passive()
{}

Passive::Passive(const char* name_, Rarity rarity_, Animation animation_, int x_, int y_, int tabIndex_, const char* description_)
{
    name = name_;
    rarity = rarity_;
    animation = animation_;
    x = x_;
    y = y_;
    tabIndex = tabIndex_;
    description = description_;
    owned = false;
    bNew = true;
}
