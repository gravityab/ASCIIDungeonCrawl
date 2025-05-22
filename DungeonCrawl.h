///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DungeonCrawl.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Action.h"
#include "Animation.h"
#include "Clock.h"
#include "CursorStack.h"
#include "DamageTypes.h"
#include "Database.h"
#include "Die.h"
#include "Dungeon.h"
#include "Frame.h"
#include "Hero.h"
#include "Image.h"
#include "Input.h"
#include "Rarity.h"
#include "State.h"
#include "Target.h"
#include "Weapon.h"

/// Class representing a dungeon crawl game
class DungeonCrawl
{
public:
    /// Constructor for a dungeon crawl game
    DungeonCrawl();

    /// Destructor
    ~DungeonCrawl();

    /// Initialize a dungeon crawler game
    bool Initialize(HANDLE handle, int frameLimit);

    /// Shutdown a dungeon crawler game
    void Shutdown();

    /// Run the game and block until shutdown
    bool RunLoop();

private:
    /// Draw the main screen
    void DrawMainScreen(Time delta);

    /// Draw the floor doors
    void DrawDoorsScreen(Time delta);

    /// Draw the shop screen
    void DrawShopScreen(Time delta);

    /// Draw the combat screen
    void DrawCombatScreen(Time delta);

    /// Draw the treasure screen
    void DrawTreasureScreen(Time delta);

    /// Draw the passive selection screen
    void DrawPassiveScreen(Time delta);

    /// Draw the fountain room
    void DrawFountain(Time delta);

    /// Draw the trap room
    void DrawTrapRoom(Time delta);

    /// Setup the next floor
    void NextFloor(Time delta);

    /// Add a new hero to the party
    void AddHero();

    /// Draw tiny side view of dungeon
    void DrawDungeon(Time delta);

    /// Draw the stairs background
    void DrawStairs(Time delta, uint16_t attribute);

    /// Draw the menu dialog
    void DrawMenu(Time delta);

    /// Draw the restart confirmation dialog
    void DrawRestart(Time delta);

    /// Draw a background
    void DrawBackground(Time delta, int index, uint16_t attribute);

    /// Draw the trap background
    void DrawTrap(Time delta, uint16_t attribute, bool showExit, bool triggered);

    /// Draw the doors
    void DrawDoors(Time delta);

    /// Draw the shop
    void DrawShop(Time delta);

    /// Draw the heroes
    void DrawHero(Time delta);

    /// Draw monsters
    void DrawMonsters(Time delta);

    /// Draw reward
    void DrawReward(Time delta);

    /// Draw item
    void DrawItem(Time delta, Weapon* weapon, int x, int y);

    /// Draw fairies
    void DrawFairies(Time delta);

    /// Draw action
    void DrawAction(Time delta);

    /// Draw the passives tab
    void DrawPassivesTab(int index, const std::string& label, bool hasNew);

    /// Process passive input
    void ProcessPassiveInput();

    /// Process state input
    void ProcessInput();

    /// Draw the passive options
    void DrawPassiveOptions(Time delta);

    /// Draw the passive screen
    void DrawPassives(Time delta);

    /// Draw the cursor
    void DrawCursor();

    /// Select next hero
    void NextHero();

    /// Select previous hero
    void PrevHero();

    /// Get unique passive that isn't in the current and owned list
    void GetUniquePassive();

    /// Give player xp
    void ReceiveXP(Hero& hero);

    /// Level a hero
    void LevelUp(Hero& hero);

    /// Check if the player owns a particular passive
    bool OwnsPassive(PassiveType type);

    /// Check if actor is equipping specific weapon types
    bool EquippingWeapon(Actor* actor, WeaponType type, int count);

    /// Calculate the final damage die with bonuses
    Die CalculateDamageDie(Actor* actor, Weapon* weapon, bool baseDamage);

    /// Use the weapon on the target
    void UseWeapon(Action action);

    /// Handle left and right movement
    void ClampCursor(int maxIndex);

    /// Transition to next state
    void SetState(State state);

    /// Update the tile list
    void UpdateTiles();

    /// Add monster actions to the action queue
    void PushMonsterActions();

    /// Create a monster action
    void CreateMonsterAction(Monster& monster);

    /// Perform a sort of the actions by speed
    void SortActions();

    /// Add random fairy
    void AddFairy();

    /// Apply condition damage or healing
    void ApplyCondition();

    /// Use a selected iton
    void UseSelectedItem();

    /// Finalize purchase the current weapon
    void PurchaseItem();

    /// Push the door cursor state
    void PushDoors();

    /// Push the shop cursor state
    void PushShop();

    /// Push the passive cursor state
    void PushPassive();

    /// Push the fountain cursor state
    void PushFountain();

    /// Push the spike trap cursor state
    void PushTrap();

    /// Push the spike trap initiated cursor state
    void PushTrapInitiated();

    /// Push the treasure cursor state
    void PushTreasure();

    /// Push the reward cursor state
    void PushReward();

    /// Push the hero selection for rewards cursor state
    void PushRewardHero();

    /// Push the item swap for rewards cursor state
    void PushRewardHeroItem();

    /// Push the combat cursor state
    void PushCombat(Actor* nextHero);

    /// Push the combat selection cursor state
    void PushCombatSelection();

    /// Push the hero selection cursor state
    void PushHero();

    /// Push the hero selection cursor state to use an item
    void PushHeroItem();

    /// Push the item use cursor state
    void PushUseItem();

    /// Push the hero selection shop cursor state
    void PushShopHero();

    /// Push the item purchase and replacement cursor state
    void PushShopItem();

    /// Push the menu dialog
    void PushMenu();

    /// Push the menu restart confirmation dialog
    void PushMenuRestart();

    /// Print levels of dungeon numbers
    void TestDungeon();

    /// Print the floor
    void PrintFloor(Floor floor);

    /// Print the room
    void PrintRoom(int index, Room room);

    /// Print monster
    void PrintMonster(int index, Monster monster);

    /// Print weapon
    void PrintWeapon(int index, Weapon weapon);

private:
    /// The game clock
    Clock m_clock;

    /// Timer for frame limiter
    Time m_frameTimeLimit;

    /// The console handle
    HANDLE m_handle = nullptr;

    /// The console entity representing a frame to render
    ConsoleEntity m_console;

    /// The current state of the dungeon crawl
    State m_state = State::STATE_MAIN;

    /// Wether the game should shutdown
    bool m_shutdown = false;

    /// The input manager
    Input m_input;

    /// The dungeon instance
    DungeonEx m_dungeonEx;

    /// The floor number
    int m_floor = 0;

    /// The initial room with a common shop
    Floor m_initFloor;

    /// The current floor the party is at
    Floor* m_currentFloorPtr = nullptr;
    Floor m_currentFloor;

    /// The currently selected room
    Room* m_currentRoom = nullptr;

    /// The parties current gold
    int m_gold;

    /// The database instance
    Database& m_db;

    /// The current party
    std::vector<Hero> m_heroes;

    /// The blink timing
    Time m_blinkTime;

    /// Whether the blink text should strobe
    bool m_blink = false;

    /// The current timer to execute the next state
    Time m_timeLeft = Time::Zero;

    /// The state of the treasure chest
    bool m_chestClosed = false;

    /// The chest animation
    Animation m_chest;

    /// The attribute of the chest
    uint16_t m_chestAttribute;

    /// The cursor state stack
    CursorStack m_ui;

    /// List of combat actions
    std::vector<Action> m_actions;

    /// The fountain animation
    Animation m_fountain;

    /// The stairs animation
    Animation m_stairs;

    /// Amount of damage to display
    int m_damageOriginal;

    /// Amount of damage resisted
    int m_damageFinal;

    /// Damage display attribute
    uint16_t m_damageAttribute;

    /// The damage element attribute
    uint16_t m_damageAttribute2;

    /// Amount of time to display damage
    Time m_damageTimeLeft = Time::Zero;

    /// Whether the trap was initiated
    int m_trapInitiated = false;

    /// Whether the trap is triggered
    bool m_trapTriggered = false;

    /// The trap DC
    int m_trapDC = 0;

    /// The trap roll
    int m_trapRoll = 0;

    /// Track when to display attributes
    bool m_showAttributes = true;

    /// Show passive screen
    bool m_showPassives = false;

    /// Show the menu dialog
    bool m_showMenu = false;

    /// Show the restart confirmation dialog
    bool m_showRestartDialog = false;

    /// The current hero index
    int m_heroIndex = 0;

    /// Condition on player 1
    Weapon m_condition1;

    /// Condition on player 2
    Weapon m_condition2;

    /// Condition on player 3
    Weapon m_condition3;

    /// Condition on player 4
    Weapon m_condition4;

    /// The passives screen cursor
    Animation m_passivesCursor;

    /// The passives screen cursor x
    int m_passivesX = 0;

    /// The passives screen cursor y
    int m_passivesY = 0;

    /// Whether the 
    bool m_passivesTab = false;

    /// The passives screen cursor tab
    int m_passivesTabIndex = 0;

    /// List of owned passives
    std::vector<Passive> m_passives;

    /// List of passive options after combat
    std::vector<Passive> m_passiveOptions;

    /// The tiles for the dungeon map
    std::vector<Animation> m_tiles;

    /// List of fairies
    struct Fairy { Animation fairy; Animation dust; int dir; int x; int y; int x_dust; int y_dust; Time timeLeft; };
    std::vector<Fairy> m_fairies;
};