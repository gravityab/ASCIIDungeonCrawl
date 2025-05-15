///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CursorStack.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "CursorStack.h"

// --------------------------------------------------------------------------------------------------------------------
CursorStack::CursorStack()
{}

CursorContext& CursorStack::GetContext()
{
    return m_contexts[m_contexts.size() - 1];
}

CursorState CursorStack::GetState()
{
    return GetContext().state;
}

Animation& CursorStack::GetAnimation()
{
    return GetContext().cursor;
}

CursorIndexDirection CursorStack::GetDirection()
{
    return GetContext().direction;
}

int CursorStack::GetSize() const
{
    return m_contexts.size();
}

int CursorStack::GetCursorIndex()
{
    return GetContext().index;
}

void CursorStack::IncrementCursorIndex()
{
    CursorContext& context = GetContext();
    ClampCursorIndex(1); // Add
}

void CursorStack::DecrementCursorIndex()
{
    CursorContext& context = GetContext();
    ClampCursorIndex(-1); // Subtract
}

void CursorStack::PushBack(CursorContext context)
{
    std::vector<bool> validIndex;
    validIndex.resize(context.maxIndex + 1);

    // Create a full list of selectable indexes
    for (int index = context.minIndex; index < context.maxIndex + 1; index++)
        validIndex[index] = true;

    // Update the full list to incldude excludes
    for (int index = 0; index < (int)context.excludeIndexes.size(); index++)
        validIndex[context.excludeIndexes[index]] = false;

    // If the initial index is fine, use it
    if (!validIndex[context.index])
    {
        // Check if there are no options to select from
        bool noOptions = true;
        for (int index = context.minIndex; index < context.maxIndex + 1; index++)
        {
            if (validIndex[index])
            {
                noOptions = false;
                context.index = index; // First available
                break;
            }
        }
        assert(noOptions != true && "All options excluded");
    }

    m_contexts.push_back(context);
}

void CursorStack::PopBack(int limit)
{
    if (m_contexts.size() > size_t(limit))
        m_contexts.pop_back();
}

void CursorStack::PopBackTo(int index)
{
    while (m_contexts.size() > size_t(index))
        m_contexts.pop_back();
}

void CursorStack::Clear()
{
    m_contexts.clear();
}

void CursorStack::ClampCursorIndex(int direction)
{
    CursorContext& context = GetContext();
    context.index += direction;

    // Clamp the cursor position
    if (context.index > context.maxIndex)
        context.index = context.maxIndex;
    if (context.index < context.minIndex)
        context.index = context.minIndex;

    std::vector<bool> validIndex;
    validIndex.resize(context.maxIndex + 1);

    // Create a full list of selectable indexes
    for (int index = context.minIndex; index < context.maxIndex + 1; index++)
        validIndex[index] = true;

    // Update the full list to incldude excludes
    for (int index = 0; index < (int)context.excludeIndexes.size(); index++)
        validIndex[context.excludeIndexes[index]] = false;

    // Check if there are no options to select from
    bool noOptions = true;
    for (int index = context.minIndex; index < context.maxIndex + 1; index++)
    {
        if (validIndex[index])
            noOptions = false;
    }
    assert(noOptions != true && "All options excluded");

    // Keep moving a direction until we can select a valid index.
    // If there are no valid options left, move the opposite direction until we find one
    bool turn = false;
    while (!validIndex[context.index])
    {
        if (!turn && context.index != (direction == -1 ? context.minIndex : context.maxIndex))
            context.index += direction;
        else
            turn = true;
        if (turn)
            context.index -= direction;
    }
}

// --------------------------------------------------------------------------------------------------------------------

std::string ToString(CursorState state)
{
    switch (state)
    {
        case CursorState::DOOR: return "Select Door";
        case CursorState::HERO: return "Hero";
        case CursorState::HERO_ITEM: return "Select Item";
        case CursorState::ATTRIBUTES: return "Attributes";
        case CursorState::INVENTORY_1: return "Select Item";
        case CursorState::INVENTORY_2: return "Select Item";
        case CursorState::USE_ITEM: return "Select Target";
        case CursorState::SHOP: return "Shop";
        case CursorState::SHOP_HERO: return "Purchase For";
        case CursorState::SHOP_HERO_ITEM: return "Replace Slot";
        case CursorState::COMBAT: return "Select Action";
        case CursorState::COMBAT_MONSTERS: return "Attack";
        case CursorState::COMBAT_HERO: return "Select Hero";
        case CursorState::CHEST: return "Chest";
        case CursorState::REWARD: return "Reward";
        case CursorState::REWARD_HERO: return "Give Hero";
        case CursorState::REWARD_HERO_ITEM: return "Replace Slot";
        case CursorState::FOUNTAIN: return "Fountain";
        case CursorState::TRAP: return "Trap";
        case CursorState::TRAP_INITIATED: return "Leave";
        case CursorState::ITEM_USED: return "Item Used";
        default:
            return "Unknown";
    }
}