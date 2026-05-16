///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DungeonCrawl.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "DungeonCrawl.h"

/// Dungeon Crawl Library Headers
#include "Sleep.h"

/// Standard Template Library Headers
#include <algorithm>

// --------------------------------------------------------------------------------------------------------------------
Time s_blinkTime = ToMilliseconds(600);

#define IMAGE(x)     m_db.m_imageDb[x]
#define FRAME(x)     m_db.m_frameDb[x]
#define ANIMATION(x) m_db.m_animationDb[x]
#define WEAPON(x)    m_db.m_weaponDb[x] 
#define MONSTER(x)   m_db.m_monsterDb[x]
#define PASSIVE(x)   m_db.m_passiveDb[x]
#define BLINK(x)     m_blink ? x : x | 0xC000
#define SOLID(x)     x | 0xC000

// --------------------------------------------------------------------------------------------------------------------
DungeonCrawl::DungeonCrawl()
    : m_db(m_dungeonEx.GetDatabase())
{}

DungeonCrawl::~DungeonCrawl()
{}

bool DungeonCrawl::Initialize(int frameLimit)
{
    if (frameLimit > 0)
        m_frameTimeLimit = ToSeconds(1.f / static_cast<float>(frameLimit));
    else
        m_frameTimeLimit = Time::Zero;

    // Load persisted high scores from %LOCALAPPDATA%\ASCIIDungeonCrawl\highscores.txt.
    m_highScores.Load();

    SetState(State::STATE_MAIN);

    return true;
}

void DungeonCrawl::Shutdown()
{}

bool DungeonCrawl::RunLoop()
{
    // Start the game clock
    m_clock.Start();

    while (!m_shutdown)
    {
        if (m_frameTimeLimit != Time::Zero)
            SleepTime(m_frameTimeLimit - m_clock.GetElapsedTime());
        Time delta = m_clock.Restart();

        m_blinkTime += delta;
        if (m_blinkTime > ToMilliseconds(200))
            m_blink = true;
        if (m_blinkTime > ToMilliseconds(600))
        {
            m_blinkTime = ToMicroseconds(m_blinkTime.AsMicroseconds() % ToMilliseconds(600).AsMicroseconds());
            m_blink = false;
        }

        m_input.Poll();
        if (m_input.GetWindowClosed())
            m_shutdown = true;
        m_console.SetData(' ', 0, 0, 120, 30, 0x0007);

        switch (m_state)
        {
            case State::STATE_MAIN:
                DrawMainScreen(delta);
                break; 
            case State::STATE_NEXT_FLOOR:
                NextFloor(delta);
                break;
            case State::STATE_DOORS:
                DrawDoorsScreen(delta);
                break;
            case State::STATE_SHOP:
                DrawShopScreen(delta);
                break;
            case State::STATE_COMBAT:
            case State::STATE_COMBAT_RESOLVE:
                DrawCombatScreen(delta);
                break;
            case State::STATE_TREASURE:
                DrawTreasureScreen(delta);
                break;
            case State::STATE_PASSIVE:
                DrawPassiveScreen(delta);
                break;
            case State::STATE_FOUNTAIN:
                DrawFountain(delta);
                break;
            case State::STATE_TRAP:
                DrawTrapRoom(delta);
                break;
            case State::STATE_HIGHSCORE:
                DrawHighScoreEntry(delta);
                break;
        }

        // Draw the dungeon tiles
        DrawDungeon(delta);

        // Draw the passive screen
        DrawPassives(delta);

        // Draw the menu screen
        DrawMenu(delta);

        // Draw the restart confirmation dialog
        DrawRestart(delta);

        // Draw the high-score list overlay (gated on cursor state HIGHSCORE_LIST).
        DrawHighScoreList(delta);

        // Draw the boss-dragon modifier hover dialog (gated on COMBAT_MONSTERS over a modded boss).
        DrawDragonModifierDialog(delta);

        m_console.Draw(nullptr);
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void DungeonCrawl::DrawMainScreen(Time delta)
{
    std::string title =
        ":::::::-:...:::....:::::::....::::..:,-:::::/..:,::::::......:::.....::::....::::" \
        ".;;,...`';,.;;.....;;;`;;;;,..`;;;,;;-'````'...;;;;''''...:;;;;;;;:..`;;;;,..`;;;" \
        ".`[[.....[[[['.....[[[..[[[[[:.'[[[[[...[[[[[[/.[[cccc...,[[.....\\[[,..[[[[[:.'[[" \
        "..$$,....$$$$......$$$..$$$.^Y$c$$^$$c:....^$$..$$^^^^...$$$,.....$$$..$$$.^Y$c$$" \
        "..888_,o8P'88....:d888..888....Y88.`Y8bo,,,o88o.888oo,__.^888,_._,88P..888....Y88" \
        "..MMMMP^`...^YmmMMMM^^..MMM.....YM...`'YMUP^YMM.^^^^YUMMM..^YMMMMMP^...MMM.....YM" \
        "..:,-:::::..:::::::::.....::::.....:::....:...::::.:::..........................." \
        ",;;;'````'..;;;;``;;;;....;;`;;....';;,..;;..;;;'..;;;..........................." \
        "[[[..........[[[,/[[['...,[[.'[[,...'[[,.[[,.[['...[[[..........................." \
        "$$$..........$$$$$$c....c$$$cc$$$c....Y$c$$$c$P....$$'..........................." \
        "`88bo,__,o,..888b.^88bo,.888...888,....^88^888....o88oo,:__......................" \
        "..^YUMMMMMP^.MMMM...^W^..YMM...^^`......^M.^M^....^^^^YUMMM......................";

    std::string pressEnter =
        ".____..____..____..____..____........____..__._..____..____..____." \
        "(  _ \\(  _ \\(  __)/ ___)/ ___)......(  __)(  ( \\(_  _)(  __)(  _ \\" \
        ".) __/.)   /.) _).\\___ \\\\___ \\.......) _)./    /..)(...) _)..)   /" \
        "(__)..(__\\_)(____)(____/(____/......(____)\\_)__).(__).(____)(__\\_)";

    IMAGE("main").WriteData(m_console, 0, 0);
    m_console.WriteData(title.data(), 20, 4, 81, 12, 0x0006);
    m_console.WriteData(pressEnter.data(), 27, 23, 66, 4, m_blink ? 0x0008 : 0x000B);
    m_console.WriteData(2, 28, 0x0008, "Press Ctrl for high scores");

    // Only respond to top-level input when no overlay is up. If e.g. the high-score list is
    // showing, m_ui.GetState() will be HIGHSCORE_LIST and we let that overlay own the input.
    if (m_ui.GetState() != CursorState::ROOT)
        return;

    if (m_input.Released(Button::BUTTON_SELECT))
    {
        SetState(State::STATE_SHOP);
        //SetState(State::STATE_PASSIVE);
    }
    // Ctrl (BUTTON_PASSIVES = RCtrl) opens the high-score list overlay.
    if (m_input.Released(Button::BUTTON_PASSIVES))
    {
        PushHighScoreList();
    }
}

void DungeonCrawl::DrawDoorsScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 0, ToAttribute(m_currentFloorPtr->type));

    // Draw the individual doors
    DrawDoors(delta);

    // Draw the party
    DrawHero(delta);

    // Draw the cursor over everything
    DrawCursor();

    // Create a delay before transitioning to next room
    if (m_timeLeft != Time::Zero)
    {
        m_timeLeft -= delta;
        if (m_timeLeft < Time::Zero)
            m_timeLeft = Time::Zero;
    }
    if (m_currentRoom && m_timeLeft == Time::Zero)
    {
        SetState(m_currentRoom->door.state);
    }
}

void DungeonCrawl::DrawShopScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 1, ToAttribute(m_currentFloorPtr->type));

    // Draw the shop screen
    DrawShop(delta);

    // Draw the party
    DrawHero(delta);

    // Draw the cursor over everything
    DrawCursor();
}

void DungeonCrawl::DrawCombatScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 1, ToAttribute(m_currentFloorPtr->type));

    // Draw the monsters
    DrawMonsters(delta);

    // Draw the party
    DrawHero(delta);

    // Draw the cursor over everything
    if (m_state == State::STATE_COMBAT)
        DrawCursor();

    if (m_state == State::STATE_COMBAT_RESOLVE)
        DrawAction(delta);
}

void DungeonCrawl::DrawTreasureScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 2, ToAttribute(m_currentFloorPtr->type));

    // Draw the rewards
    DrawReward(delta);

    // Draw the party
    DrawHero(delta);

    // Handle cursor state
    DrawCursor();
}

void DungeonCrawl::DrawPassiveScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 2, ToAttribute(m_currentFloorPtr->type));

    // Draw the heroes
    DrawHero(delta);

    // Draw the passives
    DrawPassiveOptions(delta);

    // Handle cursor state
    DrawCursor();
}

void DungeonCrawl::DrawFountain(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 1, ToAttribute(m_currentFloorPtr->type));

    bool complete;
    m_fountain.WriteData(m_console, delta, 74, 4, complete);

    // Draw the party
    DrawHero(delta);

    // Draw the fairies
    DrawFairies(delta);

    // Artifact fountain overlay: draws only the artifact reward in the middle. The exit icon
    // is already drawn by DrawFairies above at (2, 3) with its own cursor-index-0 highlight,
    // so we don't duplicate it here. The fountain still heals + revives + spawns fairies; this
    // just adds the claim UI for the artifact reward.
    if (m_currentRoom != nullptr
        && m_currentRoom->door.rarity == Rarity::ARTIFACT
        && !m_currentRoom->rewardWeapons.empty())
    {
        const int artifactX = 40;
        const int artifactY = 4;
        DrawItem(delta, &m_currentRoom->rewardWeapons[0], artifactX, artifactY);
        if (m_ui.GetState() == CursorState::FOUNTAIN && m_ui.GetCursorIndex() == 1)
            m_ui.GetAnimation().WriteData(m_console, delta, artifactX, artifactY, complete);
    }

    // Draw cursor
    DrawCursor();
}

void DungeonCrawl::DrawTrapRoom(Time delta)
{
    // Draw the doors first
    DrawTrap(delta, ToAttribute(m_currentFloorPtr->type), m_trapInitiated, m_trapTriggered);

    // Draw the party
    DrawHero(delta);

    // Draw cursor
    DrawCursor();
}

void DungeonCrawl::NextFloor(Time delta)
{
    // Draw the doors first
    DrawStairs(delta, ToAttribute(m_currentFloorPtr->type));

    // Draw the party
    DrawHero(delta);

    // Create a delay before transitioning to next floor
    if (m_timeLeft != Time::Zero)
    {
        m_timeLeft -= delta;
        if (m_timeLeft < Time::Zero)
            m_timeLeft = Time::Zero;
    }
    if (m_timeLeft == Time::Zero)
        SetState(State::STATE_DOORS);
}

void DungeonCrawl::AddHero()
{
    if (m_heroes.size() >= 4)
        return;

    Weapon armor;
    armor.armorClass = 0;
    armor.name = "Cloth";
    armor.rarity = Rarity::COMMON;
    armor.resistances.clear();

    Hero hero;
    hero.index = (int)m_heroes.size();
    hero.armor = armor;
    hero.totalHp = GetRandomValue(24, 36);
    hero.totalMp = GetRandomValue(12, 18);
    hero.currentHp = hero.totalHp;
    hero.currentMp = hero.totalMp;
    hero.experience = 4;
    hero.level = 1;
    hero.weapon1 = WEAPON("Unarmed");
    hero.weapon2 = WEAPON("Lesser Healing Potion");
    //hero.weapon2 = WEAPON("Lesser Resurrect Potion");
    hero.weapon2.Randomize();
    hero.weapon3 = WEAPON("Unarmed");
    hero.weapon4 = WEAPON("Unarmed");
    hero.dead = ANIMATION("hero_dead");
    hero.levelUp = ANIMATION("hero_level");
    hero.idle = ANIMATION("hero_border");
    hero.isTurn = false;
    hero.condition = Die(0, 0, 0, DamageType::NORMAL);
    hero.levelUpTimeLeft = Time::Zero;

    hero.x = (25 * ((int)m_heroes.size() + 1)) - 25;
    hero.y = 18;

    m_heroes.push_back(hero);
}

void DungeonCrawl::DrawDungeon(Time delta)
{
    if (m_state == State::STATE_MAIN)
        return;

    bool complete;
    int tileCount = 0;
    for (int index = (m_floor / 5); index < (m_floor / 5) + 7; index++)
    {
        //if (index >= m_dungeon.Size())
        //    IMAGE("side_tile_background_3").WriteData(m_console, 100, tileCount * 4);
        //else
        {
            m_tiles[index].WriteData(m_console, delta, 100, tileCount * 4, complete);
            if (index > 2)
                m_console.WriteData(101, (tileCount * 4) + 2, 0x0007, "%d", ((index - 4) * 5) + 5);
        }
        tileCount++;
    }
    if ((m_floor / 5) % 2)
    {
        IMAGE("side_tile_background_4_left").WriteData(m_console, 100, 28);
    }
    else
    {
        IMAGE("side_tile_background_4_right").WriteData(m_console, 100, 28);
    }

    if ((m_floor % 5) != 0 || m_state != State::STATE_NEXT_FLOOR)
    {
        int playerDirection = (m_floor / 5) % 2 == 0 ? -1 : 1;
        int playerPos = playerDirection == -1 ? 112 + ((m_floor % 5) * playerDirection) : 107 + ((m_floor % 5) * playerDirection);
        m_console.WriteData(playerPos, 14, BLINK(0x0004), "O");
    }
    else if (m_state == State::STATE_NEXT_FLOOR)
    {
        if ((m_floor / 5) % 2)
        {
            m_console.WriteData(106, 13, BLINK(0x0004), "O");
        }
        else
        {
            m_console.WriteData(113, 13, BLINK(0x0004), "O");
        }
    }

    if ((m_floor / 5) < 2)
    {
        static Animation sideBird1 = ANIMATION("side_bird");
        static int direction1 = -1;
        static int bird1x, bird1y = 0;
        static Time bird1appear = ToMilliseconds(GetRandomValue(800, 1000));
        static Time bird1move = ToMilliseconds(800);
        if (bird1appear > Time::Zero)
        {
            bird1appear -= delta;
            if (bird1appear < Time::Zero)
            {
                bird1appear = Time::Zero;
                direction1 = GetRandomValue(0, 1) == 0 ? -1 : 1;
                bird1x = direction1 == 1 ? 100 : 120;
                if ((m_floor / 5) == 0)
                    bird1y = GetRandomValue(4, 8);
                if ((m_floor / 5) == 1)
                    bird1y = GetRandomValue(0, 4);
            }
        }
        else
        {
            bird1move -= delta;
            if (bird1move < Time::Zero)
            {
                bird1move = ToMilliseconds(GetRandomValue(600, 1200));
                bird1x += direction1;
                if (bird1x == 100 || bird1x == 120)
                    bird1appear = ToMilliseconds(GetRandomValue(100, 5000));
            }
            sideBird1.WriteData(m_console, delta, bird1x, bird1y, complete);
        }
    }
}

void DungeonCrawl::DrawStairs(Time delta, uint16_t attribute)
{
    bool complete = false;

    // Draw stairs, panel, and footer
    m_stairs.SetAttributes(0, 0x0008);
    m_stairs.SetAttributes(1, attribute == 0x0007 ? 0x0008 : attribute);
    m_stairs.WriteData(m_console, delta, 0, 0, complete);

    // Draw the floor and parties money
    m_console.WriteData(2, 27, 0x0007, "Floor: %d", m_floor);
    m_console.WriteData(90, 27, 0x0006, "o", m_floor);
    m_console.WriteData(92, 27, 0x0007, "%d", m_gold);
    DrawPassiveXP(delta);
}

void DungeonCrawl::DrawPassiveXP(Time delta)
{
    const int visible = m_passiveXP < 5 ? m_passiveXP : 5;
    for (int i = 0; i < visible; ++i)
    {
        const int x = 26 + (i * 11);
        const int y = 27;
        IMAGE("passive_xp").WriteData(m_console, x, y);
    }
}

void DungeonCrawl::DrawMenu(Time delta)
{
    if (!m_showMenu)
        return;
    if (m_ui.GetState() != CursorState::MENU
        && m_ui.GetState() != CursorState::MENU_CREDITS
        && m_ui.GetState() != CursorState::MENU_RESTART)
        return;

    int x = 52;
    int y = 6;
    IMAGE("menu").WriteData(m_console, x, y);
    m_console.WriteData(x + 4, y + 2, m_ui.GetCursorIndex() == 0 ? BLINK(0x0007) : 0x0007, "Restart");
    m_console.WriteData(x + 4, y + 4, m_ui.GetCursorIndex() == 1 ? BLINK(0x0007) : 0x0007, "Credits");
    m_console.WriteData(x + 4, y + 6, m_ui.GetCursorIndex() == 2 ? BLINK(0x0007) : 0x0007, "Exit Game");
}

void DungeonCrawl::DrawRestart(Time delta)
{
    if (!m_showMenu)
        return;
    if (m_ui.GetState() != CursorState::MENU_RESTART)
        return;

    int x = 44;
    int y = 7;
    IMAGE("menu_restart_dialog").WriteData(m_console, x, y);
    m_console.WriteData(x + 6, y + 6, m_ui.GetCursorIndex() == 0 ? BLINK(0x0007) : 0x0007, " NO ");
    m_console.WriteData(x + 21, y + 6, m_ui.GetCursorIndex() == 1 ? BLINK(0x0007) : 0x0007, " YES " );
}

// --------------------------------------------------------------------------------------------------------------------
// High-score: initials entry shown after a party wipe.
// --------------------------------------------------------------------------------------------------------------------
void DungeonCrawl::DrawHighScoreEntry(Time delta)
{
    // Drive the input here so this screen is self-contained.
    // Up/Down: cycle letter at the current position. Left/Right: move position. Enter: confirm
    // all three letters and return to main. Backspace: rewind position.
    if (m_highScoreInputCooldown > Time::Zero)
        m_highScoreInputCooldown -= delta;

    if (m_ui.GetState() == CursorState::HIGHSCORE_INITIALS && !(m_highScoreInputCooldown > Time::Zero))
    {
        auto cycle = [](char c, int delta) -> char
        {
            // Wrap A..Z inclusive.
            int v = (c - 'A' + delta + 26) % 26;
            return (char)('A' + v);
        };
        if (m_input.Released(Button::BUTTON_UP))
            m_initials[m_initialsPosition] = cycle(m_initials[m_initialsPosition], +1);
        if (m_input.Released(Button::BUTTON_DOWN))
            m_initials[m_initialsPosition] = cycle(m_initials[m_initialsPosition], -1);
        if (m_input.Released(Button::BUTTON_RIGHT) && m_initialsPosition < 2)
            m_initialsPosition++;
        if (m_input.Released(Button::BUTTON_LEFT) && m_initialsPosition > 0)
            m_initialsPosition--;
        if (m_input.Released(Button::BUTTON_BACK) && m_initialsPosition > 0)
            m_initialsPosition--;

        if (m_input.Released(Button::BUTTON_SELECT))
        {
            // Commit the typed initials into the inserted record and persist.
            const std::string initials(m_initials, 3);
            m_highScores.SetInitialsAt(m_lastRank, initials);
            m_currentRun.initials = initials;
            m_highScores.Save();
            SetState(State::STATE_MAIN);
            return;
        }
    }

    // Render. Use a plain background and draw stat lines plus an editable initials field.
    DrawBackground(delta, 2, 0x0004);

    int x = 30;
    int y = 4;
    m_console.WriteData(x, y++, 0x000C, "G A M E   O V E R");
    y++;

    if (m_lastRank > 0)
        m_console.WriteData(x, y++, 0x000E, "NEW HIGH SCORE - RANK %d", m_lastRank);
    else
        m_console.WriteData(x, y++, 0x0008, "(didn't make the top %d)", HighScoreFile::kMaxScores);
    y++;

    m_console.WriteData(x, y++, 0x0007, "Floor reached    : %d", m_currentRun.floor);
    m_console.WriteData(x, y++, 0x0007, "Monsters killed  : %d", m_currentRun.TotalKills());
    m_console.WriteData(x, y++, 0x0008, "  Common    : %d", m_currentRun.killsCommon);
    m_console.WriteData(x, y++, 0x0008, "  Rare      : %d", m_currentRun.killsRare);
    m_console.WriteData(x, y++, 0x0008, "  Epic      : %d", m_currentRun.killsEpic);
    m_console.WriteData(x, y++, 0x0008, "  Legendary : %d", m_currentRun.killsLegendary);
    y++;
    m_console.WriteData(x, y++, 0x0007, "Hero levels      : ");
    for (size_t i = 0; i < m_currentRun.heroLevels.size(); i++)
        m_console.WriteData(x + 19 + (int)i * 4, y - 1, 0x000F, "%d  ", m_currentRun.heroLevels[i]);
    y++;

    m_console.WriteData(x, y++, 0x000F, "Enter your initials:");
    int ix = x + 22;
    for (int i = 0; i < 3; i++)
    {
        uint16_t attr = (i == m_initialsPosition) ? BLINK(0x000E) : 0x0007;
        char buf[2] = { m_initials[i], 0 };
        m_console.WriteData(ix + i * 2, y - 1, attr, "%s", buf);
    }
    y += 2;
    m_console.WriteData(x, y++, 0x0008, "UP/DOWN: letter   LEFT/RIGHT: position   ENTER: save");
}

// --------------------------------------------------------------------------------------------------------------------
// High-score: read-only list overlay, shown from the main menu by pressing Ctrl.
// --------------------------------------------------------------------------------------------------------------------
void DungeonCrawl::DrawHighScoreList(Time delta)
{
    if (m_ui.GetState() != CursorState::HIGHSCORE_LIST)
        return;

    // Any of these closes the overlay and returns to the main menu. Note we intentionally do
    // NOT close on BUTTON_PASSIVES (Ctrl) here, because that's the same key that opened the
    // overlay - DrawMainScreen consumes the Released event to push the list, then this draw
    // would consume the same event to pop it off in the same frame.
    if (m_input.Released(Button::BUTTON_BACK)
        || m_input.Released(Button::BUTTON_SELECT)
        || m_input.Released(Button::BUTTON_MENU))
    {
        m_ui.PopBack(1);
        return;
    }

    int x = 22;
    int y = 2;

    Image& background = IMAGE("main");
    background.SetAttribute(0x0008);
    background.WriteData(m_console, 0, 0);
    for (int i = 20; i < 97; i++)
    {
        for (int j = 2; j < 22; j++)
        {
            m_console.SetData(L" ", i, j, 1, 1, 0x0008);
        }
    }

    // Draw a black backdrop block behind the table so it pops over the menu art.
    //m_console.SetData('#', x, y, 5, 5, 0x0008);

    m_console.WriteData(x + 25, y++, 0x000E, "H I G H   S C O R E S");
    y++;

    m_console.WriteData(x,      y, 0x000F, "Rank");
    m_console.WriteData(x +  6, y, 0x000F, "Tag");
    m_console.WriteData(x + 12, y, 0x000F, "Floor");
    m_console.WriteData(x + 20, y, 0x000F, "Kills");
    m_console.WriteData(x + 36, y, 0x000F, "Hero Lvl");
    m_console.WriteData(x + 57, y, 0x000F, "Max Damage");
    y++;
    m_console.WriteData(x, y++, 0x0008, "------------------------------------------------------------------------");

    const auto& scores = m_highScores.GetAll();
    for (int i = 0; i < HighScoreFile::kMaxScores; i++)
    {
        uint16_t attr = ((i + 1) == m_lastRank) ? 0x000E : 0x0007;
        if (i < (int)scores.size())
        {
            const RunStats& s = scores[i];
            m_console.WriteData(x,      y, attr, "%2d.",  i + 1);
            m_console.WriteData(x +  6, y, attr, "%s",    s.initials.c_str());
            m_console.WriteData(x + 12, y, attr, "%d",    s.floor);
            m_console.WriteData(x + 20, y, ToAttribute(Rarity::COMMON), "%2d", s.killsCommon);
            m_console.WriteData(x + 23, y, ToAttribute(Rarity::RARE), "%2d", s.killsRare);
            m_console.WriteData(x + 26, y, ToAttribute(Rarity::EPIC), "%2d", s.killsEpic);
            m_console.WriteData(x + 29, y, ToAttribute(Rarity::LEGENDARY), "%2d", s.killsLegendary);
            if (s.killsArtifact > 0)
                m_console.WriteData(x + 32, y, ToAttribute(Rarity::ARTIFACT), "%2d", s.killsArtifact);
            if (s.heroLevels.size() > 0)
                m_console.WriteData(x + 36, y, attr, "%d",    s.heroLevels[0]);
            if (s.heroLevels.size() > 1)
                m_console.WriteData(x + 39, y, attr, "%d",    s.heroLevels[1]);
            if (s.heroLevels.size() > 2)
                m_console.WriteData(x + 42, y, attr, "%d",    s.heroLevels[2]);
            if (s.heroLevels.size() > 3)
                m_console.WriteData(x + 45, y, attr, "%d",    s.heroLevels[3]);
            m_console.WriteData(x + 57, y, attr, "%2d", s.maxDamage);
        }
        else
        {
            m_console.WriteData(x,      y, 0x0008, "%2d.", i + 1);
            m_console.WriteData(x +  6, y, 0x0008, "---");
        }
        y++;
    }
    y++;
    //m_console.WriteData(x, y, 0x0008, "Press BACK / ENTER / ESC to close");
}

void DungeonCrawl::PushHighScoreEntry()
{
    CursorContext context;
    context.state = CursorState::HIGHSCORE_INITIALS;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = 0;
    context.minIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushHighScoreList()
{
    CursorContext context;
    context.state = CursorState::HIGHSCORE_LIST;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = 0;
    context.minIndex = 0;
    context.direction = CursorIndexDirection::VERTICAL;
    m_ui.PushBack(context);
}

// --------------------------------------------------------------------------------------------------------------------
// Hover dialog: list boss-dragon modifiers when the player is aiming at a modded monster.
// --------------------------------------------------------------------------------------------------------------------
void DungeonCrawl::DrawDragonModifierDialog(Time delta)
{
    if (m_ui.GetState() != CursorState::COMBAT_MONSTERS)
        return;
    if (!m_currentRoom)
        return;

    const int idx = m_ui.GetCursorIndex();
    if (idx < 0 || idx >= (int)m_currentRoom->monsters.size())
        return;

    const Monster& monster = m_currentRoom->monsters[idx];
    if (monster.modifiers.empty())
        return;

    // Box anchored at the spot the spec called out. The passive_dialog_short art is wide enough
    // for two text lines; one modifier per line.
    const int x = 1;
    const int y = 15;
    IMAGE("passive_dialog_short").WriteData(m_console, x, y);

    // Header + up to 2 modifier rows.
    m_console.WriteData(x + 2, y, 0x000E, "Dragon Modifiers");
    for (size_t i = 0; i < monster.modifiers.size() && i < 2; ++i)
    {
        const std::string text = ToString(monster.modifiers[i]);
        m_console.WriteData(x + 2, y + 1 + (int)i, 0x000F, "%s", text.c_str());
    }

    // For ElementImmune, append the immune element name on the same line as that modifier so the
    // player can tell which element to avoid.
    for (size_t i = 0; i < monster.modifiers.size() && i < 2; ++i)
    {
        if (monster.modifiers[i] == BossModifier::ElementImmune
            && monster.immuneElement != DamageType::INVALID)
        {
            const std::string suffix = std::string(" (") + ToString(monster.immuneElement) + ")";
            // Suffix is drawn in the immune element's color so it stands out.
            const std::string base = ToString(monster.modifiers[i]);
            m_console.WriteData(x + 2 + (int)base.size(), y + 2 + (int)i,
                ToAttribute(monster.immuneElement), "%s", suffix.c_str());
        }
    }
}

void DungeonCrawl::DrawBackground(Time delta, int index, uint16_t attribute)
{
    bool complete = false;

    // Draw background, panel, and footer
    Animation& background = index == 0 ? ANIMATION("background_1") : ANIMATION("background_2");
    background.SetAttributes(0, 0x0008);
    background.SetAttributes(1, attribute);
    background.WriteData(m_console, delta, 0, 0, complete);

    // Draw the floor and parties money
    m_console.WriteData(2, 27, 0x0007, "Floor: %d", m_floor);
    m_console.WriteData(90, 27, 0x0006, "o", m_floor);
    m_console.WriteData(92, 27, 0x0007, "%d", m_gold);
    DrawPassiveXP(delta);

    // Draw torches
    static Animation torches[4] = { ANIMATION("torch"), ANIMATION("torch"), ANIMATION("torch"), ANIMATION("torch") };
    torches[0].WriteData(m_console, delta, index == 0 ? 9 : 17, index == 0 ? 7 : 5, complete);
    torches[1].WriteData(m_console, delta, index == 0 ? 36 : 81, index == 0 ? 7 : 5, complete);
    if (index == 0)
    {
        torches[1].WriteData(m_console, delta, 63, 7, complete);
        torches[2].WriteData(m_console, delta, 90, 7, complete);
    }
}

void DungeonCrawl::DrawTrap(Time delta, uint16_t attribute, bool showExit, bool triggered)
{
    bool complete;

    // Draw background, panel, and footer
    Animation& background = showExit ? ANIMATION("trap") : ANIMATION("trap");
    background.SetAttributes(0, 0x0008);
    background.SetAttributes(1, attribute == 0x0007 ? 0x0008 : attribute);
    background.WriteData(m_console, delta, 0, 0, complete);

    if (m_currentRoom->trap == TrapType::TRAP_SPIKES)
    {
        Animation& trap = !triggered ? ANIMATION("spikes_1") : ANIMATION("spikes_2");
        trap.WriteData(m_console, delta, 0, 0, complete);
    }
    else if (m_currentRoom->trap == TrapType::TRAP_SWINGINGAXE)
    {
        Animation& trap = !triggered ? ANIMATION("swinging_axe_1") : ANIMATION("swinging_axe_2");
        trap.WriteData(m_console, delta, 0, 0, complete);
    }

    // Draw the floor and parties money
    m_console.WriteData(2, 27, 0x0007, "Floor: %d", m_floor);
    m_console.WriteData(90, 27, 0x0006, "o", m_floor);
    m_console.WriteData(92, 27, 0x0007, "%d", m_gold);
    DrawPassiveXP(delta);

    // Draw torches
    static Animation torches[4] = { ANIMATION("torch"), ANIMATION("torch"), ANIMATION("torch"), ANIMATION("torch") };
    torches[0].WriteData(m_console, delta, 36, 5, complete);
    torches[1].WriteData(m_console, delta, 59, 5, complete);

    bool heroesAlive = false;
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp > 0)
        {
            heroesAlive = true;
        }
    }

    if (showExit && heroesAlive)
    {
        ANIMATION("exit").WriteData(m_console, delta, 2, 3, complete);
        if (m_ui.GetCursorIndex() == 0)
            m_ui.GetAnimation().WriteData(m_console, delta, 2, 3, complete);

        m_console.WriteData(3, 4, 0x0007, "CHALLENGE: %d", m_trapDC);
        m_console.WriteData(3, 5, m_trapRoll > m_trapDC ? 0x0004 : 0x0002, "ROLL: %d", m_trapRoll);
    }
}

void DungeonCrawl::DrawDoors(Time delta)
{
    bool complete;
    for (int index = 0; index < (int)m_currentFloorPtr->rooms.size(); index++)
    {
        Door& door = m_currentFloorPtr->rooms[index].door;
        const int x = 27 * (index + 1) - 22;
        const int y = 1;

        // Draw the door
        if (door.opened)
            door.open.WriteData(m_console, delta, x, y, complete);
        else
            door.close.WriteData(m_console, delta, x, y, complete);

        // Draw the cursor
        if (index == m_ui.GetCursorIndex() && m_ui.GetState() == CursorState::DOOR)
            m_ui.GetAnimation().WriteData(m_console, delta, x + 10, 0, complete);

        // Draw the cursor
        //if (index == m_cursorIndex && m_cursorState == CursorState::DOOR)
        //	m_cursor.WriteData(m_console, delta, x + 10, 0, complete);

        if (m_currentRoom)
        {
            // Display a preview of the other doors contents
            uint16_t attribute = ToAttribute(m_currentFloorPtr->rarity);
            for (int rm = 0; rm < (int)m_currentFloorPtr->rooms.size(); rm++)
            {
                Room& room = m_currentFloorPtr->rooms[rm];
                if (rm == index)
                {
                    if (room.shop.size() > 0)
                        m_console.WriteData(x + 14, 7, attribute, "Shop");
                    else if (room.door.state == State::STATE_FOUNTAIN)
                        m_console.WriteData(x + 14, 7, attribute, "Fountain");
                    else if (room.door.state == State::STATE_TRAP)
                        m_console.WriteData(x + 14, 7, attribute, "Trap");
                    else if (room.monsters.size() == 1)
                        m_console.WriteData(x + 14, 7, attribute, "1 Monster");
                    else if (room.monsters.size() > 1)
                    {
                        int count = 0;
                        for (int index = 0; index < (int)room.monsters.size(); index++)
                        {
                            if (room.monsters[index].currentHp > 0)
                                count++;
                        }
                        m_console.WriteData(x + 14, 7, attribute, "%d Monster", count);
                    }
                    break;
                }
            }
        }
        else
        {
            // LEATHER_SCOUT: gates leather-based door previews
            int sightCount = 0;
            if (OwnsPassive(PassiveType::LEATHER_SCOUT))
            {
                for (int heroIndex = 0; heroIndex < (int)m_heroes.size(); heroIndex++)
                {
                    if (m_heroes[heroIndex].armor.target == Target::PLAYERAC_SPEED
                        && m_heroes[heroIndex].armor.rarity >= Rarity::COMMON
                        && m_heroes[heroIndex].currentHp > 0)
                    {
                        sightCount++;
                    }
                }
            }

            // Display a preview of the other doors contents
            uint16_t attribute = ToAttribute(m_currentFloorPtr->rarity);
            for (int rm = 0; rm < (int)m_currentFloorPtr->rooms.size(); rm++)
            {
                Room& room = m_currentFloorPtr->rooms[rm];
                if (rm == index)
                {
                    if (room.shop.size() > 0 && sightCount > 0)
                        m_console.WriteData(x + 14, 7, attribute, "Shop");
                    else if (room.door.state == State::STATE_FOUNTAIN && sightCount == 4)
                        m_console.WriteData(x + 14, 7, attribute, "Fountain");
                    else if (room.door.state == State::STATE_TRAP && sightCount > 1)
                        m_console.WriteData(x + 14, 7, attribute, "Trap");
                    else if (room.monsters.size() == 1 && sightCount == 4)
                        m_console.WriteData(x + 14, 7, attribute, "1 Monster");
                    else if (room.monsters.size() > 1 && sightCount == 4)
                    {
                        int count = 0;
                        for (int index = 0; index < (int)room.monsters.size(); index++)
                        {
                            if (room.monsters[index].currentHp > 0)
                                count++;
                        }
                        m_console.WriteData(x + 14, 7, attribute, "%d Monster", count);
                    }
                    break;
                }
            }
        }

        // Draw the label
        m_console.WriteData(x + 11, 1, ToAttribute(door.rarity), "%s", door.label.c_str());
    }
}

void DungeonCrawl::DrawShop(Time delta)
{
    bool complete = false;
    int x = 2;
    int y = 4;

    int index = 0;
    for (int index = 0; index < (int)m_currentRoom->shop.size(); index++)
    {
        DrawItem(delta, &m_currentRoom->shop[index], x, y);

        if (m_ui.GetState() == CursorState::SHOP)
        {
            if (m_ui.GetCursorIndex() == index)
                m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
        }

        x += 20;
    }

    ANIMATION("exit").WriteData(m_console, delta, x, y, complete);
    if (m_ui.GetState() == CursorState::SHOP)
    {
        if (m_ui.GetCursorIndex() == 4)
            m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
    }
}

void DungeonCrawl::DrawHero(Time delta)
{
    bool complete;
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        Hero& hero = m_heroes[index];

        // Dissallow a dead person to have turns
        if (hero.isTurn && hero.currentHp == 0)
            hero.isTurn = false;

        int x = (25 * (index + 1)) - 25;
        int y = hero.isTurn ? 17 : 18;

        // Select border color
        uint16_t borderColor = 0x0007;
        if (hero.condition.die != 0)
            borderColor = ToAttribute(hero.condition.type);
        if (hero.levelUpTimeLeft > Time::Zero)
            borderColor = 0x0006;
        if (hero.currentHp == 0)
            borderColor = 0x0004;

        // Show the hero an at-a-glance preview of how the currently-targeted monster's element
        // will land on their armor:
        //   GREEN border + "RESIST"   -> armor element beats the monster element (0.5x incoming)
        //   RED   border + "WEAKNESS" -> armor element is beaten by the monster element (2x incoming)
        // (Previously the two flags were swapped relative to the damage formula, and the labels
        //  were painted in the element's own color which made resist/weakness look identical.)
        bool hasResist   = false;
        bool hasWeakness = false;
        if (m_ui.GetState() == CursorState::COMBAT_MONSTERS)
        {
            Monster* monster = &m_currentRoom->monsters[m_ui.GetCursorIndex()];
            DamageType type = monster->element;
            for (int res = 0; res < (int)hero.armor.resistances.size(); res++)
            {
                if (hero.armor.resistances[res] == ToStrength(type))
                {
                    // monster's element is what this damage type is STRONG against -> hero is WEAK
                    hasWeakness = true;
                    borderColor = 0x000C; // red
                }
                if (hero.armor.resistances[res] == ToWeakness(type))
                {
                    // monster's element is what this damage type is WEAK against -> hero RESISTS
                    hasResist = true;
                    borderColor = 0x000A; // green
                }
            }
        }

        // Draw border and condition
        hero.idle.SetAttributes(0, borderColor);
        hero.idle.WriteData(m_console, delta, x, y, complete);
        if (hero.protect)
            ANIMATION("hero_protect").WriteData(m_console, delta, x, y, complete);

        if (hero.condition.die != 0)
            m_console.WriteData(x + 6, y, ToAttribute(hero.condition.type), " %s ", ToConditionString(hero.condition.type).c_str());
        if (hasResist)
            m_console.WriteData(x +  2, y + 7, 0x000A, " RESIST ");   // green
        if (hasWeakness)
            m_console.WriteData(x + 13, y + 7, 0x000C, " WEAKNESS "); // red

        // Animate selection
        if (m_ui.GetState() == CursorState::HERO
            || m_ui.GetState() == CursorState::USE_ITEM
            || m_ui.GetState() == CursorState::SHOP_HERO
            || m_ui.GetState() == CursorState::REWARD_HERO
            || m_ui.GetState() == CursorState::COMBAT_HERO
            || m_ui.GetState() == CursorState::ATTRIBUTES)
        {
            if (m_ui.GetCursorIndex() == index)
                m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
        }
        if (m_state == State::STATE_COMBAT
            && (m_ui.GetState() == CursorState::INVENTORY_1 || m_ui.GetState() == CursorState::INVENTORY_2))
        {
            if (m_ui.GetCursorIndex() == index)
                m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
        }

        if (m_ui.GetState() == CursorState::ATTRIBUTES
            || m_showAttributes)
        {
            m_console.WriteData(x + 2, y + 1, 0x0007, "HP: %d/%d", hero.currentHp, hero.totalHp);
            m_console.WriteData(x + 2, y + 2, 0x0007, "MP: %d/%d", hero.currentMp, hero.totalMp);
            m_console.WriteData(x + 2, y + 3, 0x0007, "LV: %d", hero.level);
            m_console.WriteData(x + 2, y + 4, 0x0007, "XP: %d", hero.experience);
            if (m_ui.GetState() != CursorState::SHOP && m_ui.GetState() != CursorState::REWARD)
                m_console.WriteData(x + 4, y + 5, ToAttribute(hero.armor.rarity), "(AC %d) %s", hero.armor.armorClass + hero.level + hero.bonusAC, hero.armor.name.c_str());
            else
            {
                int len = m_console.WriteData(x + 4, y + 5, ToAttribute(hero.armor.rarity), "(AC %d) %s", hero.armor.armorClass, hero.armor.name.c_str());
                m_console.WriteData(x + 4 + len + 1, y + 5, 0x0008, "Base");
            }

            for (int res = 0; res < (int)hero.armor.resistances.size(); res++)
            {
                DamageType resistance = hero.armor.resistances[res];
                m_console.WriteData(x + 4, y + 6 + res, ToAttribute(resistance), ToString(resistance).c_str());
            }
        }
        else
        {
            // Draw HP
            int hp = (int)(16 * float((float)hero.currentHp / (float)hero.totalHp)) + 1;
            if (hp <= 0) hp = 0;
            m_console.WriteData("HP: ", x + 2, y + 1, 4, 1, 0x0007);
            m_console.WriteData("_________________", x + 6, y + 1, 17, 1, 0x0008);
            m_console.WriteData("#################", x + 6, y + 1, hp, 1, 0x0004);

            // Draw MP
            int mp = (int)(16 * float((float)hero.currentMp / (float)hero.totalMp)) + 1;
            if (mp <= 0) mp = 0;
            m_console.WriteData("MP: ", x + 2, y + 2, 4, 1, 0x0007);
            m_console.WriteData("_________________", x + 6, y + 2, 17, 1, 0x0008);
            m_console.WriteData("#################", x + 6, y + 2, mp, 1, 0x0001);

            int levelMultiplier = (hero.level / 3);
            // Stacks on top of the level-scaled levelMultiplier in the buffDie below.
            int spellMultiplier = (int)hero.armor.rarity;
            Die buffDie = Die((hero.armor.target == Target::PLAYERAC_SPELL && hero.weapon1.mpCost) 
                ? spellMultiplier + levelMultiplier : 0 + levelMultiplier,
                0,
                hero.level);

            if (hero.currentMp >= GetEffectiveMpCost(&hero, &hero.weapon1))
            {
                m_console.WriteData(x + 2, y + 3,
                    hero.weapon1.selected ? BLINK(ToAttribute(hero.weapon1.rarity)) : ToAttribute(hero.weapon1.rarity),
                    hero.weapon1.name.c_str());
                if (m_ui.GetState() != CursorState::SHOP && m_ui.GetState() != CursorState::REWARD)
                    (hero.weapon1.die + buffDie).WriteData(m_console, x + 4, y + 4);
                else
                {
                    int len = (hero.weapon1.die).WriteData(m_console, x + 4, y + 4);
                    m_console.WriteData(x + 4 + len + 1, y + 4, 0x0008, "Base");
                }
            }
            else
            {
                m_console.WriteData(x + 2, y + 3,
                    hero.weapon1.selected ? BLINK(0x0007) : 0x0007,
                    hero.weapon1.name.c_str());
                if (m_ui.GetState() != CursorState::SHOP && m_ui.GetState() != CursorState::REWARD)
                    (WEAPON("Unarmed").die + buffDie).WriteData(m_console, x + 4, y + 4);
                else
                {
                    int len = (WEAPON("Unarmed").die).WriteData(m_console, x + 4, y + 4);
                    m_console.WriteData(x + 4 + len + 1, y + 4, 0x0008, "Base");
                }
            }

            if (hero.currentMp >= GetEffectiveMpCost(&hero, &hero.weapon2))
            {
                m_console.WriteData(x + 2, y + 5,
                    hero.weapon2.selected ? BLINK(ToAttribute(hero.weapon2.rarity)) : ToAttribute(hero.weapon2.rarity),
                    hero.weapon2.name.c_str());
                if (m_ui.GetState() != CursorState::SHOP && m_ui.GetState() != CursorState::REWARD)
                    (hero.weapon2.die + buffDie).WriteData(m_console, x + 4, y + 6);
                else
                {
                    int len = (hero.weapon2.die).WriteData(m_console, x + 4, y + 6);
                    m_console.WriteData(x + 4 + len + 1, y + 6, 0x0008, "Base");
                }
            }
            else
            {
                m_console.WriteData(x + 2, y + 5,
                    hero.weapon2.selected ? BLINK(0x0007) : 0x0007,
                    hero.weapon2.name.c_str());
                if (m_ui.GetState() != CursorState::SHOP && m_ui.GetState() != CursorState::REWARD)
                    (WEAPON("Unarmed").die + buffDie).WriteData(m_console, x + 4, y + 6);
                else
                {
                    int len = (WEAPON("Unarmed").die).WriteData(m_console, x + 4, y + 6);
                    m_console.WriteData(x + 4 + len + 1, y + 6, 0x0008, "Base");
                }
            }
        }

        // Draw level up animation
        if (hero.levelUpTimeLeft > Time::Zero)
        {
            hero.levelUpTimeLeft -= delta;
            if (hero.levelUpTimeLeft < Time::Zero)
                hero.levelUpTimeLeft = Time::Zero;
            hero.levelUp.WriteData(m_console, delta, x, y, complete);

            if (hero.armor.target == Target::PLAYERAC_SLOW)
                m_console.WriteData(x + 5, y + 5, 0x0004, "HP BOOST");
            else if (hero.armor.target == Target::PLAYERAC_SPELL)
                m_console.WriteData(x + 5, y + 5, 0x0001, "MP BOOST");
            else if (hero.armor.target == Target::PLAYERAC_SPEED)
                m_console.WriteData(x + 5, y + 5, 0x0006, "XP BOOST");
        }

        // Draw dead heroes
        if (hero.currentHp == 0)
        {
            hero.dead.WriteData(m_console, delta, x, y, complete);
        }
    }
}

void DungeonCrawl::DrawMonsters(Time delta)
{
    bool complete;

    for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
    {
        // Calculate the monster position
        int x = 0;
        int y = m_currentRoom->boss ? 2 : 3;
        if (m_currentRoom->monsters.size() == 1 && m_currentRoom->boss)
            x = 24 * (index + 1) - 19;
        else if (m_currentRoom->monsters.size() == 1 && !m_currentRoom->boss)
            x = 26 * (index + 1) + 16;
        else if (m_currentRoom->monsters.size() == 2)
            x = 26 * (index + 1) + 2;
        else if (m_currentRoom->monsters.size() == 3)
            x = 25 * (index + 1) - 9;
        else if (m_currentRoom->monsters.size() == 4)
            x = 24 * (index + 1) - 19;

        Monster& monster = m_currentRoom->monsters[index];
        monster.x = x;
        monster.y = y;
        if (monster.currentHp == 0)
            continue;

        if (index == m_ui.GetCursorIndex() && m_ui.GetState() == CursorState::COMBAT_MONSTERS)
        {
            DamageType type = m_ui.GetContext().weapon->type;

            // Tint the targeting cursor by elemental matchup so the player gets a clear
            // good/bad signal:
            //   GREEN  -> attack is super-effective (target's element is one this weapon beats)
            //   RED    -> attack is resisted        (target's element is one that beats this weapon)
            // Previously the cursor was tinted in the matched element's own color, which made
            // weakness and resistance look identical (e.g. Water target showed as blue either way).
            for (int res = 0; res < (int)monster.armor.resistances.size(); res++)
            {
                if (monster.armor.resistances[res] == ToStrength(type))
                {
                    const uint16_t green = 0x000A;
                    m_ui.GetAnimation().SetAttributes(0, green);
                    m_ui.GetAnimation().SetAttributes(1, SOLID(green));
                }
                if (monster.armor.resistances[res] == ToWeakness(type))
                {
                    const uint16_t red = 0x000C;
                    m_ui.GetAnimation().SetAttributes(0, red);
                    m_ui.GetAnimation().SetAttributes(1, SOLID(red));
                }
            }

            m_ui.GetAnimation().WriteData(m_console, delta, x + 2, y - 1, complete);
        }

        m_console.WriteData(x + 3, y, ToAttribute(monster.rarity), monster.name.c_str());

        if (monster.attacking1 || monster.attacking2)
        {
            if (monster.attacking1)
                monster.attack1.WriteData(m_console, delta, x, y, complete);
            else if (monster.attacking2)
                monster.attack2.WriteData(m_console, delta, x, y, complete);
        }
        else
        {
            if (monster.spawning > Time::Zero)
            {
                monster.spawning -= delta;
                if (monster.spawning < Time::Zero)
                    monster.spawning = Time::Zero;

                monster.spawn.WriteData(m_console, delta, x, y, complete);
            }
            else
            {
                monster.idle.WriteData(m_console, delta, x, y, complete);
            }
        }

        if (monster.totalHp != monster.currentHp)
        {
            int hp = (int)(11 * float((float)monster.currentHp / (float)monster.totalHp)) + 1;
            m_console.WriteData("____________", x + 3, y + 2, 12, 1, 0x0008);
            m_console.WriteData("############", x + 3, y + 2, hp, 1, 0x0004);
            m_console.WriteData(x + 6, y + 1, 0x0007, "%d/%d", monster.currentHp, monster.totalHp);
        }
    }
}

void DungeonCrawl::DrawReward(Time delta)
{
    bool complete;

    m_chest.SetAttributes(0, m_chestAttribute);
    m_chest.WriteData(m_console, delta, 38, 6, complete);
    if (!m_chestClosed)
    {
        m_chest.WriteData(m_console, delta, 38, 6, complete);

        if (m_currentRoom->reward == Reward::MONEY
            || m_currentRoom->reward == Reward::RARE_MONEY
            || m_currentRoom->reward == Reward::EPIC_MONEY
            || m_currentRoom->reward == Reward::LEGENDARY_MONEY)
        {}
        else
        {
            // Single reward: original slot (x=21, right-justified against the chest).
            // Two rewards: first slot at x=2 (right-justified against the left side of the
            // chest), second slot at the original x=21. Exit stays at x=62 in either case.
            const int rewardCount = (int)m_currentRoom->rewardWeapons.size();
            const int y = 4;
            int slotX[2] = { 21, 21 };
            if (rewardCount == 2)
            {
                slotX[0] = 2;
                slotX[1] = 21;
            }
            for (int slot = 0; slot < rewardCount; slot++)
            {
                DrawItem(delta, &m_currentRoom->rewardWeapons[slot], slotX[slot], y);

                if (m_ui.GetState() == CursorState::REWARD && m_ui.GetCursorIndex() == slot)
                    m_ui.GetAnimation().WriteData(m_console, delta, slotX[slot], y, complete);
            }
        }

        // Draw the exit button
        {
            int x = 62;
            int y = 4;

            int extraGold = 0;
            for (int index = 0; index < (int)m_heroes.size(); index++)
            {
                if (m_heroes[index].currentHp > 0
                    && m_heroes[index].armor.target == Target::PLAYERAC_SPEED)
                {
                    if (m_heroes[index].armor.rarity >= Rarity::COMMON)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::RARE)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::EPIC)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::LEGENDARY)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::ARTIFACT)
                        extraGold += m_currentRoom->monsters.size();
                }
            }

            // Money-only rewards have no weapon to highlight, so the exit is the only cursor
            // slot (index 0). Weapon rewards put each weapon at indices [0..N-1] and the exit
            // at index N (so 1 reward -> exit at 1, 2 rewards -> exit at 2).
            const bool moneyOnly = (m_currentRoom->reward == Reward::MONEY
                || m_currentRoom->reward == Reward::RARE_MONEY
                || m_currentRoom->reward == Reward::EPIC_MONEY
                || m_currentRoom->reward == Reward::LEGENDARY_MONEY);
            int exitIndex = moneyOnly ? 0 : (int)m_currentRoom->rewardWeapons.size();

            ANIMATION("exit").WriteData(m_console, delta, x, y, complete);
            m_console.WriteData(x + 8, y + 8, 0x0006, "+o   ");
            m_console.WriteData(x + 11, y + 8, 0x0007, "%d", m_currentRoom->gold);
            if (extraGold > 0)
            {
                m_console.WriteData(x + 8, y + 7, 0x0006, "+o   ");
                m_console.WriteData(x + 11, y + 7, 0x0002, "%d", extraGold);
            }

            if (m_ui.GetState() == CursorState::REWARD && m_ui.GetCursorIndex() == exitIndex)
                m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
        }
    }
}

void DungeonCrawl::DrawItem(Time delta, Weapon* weapon, int x, int y)
{
    // Write the name
    m_console.WriteData(x - 1, y - 3, ToAttribute(weapon->rarity), " %s ", weapon->name.data());

    // Write the speed or ac depending on the item
    if (weapon->target != Target::PLAYERAC_SPELL
        && weapon->target != Target::PLAYERAC_SPEED
        && weapon->target != Target::PLAYERAC_SLOW)
    {
        m_console.WriteData(x + 0, y - 2, 0x007, " ");
        weapon->die.WriteData(m_console, x + 1, y - 2);
        m_console.WriteData(x + 9, y - 1, 0x007, " SP %d ", weapon->speed);
    }
    else
        m_console.WriteData(x + 2, y - 2, ToAttribute(weapon->resistances[0]), "AC %d", weapon->armorClass);

    // Grey the weapon out if its purchased or unavailable due to funds
    if (weapon->purchased || m_gold < weapon->gold)
    {
        weapon->idle.SetAttributes(0, 0x0008);
        weapon->idle.SetAttributes(1, 0x0008);
        weapon->idle.SetAttributes(2, 0x0008);
        if (weapon->rarity == Rarity::RARE)
        {
            weapon->idle.SetAttributes(3, 0x0008);
        }
        if (weapon->rarity == Rarity::EPIC)
        {
            weapon->idle.SetStrobe(3, false);
            weapon->idle.SetAttributes(3, 0x0008);
        }
        m_console.WriteData(x - 1, y - 3, 0x0008, " %s ", weapon->name.data());
    }

    bool complete;
    weapon->idle.WriteData(m_console, delta, x, y, complete);
    m_console.WriteData(x + 9, y + 8, 0x0006, "-o   ");
    if (weapon->gold >= weapon->avgGold)
    {
        m_console.WriteData(x + 12, y + 8, 0x0004, "%d", weapon->gold);
    }
    else
    {
        if (!weapon->purchased && weapon->gold > 0)
            m_console.WriteData(x + 1, y + 1, 0x000A, "DEAL");
        m_console.WriteData(x + 12, y + 8, 0x000A, "%d", weapon->gold);
    }

    if (weapon->mpCost > 0)
    {
        m_console.WriteData(x + 0, y - 1, 0x0001, " MP %d ", weapon->mpCost);
    }

    if (weapon->purchased && weapon->gold > 0)
    {
        m_console.WriteData(x + 10, y + 8, 0x0008, "SOLD");
    }
    else if (weapon->purchased && weapon->gold == 0)
    {
        m_console.WriteData(x + 8, y + 8, 0x0008, "TAKEN");
    }
}

void DungeonCrawl::DrawFairies(Time delta)
{
    bool complete;
    int x = 2;
    int y = 3;

    // When the artifact reward is on screen (drawn by DrawFountain at y=4), shift the exit
    // down one row so the two boxes are horizontally aligned, matching the layout used by
    // the reward room where the exit and reward icons share y=4.
    if (m_currentRoom != nullptr
        && m_currentRoom->door.rarity == Rarity::ARTIFACT
        && !m_currentRoom->rewardWeapons.empty())
    {
        y = 4;
    }

    for (int index = 0; index < (int)m_fairies.size(); index++)
    {
        Fairy& fairy = m_fairies[index];

        if (fairy.timeLeft != Time::Zero)
        {
            fairy.timeLeft -= delta;
            if (fairy.timeLeft < Time::Zero)
                fairy.timeLeft = Time::Zero;
        }

        if (fairy.timeLeft == Time::Zero)
        {
            fairy.x += fairy.dir == 0 ? 1 : -1;
            if (fairy.x <= 4)
            {
                fairy.x = 4;
                fairy.dir = 0; // Move right

                fairy.y += GetRandomValue(0, 1) == 0 ? 1 : -1;
                if (fairy.y < 2)
                    fairy.y = 2;
                if (fairy.y > 14)
                    fairy.y = 14;
            }
            if (fairy.x >= 95)
            {
                fairy.x = 95;
                fairy.dir = 1; // Move left

                fairy.y += GetRandomValue(0, 1) == 0 ? 1 : -1;
                if (fairy.y < 2)
                    fairy.y = 2;
                if (fairy.y > 14)
                    fairy.y = 14;
            }

            fairy.y_dust++;
            if (fairy.y_dust >= 17)
            {
                fairy.x_dust = fairy.x;
                fairy.y_dust = fairy.y;
            }

            fairy.timeLeft = ToMilliseconds(GetRandomValue(50, 100));
        }

        if (GetRandomValue(0, 100) > 99)
        {
            fairy.y += GetRandomValue(0, 1) == 0 ? 1 : -1;
            if (fairy.y < 2)
                fairy.y = 2;
            if (fairy.y > 14)
                fairy.y = 14;
        }
        if (GetRandomValue(0, 1000) > 990)
        {
            if (fairy.dir == 0)
                fairy.dir = 1;
            else if (fairy.dir == 1)
                fairy.dir = 0;
        }

        // Draw the fairy and dust
        fairy.fairy.WriteData(m_console, delta, fairy.x, fairy.y, complete);
        fairy.dust.WriteData(m_console, delta, fairy.x_dust, fairy.y_dust, complete);
    }

    ANIMATION("exit").WriteData(m_console, delta, x, y, complete);
    // Only highlight the exit when the fountain itself owns the cursor. Without the state
    // guard, transitioning to REWARD_HERO / HERO (e.g. assigning an artifact reward) would
    // hijack the highlight: those states use the `select_hero` cursor which carries a
    // hero_border underneath, and that border would render on top of the exit box.
    if (m_ui.GetState() == CursorState::FOUNTAIN && m_ui.GetCursorIndex() == 0)
        m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
}

void DungeonCrawl::DrawAction(Time delta)
{
    if (m_actions.size() > 0)
    {
        bool complete;
        auto iter = m_actions.begin();
        Action& action = (*iter);

        // Iterate over all targets and cull actors with 0 HP
        bool hasTargets = true;
        if (action.weapon->target == Target::ENEMY
            || action.weapon->target == Target::ALLENEMIES
            || action.weapon->target == Target::ALLENEMIES)
        {
            action.targets.erase(std::remove_if(action.targets.begin(), action.targets.end(), [](Actor* target) { return target->currentHp == 0; }), action.targets.end());
            hasTargets = action.targets.size() > 0;
        }

        if (hasTargets)
        {
            if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
            {
                Hero* hero = static_cast<Hero*>(action.source);
                hero->isTurn = true;

                if (action.weapon->target == Target::ALLENEMIES)
                {
                    Actor* actor = action.targets[0];
                    if (actor->GetType() == ActorType::ACTOR_MONSTER)
                    {
                        if (m_damageTimeLeft == Time::Zero)
                        {
                            action.weapon->attack.WriteData(m_console, delta, actor->x, actor->y, complete);
                            action.weapon->attack.SetAnimating(false);
                            for (int index = 1; index < (int)action.targets.size(); index++)
                            {
                                Actor* actor = action.targets[index];
                                action.weapon->attack.WriteData(m_console, delta, actor->x, actor->y, complete);
                            }
                            action.weapon->attack.SetAnimating(true);
                        }
                        else
                        {
                            for (int index = 1; index < (int)action.targets.size(); index++)
                            {
                                Actor* actor = action.targets[index];
                                m_console.WriteData(actor->x + 5, actor->y + 8, m_damageAttribute, "%d", std::abs(m_damageFinal));
                                if (m_damageOriginal - m_damageFinal != 0)
                                    m_console.WriteData(actor->x + 8, actor->y + 8, m_damageAttribute2, "(%d)", std::abs(m_damageOriginal - m_damageFinal));
                            }
                        }
                    }
                }
                else if (action.weapon->target == Target::ENEMY)
                {
                    Actor* actor = action.targets[0];
                    if (m_damageTimeLeft == Time::Zero)
                    {
                        action.weapon->attack.WriteData(m_console, delta, actor->x, actor->y, complete);
                    }
                    else
                    {
                        m_console.WriteData(actor->x + 5, actor->y + 8, m_damageAttribute, "%d", std::abs(m_damageFinal));
                        if (m_damageOriginal - m_damageFinal != 0)
                            m_console.WriteData(actor->x + 8, actor->y + 8, m_damageAttribute2, "(%d)", std::abs(m_damageOriginal - m_damageFinal));
                    }
                }
            }
            if (!action.source || action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
            {
                if (action.source)
                {
                    // TODO: Animate hero getting hit
                    Monster* monster = static_cast<Monster*>(action.source);
                    if (action.weapon->altAnimation)
                        monster->attacking2 = true;
                    else
                        monster->attacking1 = true;
                }

                for (int index = 0; index < (int)m_heroes.size(); index++)
                    m_heroes[index].isTurn = false;

                if (m_damageTimeLeft != Time::Zero)
                {
                    Actor* actor = action.targets[0];
                    m_console.WriteData(actor->x + 7, actor->y - 1, m_damageAttribute, " %d ", std::abs(m_damageFinal));
                    if (m_damageOriginal - m_damageFinal != 0)
                        m_console.WriteData(actor->x + 11, actor->y - 1, m_damageAttribute2, "(%d) ", std::abs(m_damageOriginal - m_damageFinal));
                }
            }
        }

        if (m_damageTimeLeft > Time::Zero)
        {
            action.weapon->attack.Reset();
            action.weapon->attack.SetAnimating(true);

            if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
            {
                Monster* monster = static_cast<Monster*>(action.source);
                monster->attacking1 = false;
                monster->attacking2 = false;
                monster->attack1.Reset();
                monster->attack1.SetAnimating(true);
                monster->attack2.Reset();
                monster->attack2.SetAnimating(true);
            }
            else if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
            {
                Hero* hero = static_cast<Hero*>(action.source);
                hero->isTurn = false;
            }

            m_damageTimeLeft -= delta;
            if (m_damageTimeLeft < Time::Zero || !hasTargets)
            {
                m_damageTimeLeft = Time::Zero;
                m_actions.erase(iter);
                m_damageFinal = 0;
                m_damageOriginal = 0;
                m_damageAttribute = 0x0004;
                m_damageAttribute2 = 0x0008;
            }
        }
        else
        {
            if (action.timeLeft > Time::Zero)
            {
                action.timeLeft -= delta;
                if (action.timeLeft < Time::Zero)
                    action.timeLeft = Time::Zero;
            }

            if (action.timeLeft == Time::Zero || (action.source && action.source->currentHp == 0))
            {
                action.weapon->attack.Reset();
                action.weapon->attack.SetAnimating(true);

                if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
                {
                    Monster* monster = static_cast<Monster*>(action.source);
                    monster->attacking1 = false;
                    monster->attacking2 = false;
                    monster->attack1.Reset();
                    monster->attack1.SetAnimating(true);
                    monster->attack2.Reset();
                    monster->attack2.SetAnimating(true);
                }
                else if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
                {
                    Hero* hero = static_cast<Hero*>(action.source);
                    hero->isTurn = false;
                }

                // If we have no more targets skip the action
                if (!hasTargets)
                    m_actions.erase(iter);
                else
                {
                    // Asumming the actor isn't dead, use the weapon
                    if (!action.source || action.source->currentHp > 0)
                    {
                        UseWeapon(action);

                        // If the next queued action is a continuation of this
                        // one (same source hitting the same primary target),
                        // shorten the damage popup and the next swing's
                        // wind-up so multi-attacks (e.g. BOXER) feel snappy.
                        auto next = iter + 1;
                        const bool chainsToNext = next != m_actions.end()
                            && next->source == action.source
                            && !next->targets.empty()
                            && !action.targets.empty()
                            && next->targets[0] == action.targets[0];

                        if (chainsToNext)
                        {
                            m_damageTimeLeft = ToMilliseconds(350);
                            next->timeLeft = ToMilliseconds(200);
                        }
                        else
                        {
                            m_damageTimeLeft = ToMilliseconds(1000);
                        }
                    }
                    else
                    {
                        m_actions.erase(iter);
                    }
                }
            }
        }
    }

    // If all of the actions have been concluded transition to next state
    if (m_actions.size() == 0)
    {
        bool heroesAlive = false;
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0)
                heroesAlive = true;
        }
        bool monstersAlive = false;
        for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
        {
            if (m_currentRoom->monsters[index].currentHp > 0)
                monstersAlive = true;
        }

        // Next round of combat
        if (heroesAlive && monstersAlive)
        {
            // End-of-round bookkeeping for boss-dragon modifiers:
            //   - Increment roundsAlive (StrongerEachTurn reads this for the +25%/round scaling).
            //   - Clear FirstTurnImmune so round 2+ damage lands normally.
            for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
            {
                Monster& m = m_currentRoom->monsters[index];
                if (m.currentHp == 0) continue;
                m.roundsAlive++;
                m.firstTurnImmune = false;
            }

            SetState(State::STATE_COMBAT);
        }
        if (heroesAlive && !monstersAlive)
        {
            // Accumulate Passive XP per defeated monster:
            //   Rare = 1, Epic = 2, Legendary = 4, Dragon = +1 extra
            // Every 4 XP spent at end of combat awards one passive selection.
            // Cap one award per combat; leftover XP carries to the next fight.
            int xpEarned = 0;
            const bool hasDragonSlayer   = OwnsPassive(PassiveType::DRAGON_SLAYER);
            const bool hasSkeletonSlayer = OwnsPassive(PassiveType::SKELETON_SLAYER);
            const bool hasBlobSlayer     = OwnsPassive(PassiveType::BLOB_SLAYER);
            const bool hasBatSlayer      = OwnsPassive(PassiveType::BAT_SLAYER);
            const bool hasSpiderSlayer   = OwnsPassive(PassiveType::SPIDER_SLAYER);
            for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
            {
                const Monster& m = m_currentRoom->monsters[index];
                if (m.rarity == Rarity::RARE)
                    xpEarned += 1;
                else if (m.rarity == Rarity::EPIC)
                    xpEarned += 2;
                else if (m.rarity == Rarity::LEGENDARY)
                    xpEarned += 4;
                else if (m.rarity == Rarity::ARTIFACT)
                    xpEarned += 8;

                if (m.family == MonsterFamily::DRAGON && (int)m.rarity >= (int)Rarity::RARE)
                    xpEarned += 1;

                // DRAGON_SLAYER passive: +2 extra Passive XP per boss kill, regardless of
                // the dragon's rarity (the floor-10 Common dragon still counts as a boss).
                if (hasDragonSlayer && m.family == MonsterFamily::DRAGON)
                    xpEarned += 2;

                // SKELETON_SLAYER / BLOB_SLAYER passives: +1 extra Passive XP per Rare/Epic/
                // Legendary kill of the matching family. Common kills skipped (matches the
                // existing Rare-and-up XP cutoff).
                if (hasSkeletonSlayer && m.family == MonsterFamily::UNDEAD
                    && (int)m.rarity >= (int)Rarity::RARE)
                    xpEarned += 1;
                if (hasBlobSlayer && m.family == MonsterFamily::GELATINOUS
                    && (int)m.rarity >= (int)Rarity::RARE)
                    xpEarned += 1;
                if (hasBatSlayer && m.family == MonsterFamily::RODENT
                    && (int)m.rarity >= (int)Rarity::RARE)
                    xpEarned += 1;
                if (hasSpiderSlayer && m.family == MonsterFamily::ARACHNID
                    && (int)m.rarity >= (int)Rarity::RARE)
                    xpEarned += 1;
            }

            m_passiveXP += xpEarned;

            // Boss kill reward: fully heal living heroes and apply the same MP regen
            // calculation used at attribute-change floors. Bosses drain HP/MP hard, so
            // this is the player's "minor reward" for surviving the fight. ARCANE_BATTERY
            // still adds its per-5-levels MP bump on top, just like the every-5-floor regen.
            // (Hero level-up XP doubling for bosses is applied in ReceiveXP, not here -
            // m_passiveXP and hero.experience are two different XP pools.)
            if (m_currentRoom != nullptr && m_currentRoom->boss)
            {
                const bool hasBattery = OwnsPassive(PassiveType::ARCANE_BATTERY);
                for (int index = 0; index < (int)m_heroes.size(); index++)
                {
                    auto& hero = m_heroes[index];
                    if (hero.currentHp == 0)
                        continue;

                    hero.currentHp = hero.totalHp;

                    int regen = hero.totalMp / 10;                          // 10% baseline
                    if (hero.armor.target == Target::PLAYERAC_SPELL)
                        regen = hero.totalMp / 5;                           // 20% for robe wearers
                    if (hasBattery)
                    {
                        const int tiers = hero.level / 5;                   // 0..4 (capped at 4 since hero.level <= 20)
                        regen += (hero.totalMp * 5 * tiers) / 100;          // +5% per tier
                    }
                    hero.currentMp += regen;
                    if (hero.currentMp > hero.totalMp)
                        hero.currentMp = hero.totalMp;
                }
            }

            if (m_passiveXP >= 5)
            {
                // XP is NOT decremented here - we wait until the player actually confirms
                // their pick on the PASSIVE_SELECT screen so the footer Passive XP icons
                // stay visible (full bar) the whole time they're choosing. The deduction
                // happens in the PASSIVE_SELECT confirm handler.
                SetState(State::STATE_PASSIVE);
            }
            else
                SetState(State::STATE_TREASURE);
        }
        if (!heroesAlive)
        {
            // Party wipe. Capture end-of-run stats (hero levels + inventory snapshot), insert into
            // the high-score table, then enter the initials-entry state.
            m_currentRun.floor = m_floor;
            m_currentRun.heroLevels.clear();
            m_currentRun.inventory.clear();
            for (int index = 0; index < (int)m_heroes.size(); index++)
            {
                m_currentRun.heroLevels.push_back(m_heroes[index].level);
                std::string slot;
                slot.reserve(64);
                slot += m_heroes[index].weapon1.name; slot += '/';
                slot += m_heroes[index].weapon2.name; slot += '/';
                slot += m_heroes[index].weapon3.name; slot += '/';
                slot += m_heroes[index].weapon4.name; slot += '/';
                slot += m_heroes[index].armor.name;
                m_currentRun.inventory.push_back(slot);
            }
            // Default initials are AAA; the player will edit them in the HIGHSCORE state.
            m_currentRun.initials = "AAA";
            m_initials[0] = m_initials[1] = m_initials[2] = 'A';
            m_initialsPosition = 0;
            m_lastRank = m_highScores.Insert(m_currentRun);

            SetState(State::STATE_HIGHSCORE);
        }
    }
}

void DungeonCrawl::DrawPassivesTab(int index, const std::string& label_, bool hasNew)
{
    std::string label = label_;
    if (hasNew)
        label = "* " + label;

    int x = 14 * index + 1;
    int y = 0;

    IMAGE("passives_tab").SetAttribute(m_passivesTabIndex == index ? 0x000F : 0x0008);
    IMAGE("passives_tab").WriteData(m_console, x, y);
    m_console.WriteData(hasNew ? x + 2 : x + 4, y + 1, m_passivesTabIndex == index && m_passivesTab ? BLINK(0x000F) : 0x000F, "%s", label.c_str());
}

void DungeonCrawl::ProcessPassiveInput()
{
    if (m_input.Released(Button::BUTTON_RIGHT))
    {
        if (!m_passivesTab)
            m_passivesX++;
        if (m_passivesTab)
            m_passivesTabIndex++;
    }
    else if (m_input.Released(Button::BUTTON_LEFT))
    {
        if (!m_passivesTab)
            m_passivesX--;
        if (m_passivesTab)
            m_passivesTabIndex--;
    }
    else if (m_input.Released(Button::BUTTON_UP))
    {
        if (!m_passivesTab)
            m_passivesY--;
        if (m_passivesY == -1)
            m_passivesTab = true;
    }
    else if (m_input.Released(Button::BUTTON_DOWN))
    {
        if (!m_passivesTab)
            m_passivesY++;
        if (m_passivesTab)
            m_passivesTab = false;
    }

    if (m_passivesX < 0)
        m_passivesX = 0;
    if (m_passivesY < 0)
        m_passivesY = 0;
    if (m_passivesX > 5)
        m_passivesX = 5;
    if (m_passivesY > 1)
        m_passivesY = 1;

    if (m_passivesTabIndex < 0)
        m_passivesTabIndex = 0;
    if (m_passivesTabIndex > 5)
        m_passivesTabIndex = 5;
}

void DungeonCrawl::ProcessInput()
{
    // Handle pushing the menu above everything
    if (m_input.Released(Button::BUTTON_MENU))
    {
        if (!m_showMenu)
            PushMenu();
        else if (m_showMenu && m_ui.GetState() == CursorState::MENU)
            m_ui.PopBack(1);
        else if (m_showMenu && m_ui.GetState() == CursorState::MENU_CREDITS)
            m_ui.PopBack(1);
        else if (m_showMenu && m_ui.GetState() == CursorState::MENU_RESTART)
            m_ui.PopBack(1);

        m_showMenu = !m_showMenu;
        return;
    }

    // Handle drawing the passives
    if (m_input.Released(Button::BUTTON_PASSIVES))
    {
        m_showPassives = !m_showPassives;
        return;
    }

    // Handle the input of the passives screen
    if (m_showPassives)
    {
        ProcessPassiveInput();
        return;
    }

    // Handle cursor index
    CursorIndexDirection direction = m_ui.GetDirection();
    if ((m_input.Released(Button::BUTTON_RIGHT) && direction == CursorIndexDirection::HORIZONTAL)
        || (m_input.Released(Button::BUTTON_DOWN) && direction == CursorIndexDirection::VERTICAL))
    {
        m_ui.IncrementCursorIndex();
        return;
    }
    if ((m_input.Released(Button::BUTTON_LEFT) && direction == CursorIndexDirection::HORIZONTAL)
        || (m_input.Released(Button::BUTTON_UP) && direction == CursorIndexDirection::VERTICAL))
    {
        m_ui.DecrementCursorIndex();
        return;
    }

    // Handle toggle input
    if (m_input.Released(Button::BUTTON_TOGGLE))
    {
        m_showAttributes = !m_showAttributes;
        return;
    }

    // On neutral screens we should always be able to navigate downward to use items
    if ((m_state == State::STATE_DOORS && m_ui.GetState() == CursorState::DOOR && m_input.Released(Button::BUTTON_DOWN))
        || (m_state == State::STATE_SHOP && m_ui.GetState() == CursorState::SHOP && m_input.Released(Button::BUTTON_DOWN))
        || (m_state == State::STATE_TREASURE && m_ui.GetState() == CursorState::REWARD && m_input.Released(Button::BUTTON_DOWN))
        || (m_state == State::STATE_FOUNTAIN && m_ui.GetState() == CursorState::FOUNTAIN && m_input.Released(Button::BUTTON_DOWN))
        || (m_state == State::STATE_TRAP && m_ui.GetState() == CursorState::TRAP && m_input.Released(Button::BUTTON_DOWN)))
    {
        PushHero();
        return;
    }

    // Hitting the back button always moves backwards
    if (m_input.Released(Button::BUTTON_BACK))
    {
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].weapon1.selected = false;
            m_heroes[index].weapon2.selected = false;
            m_heroes[index].weapon3.selected = false;
            m_heroes[index].weapon4.selected = false;
        }

        if (m_ui.GetState() == CursorState::MENU)
            m_ui.PopBack(1);
        else if (m_ui.GetState() == CursorState::REWARD)
            m_ui.PopBack(3);
        else
            m_ui.PopBack(2);

        if (m_ui.GetState() == CursorState::COMBAT)
        {
            // Refund MP and pop every queued action belonging to the source we're rewinding
            // past (so a wand cast that landed an action no longer leaves its MP spent).
            UndoLastHeroActions();
            PrevHero();
        }
        return;
    }

    // Navigate escape menu
    if (m_ui.GetState() == CursorState::MENU && m_input.Released(Button::BUTTON_SELECT))
    {
        if (m_ui.GetCursorIndex() == 0)
        {
            PushMenuRestart();
        }
        else if (m_ui.GetCursorIndex() == 1)
        {
            // Display credits
            //PushMenuCredits();
        }
        else if (m_ui.GetCursorIndex() == 2)
        {
            // Exit the game
            exit(0);
        }
        return;
    }

    // Navigate restart confirmation dialog
    if (m_ui.GetState() == CursorState::MENU_RESTART && m_input.Released(Button::BUTTON_SELECT))
    {
        if (m_ui.GetCursorIndex() == 0)
        {
            m_ui.PopBack(1);
        }
        else if (m_ui.GetCursorIndex() == 1)
        {
            m_showMenu = false;
            SetState(State::STATE_MAIN);
        }
        return;
    }

    // Hitting the up button moves the button state back to the dungeon
    if (m_ui.GetState() == CursorState::HERO && m_input.Released(Button::BUTTON_UP))
    {
        m_ui.PopBack(2);
        return;
    }

    // Select a hero to use an item
    if (m_ui.GetState() == CursorState::HERO && m_input.Released(Button::BUTTON_SELECT))
    {
        PushHeroItem();
        return;
    }

    // Move to selecting a hero to use an item on
    if (m_ui.GetState() == CursorState::HERO_ITEM)
    {
        if (m_input.Released(Button::BUTTON_SELECT))
        {
            PushUseItem();
            return;
        }
        m_ui.GetContext().source->weapon1.selected = m_ui.GetCursorIndex() == 0;
        m_ui.GetContext().source->weapon2.selected = m_ui.GetCursorIndex() == 1;
        m_ui.GetContext().source->weapon3.selected = m_ui.GetCursorIndex() == 2;
        m_ui.GetContext().source->weapon4.selected = m_ui.GetCursorIndex() == 3;
    }

    // Hero selected to use an item on
    if (m_ui.GetState() == CursorState::USE_ITEM && m_input.Released(Button::BUTTON_SELECT))
    {
        m_ui.GetContext().target = &m_heroes[m_ui.GetCursorIndex()];
        UseSelectedItem();
        m_ui.PopBackTo(2);
        return;
    }

    // Avoid menu accepting input
    if (m_showMenu)
    {
        return;
    }

    // Shop
    {
        if (m_ui.GetState() == CursorState::SHOP && m_input.Released(Button::BUTTON_SELECT))
        {
            if (m_ui.GetCursorIndex() == 4)
            {
                SetState(State::STATE_NEXT_FLOOR);
                m_ui.PopBackTo(1);
                return;
            }

            if (m_gold >= m_currentRoom->shop[m_ui.GetCursorIndex()].gold
                && !m_currentRoom->shop[m_ui.GetCursorIndex()].purchased)
            {
                // Add Hero doesn't require a target
                if (m_currentRoom->shop[m_ui.GetCursorIndex()].target == Target::NEWHERO)
                {
                    m_ui.GetContext().weapon = &m_currentRoom->shop[m_ui.GetCursorIndex()];
                    PurchaseItem();
                    return;
                }

                PushShopHero();
                return;
            }
        }

        if (m_ui.GetState() == CursorState::SHOP_HERO && m_input.Released(Button::BUTTON_SELECT))
        {
            PushShopItem();
            return;
        }

        if (m_ui.GetState() == CursorState::SHOP_HERO_ITEM)
        {
            if (m_input.Released(Button::BUTTON_SELECT))
            {
                PurchaseItem();
                m_ui.PopBackTo(2);
                return;
            }

            m_ui.GetContext().source->weapon1.selected = m_ui.GetCursorIndex() == 0;
            m_ui.GetContext().source->weapon2.selected = m_ui.GetCursorIndex() == 1;
            m_ui.GetContext().source->weapon3.selected = m_ui.GetCursorIndex() == 2;
            m_ui.GetContext().source->weapon4.selected = m_ui.GetCursorIndex() == 3;
        }
    }

    // Passives
    {
        if (m_ui.GetState() == CursorState::PASSIVE_SELECT && m_input.Released(Button::BUTTON_SELECT))
        {
            Passive passive = m_passiveOptions[m_ui.GetCursorIndex()];
            passive.bNew = true;
            passive.owned = true;
            m_passives.push_back(passive);
            m_currentRun.passiveNames.push_back(passive.name);

            // Spend the 5 Passive XP here (deferred from the combat tally that triggered
            // STATE_PASSIVE) so the footer XP icons stay full while the player is choosing
            // and only deplete the moment they commit. Clamp to 0 in case excess XP was
            // accumulated through Dragon Slayer / Fairy Friend / etc.
            m_passiveXP -= 5;
            if (m_passiveXP < 0)
                m_passiveXP = 0;

            // Passive picked - calm the footer Passive XP icons back down.
            IMAGE("passive_xp").SetTrailing(false);

            SetState(State::STATE_TREASURE);
            return;
        }
    }

    // Door
    {
        if (m_ui.GetState() == CursorState::DOOR && m_input.Released(Button::BUTTON_SELECT))
        {
            m_currentRoom = &m_currentFloorPtr->rooms[m_ui.GetCursorIndex()];
            m_timeLeft = ToMilliseconds(800); // Trigger the opening sequence
            m_currentRoom->door.opened = true;
            return;
        }
    }

    // Combat
    {
        if (m_ui.GetState() == CursorState::COMBAT)
        {
            if (m_input.Released(Button::BUTTON_SELECT))
            {
                PushCombatSelection();
                return;
            }

            // Right arrow: skip this hero's turn. Awards a small MP regen (~10% of totalMp,
            // floored at 1) as compensation for forgoing the action. Pops the current COMBAT
            // cursor before NextHero() pushes a fresh one for the next hero.
            if (m_input.Released(Button::BUTTON_RIGHT))
            {
                Actor* source = m_ui.GetContext().source;
                if (source)
                {
                    int regen = source->totalMp / 10;
                    if (regen < 1) regen = 1;
                    source->currentMp += regen;
                    if (source->currentMp > source->totalMp)
                        source->currentMp = source->totalMp;
                }
                m_ui.PopBack(1);
                NextHero();
                return;
            }

            // Left arrow: rewind to the previous alive hero if one exists. No-op at the first
            // hero (we don't want a wrap-around feel; the player should be able to mash Left
            // without accidentally jumping to the last hero).
            if (m_input.Released(Button::BUTTON_LEFT))
            {
                bool hasPrev = false;
                for (int i = m_heroIndex - 1; i >= 0; --i)
                {
                    if (m_heroes[i].currentHp > 0)
                    {
                        hasPrev = true;
                        break;
                    }
                }
                if (hasPrev)
                {
                    // Refund MP and discard any actions the previous hero already committed
                    // so the player can re-pick their swing without losing the MP they spent.
                    UndoLastHeroActions();
                    m_ui.PopBack(1);
                    PrevHero();
                }
                return;
            }

            m_ui.GetContext().source->weapon1.selected = m_ui.GetCursorIndex() == 0;
            m_ui.GetContext().source->weapon2.selected = m_ui.GetCursorIndex() == 1;
            m_ui.GetContext().source->weapon3.selected = m_ui.GetCursorIndex() == 2;
            m_ui.GetContext().source->weapon4.selected = m_ui.GetCursorIndex() == 3;
        }

        if (m_ui.GetState() == CursorState::COMBAT_MONSTERS && m_input.Released(Button::BUTTON_SELECT))
        {
            Action action;
            action.source = m_ui.GetContext().source;
            action.weapon = m_ui.GetContext().weapon;
            action.targets.push_back(&m_currentRoom->monsters[m_ui.GetCursorIndex()]);

            // Charge MP now that the action is actually being queued (was previously charged
            // at weapon-select time, which leaked MP when the player hit Back). Duplicate-
            // attack passives below (WAND_FINESSE, DAGGER_MULTIATTACK) clone this action and
            // do NOT pay extra MP.
            {
                const int cost = GetEffectiveMpCost(action.source, action.weapon);
                if (cost > 0)
                {
                    action.source->currentMp -= cost;
                    if (action.source->currentMp < 0)
                        action.source->currentMp = 0;
                }
            }

            const size_t actionsBegin = m_actions.size();
            m_actions.push_back(action);

            // Apply passives
            if (OwnsPassive(PassiveType::GLOVES_BOXER))
            {
                if (EquippingWeapon(action.source, WeaponType::GLOVES, 2))
                {
                    m_actions.pop_back(); // Replace attack with attacking with both gloves per level

                    // Slower scaling than the original (level/4)+1 formula so
                    // high-level Boxer doesn't drown the round in attacks.
                    const int attacks = (action.source->level / 8) + 1;
                    bool firstSwing = true;
                    for (int index = 0; index < attacks; index++)
                    {
                        action.weapon = &action.source->weapon1;
                        action.firstSwing = firstSwing; firstSwing = false;
                        m_actions.push_back(action);
                        action.weapon = &action.source->weapon2;
                        action.firstSwing = false;
                        m_actions.push_back(action);
                    }
                }
            }

            if (OwnsPassive(PassiveType::DAGGER_DUALWIELD))
            {
                bool enabled = false;
                if (EquippingWeapon(action.source, WeaponType::DAGGER, 2))
                    enabled = true;
                if (EquippingWeapon(action.source, WeaponType::SWORD, 2))
                    enabled = true;
                if (EquippingWeapon(action.source, WeaponType::DAGGER, 1) && EquippingWeapon(action.source, WeaponType::SWORD, 1))
                    enabled = true;

                if (enabled)
                {
                    m_actions.pop_back(); // Replace attack with attacking with both daggers/swords

                    action.weapon = &action.source->weapon1;
                    action.firstSwing = true;
                    m_actions.push_back(action);

                    action.weapon = &action.source->weapon2;
                    action.firstSwing = false;
                    m_actions.push_back(action);
                }
            }

            // DAGGER_MULTIATTACK: each queued dagger swing happens twice (the
            // duplicates can't re-trigger first-swing on-hit passives)
            if (OwnsPassive(PassiveType::DAGGER_MULTIATTACK))
            {
                const size_t end = m_actions.size();
                for (size_t index = actionsBegin; index < end; index++)
                {
                    if (m_actions[index].weapon && m_actions[index].weapon->weaponType == WeaponType::DAGGER)
                    {
                        Action copy = m_actions[index];
                        copy.firstSwing = false;
                        m_actions.push_back(copy);
                    }
                }
            }

            // WAND_FINESSE: each queued wand swing has a 25% chance of attacking again
            if (OwnsPassive(PassiveType::WAND_FINESSE))
            {
                const size_t end = m_actions.size();
                for (size_t index = actionsBegin; index < end; index++)
                {
                    if (m_actions[index].weapon && m_actions[index].weapon->weaponType == WeaponType::WAND)
                    {
                        if (GetRandomValue(0, 3) == 0)
                        {
                            Action copy = m_actions[index];
                            copy.firstSwing = false;
                            m_actions.push_back(copy);
                        }
                    }
                }
            }

            m_ui.PopBackTo(2);
            NextHero();
            return;
        }

        if (m_ui.GetState() == CursorState::COMBAT_HERO && m_input.Released(Button::BUTTON_SELECT))
        {
            Action action;
            action.source = m_ui.GetContext().source;
            action.weapon = m_ui.GetContext().weapon;
            action.targets.push_back(&m_heroes[m_ui.GetCursorIndex()]);

            // Charge MP now (deferred from weapon-select). See COMBAT_MONSTERS path above.
            {
                const int cost = GetEffectiveMpCost(action.source, action.weapon);
                if (cost > 0)
                {
                    action.source->currentMp -= cost;
                    if (action.source->currentMp < 0)
                        action.source->currentMp = 0;
                }
            }

            m_actions.push_back(action);

            m_ui.PopBackTo(2);
            NextHero();
            return;
        }
    }

    // Fountain
    {
        if (m_ui.GetState() == CursorState::FOUNTAIN && m_input.Released(Button::BUTTON_SELECT))
        {
            // Artifact fountain: cursor 0 = exit (advance to next floor), cursor 1 = claim
            // the artifact. Claiming routes through the same hero-picker flow rewards use, so
            // the player picks which hero receives the item and which weapon slot to put it in.
            const bool artifact = (m_currentRoom != nullptr
                && m_currentRoom->door.rarity == Rarity::ARTIFACT
                && !m_currentRoom->rewardWeapons.empty());
            if (artifact && m_ui.GetCursorIndex() == 1)
            {
                if (!m_currentRoom->rewardWeapons[0].purchased)
                {
                    m_selectedRewardIndex = 0;
                    PushRewardHero();
                }
                return;
            }
            SetState(State::STATE_NEXT_FLOOR);
            return;
        }
    }

    // Trap
    {
        if (m_ui.GetState() == CursorState::TRAP && m_input.Released(Button::BUTTON_SELECT))
        {
            PushTrapInitiated();
            return;
        }

        if (m_ui.GetState() == CursorState::TRAP_INITIATED)
        {
            if (m_input.Released(Button::BUTTON_SELECT))
            {
                bool heroesAlive = false;
                for (int index = 0; index < (int)m_heroes.size(); index++)
                {
                    if (m_heroes[index].currentHp > 0)
                        heroesAlive = true;
                }

                if (heroesAlive)
                {
                    SetState(State::STATE_NEXT_FLOOR);
                    return;
                }
                else
                {
                    SetState(State::STATE_MAIN);
                    return;
                }
            }
        }
    }

    // Treasure
    {
        if (m_ui.GetState() == CursorState::CHEST && m_input.Released(Button::BUTTON_SELECT))
        {
            m_chest = ANIMATION("chest_opened");
            m_chestClosed = false;

            // MULTI_REWARD passive: weapon reward rooms get a 50% chance for a second weapon
            // to choose from. Rolled here (chest open) rather than at floor gen so a passive
            // picked up between floor gen and chest open still counts. Gold-only rewards
            // don't trigger this - they have no rewardWeapons[0] slot to mirror.
            if (OwnsPassive(PassiveType::MULTI_REWARD)
                && m_currentRoom != nullptr
                && m_currentRoom->rewardWeapons.size() == 1
                && (m_currentRoom->reward == Reward::RARE_WEAPON
                    || m_currentRoom->reward == Reward::EPIC_WEAPON
                    || m_currentRoom->reward == Reward::LEGENDARY_WEAPON)
                && GetRandomValue(0, 1) == 0)
            {
                m_currentRoom->rewardWeapons.push_back(
                    m_dungeonEx.GenerateBonusWeapon(m_currentRoom->reward));
            }

            m_ui.PopBackTo(2);
            PushReward();
            return;
        }

        if (m_ui.GetState() == CursorState::REWARD && m_input.Released(Button::BUTTON_SELECT))
        {
            int extraGold = 0;
            for (int index = 0; index < (int)m_heroes.size(); index++)
            {
                if (m_heroes[index].currentHp > 0
                    && m_heroes[index].armor.target == Target::PLAYERAC_SPEED)
                {
                    if (m_heroes[index].armor.rarity >= Rarity::COMMON)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::RARE)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::EPIC)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::LEGENDARY)
                        extraGold += m_currentRoom->monsters.size();
                    if (m_heroes[index].armor.rarity >= Rarity::ARTIFACT)
                        extraGold += m_currentRoom->monsters.size();
                }
            }

            // Cursor layout:
            //   money-only:   [0]=exit
            //   1 weapon:     [0]=weapon, [1]=exit
            //   2 weapons:    [0]=weapon, [1]=weapon, [2]=exit
            const bool moneyOnly = (m_currentRoom->reward == Reward::MONEY
                || m_currentRoom->reward == Reward::RARE_MONEY
                || m_currentRoom->reward == Reward::EPIC_MONEY
                || m_currentRoom->reward == Reward::LEGENDARY_MONEY);
            const int rewardCount = moneyOnly ? 0 : (int)m_currentRoom->rewardWeapons.size();
            const int cursor      = m_ui.GetCursorIndex();

            // Exit slot: take gold and leave.
            if (cursor == rewardCount)
            {
                m_gold += m_currentRoom->gold + extraGold;
                SetState(State::STATE_NEXT_FLOOR);
                return;
            }

            // Weapon slot: route to hero picker for whichever reward was selected.
            // Stash the chosen index so PushRewardHero / PushRewardHeroItem can read it.
            if (cursor >= 0 && cursor < rewardCount)
            {
                m_selectedRewardIndex = cursor;
                if (!m_currentRoom->rewardWeapons[cursor].purchased)
                {
                    PushRewardHero();
                }
            }
            return;
        }

        if (m_ui.GetState() == CursorState::REWARD_HERO && m_input.Released(Button::BUTTON_SELECT))
        {
            PushRewardHeroItem();
            return;
        }

        if (m_ui.GetState() == CursorState::REWARD_HERO_ITEM)
        {
            if (m_input.Released(Button::BUTTON_SELECT))
            {
                PurchaseItem();
                // Pop back to whichever screen launched this reward flow (REWARD for reward
                // rooms, FOUNTAIN for artifact fountains). Snapshot was captured in
                // PushRewardHero().
                m_ui.PopBackTo(m_rewardReturnDepth);
                return;
            }

            m_ui.GetContext().source->weapon1.selected = m_ui.GetCursorIndex() == 0;
            m_ui.GetContext().source->weapon2.selected = m_ui.GetCursorIndex() == 1;
            m_ui.GetContext().source->weapon3.selected = m_ui.GetCursorIndex() == 2;
            m_ui.GetContext().source->weapon4.selected = m_ui.GetCursorIndex() == 3;
        }
    }

    m_console.WriteData(0, 0, 0x0008, "%s %d %d", ToString(m_ui.GetState()).c_str(), m_ui.GetCursorIndex(), m_ui.GetSize());
}

void DungeonCrawl::DrawPassiveOptions(Time delta)
{
    bool complete = false;
    int x = 18;
    int y = 4;

    IMAGE("passive_dialog_short").WriteData(m_console, 1, 15);

    int index = 0;
    for (int index = 0; index < (int)m_passiveOptions.size(); index++)
    {
        m_passiveOptions[index].animation.WriteData(m_console, delta, x, y, complete);
        m_console.WriteData(x - 2, y - 1, ToAttribute(m_passiveOptions[index].rarity), "  %s  ", m_passiveOptions[index].name.c_str());

        if (m_ui.GetState() == CursorState::PASSIVE_SELECT)
        {
            if (m_ui.GetCursorIndex() == index)
                m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
        }

        if (m_ui.GetCursorIndex() == index)
        {
            m_console.WriteData(2, 17, 0x0007, "%s", m_passiveOptions[index].description.c_str());
        }

        x += 24;
    }
}

void DungeonCrawl::DrawPassives(Time delta)
{
    if (!m_showPassives)
        return;

    IMAGE("passives").WriteData(m_console, 1, 0);
    auto HasNew = [&](int tabIndex)->bool {
        for (auto passive : m_passives)
        {
            if (passive.tabIndex == tabIndex && passive.bNew)
                return true;
        }
        return false;
    };

    // Draw all tabs
    if (m_passivesTabIndex != 0) DrawPassivesTab(0, "Weapons 1", HasNew(0));
    if (m_passivesTabIndex != 1) DrawPassivesTab(1, "Weapons 2", HasNew(1));
    if (m_passivesTabIndex != 2) DrawPassivesTab(2, "Armor", HasNew(2));
    if (m_passivesTabIndex != 3) DrawPassivesTab(3, "Misc 1", HasNew(3));
    if (m_passivesTabIndex != 4) DrawPassivesTab(4, "Misc 2", HasNew(4));
    if (m_passivesTabIndex != 5) DrawPassivesTab(5, "Misc 3", HasNew(5));
    // Draw selected tabs over rest
    if (m_passivesTabIndex == 0) DrawPassivesTab(0, "Weapons 1", HasNew(0));
    if (m_passivesTabIndex == 1) DrawPassivesTab(1, "Weapons 2", HasNew(1));
    if (m_passivesTabIndex == 2) DrawPassivesTab(2, "Armor", HasNew(2));
    if (m_passivesTabIndex == 3) DrawPassivesTab(3, "Misc 1", HasNew(3));
    if (m_passivesTabIndex == 4) DrawPassivesTab(4, "Misc 2", HasNew(4));
    if (m_passivesTabIndex == 5) DrawPassivesTab(5, "Misc 3", HasNew(5));

    // Draw the greyed borders
    for (int x = 0; x < 6; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            Image image = IMAGE("weapon_border");
            image.SetAttribute(0x0008);
            image.WriteData(m_console, 19 * x + 4, 10 * y + 4);
        }
    }

    // Draw owned passives
    for (auto passive : m_passives)
    {
        if (passive.tabIndex == m_passivesTabIndex)
        {
            int x = 19 * passive.x + 4;
            int y = 10 * passive.y + 4;

            bool complete;
            passive.animation.WriteData(m_console, delta, x, y, complete);
            m_console.WriteData(x + 1, y + 1, 0x0007, "%s", passive.name.c_str());
            if (passive.bNew)
                m_console.WriteData(x + 12, y + 8, 0x000A, "NEW");
        }
    }

    // Draw the cursor
    if (!m_passivesTab)
    {
        int x = 19 * m_passivesX + 4;
        int y = 10 * m_passivesY + 4;

        bool complete;
        m_passivesCursor.WriteData(m_console, delta, x, y, complete);

        for (auto&& passive : m_passives)
        {
            if (passive.x == m_passivesX
                && passive.y == m_passivesY
                && passive.tabIndex == m_passivesTabIndex)
            {
                m_console.WriteData(5, 26, 0x0007, "%s", passive.description.c_str());
                passive.bNew = false;
            }
        }
    }
}

void DungeonCrawl::DrawCursor()
{
    ProcessInput();
}

// Pop every trailing action belonging to the most recent action's source and refund the
// MP that was charged for that source's turn. Used by Back and Left Arrow when the player
// rewinds past a hero who already committed.
//
// Why pop multiple: duplicate-attack passives (Boxer, Dual Wield, Nimble Fingers, Wand
// Finesse) clone the originally-queued action into N copies that all share the same
// .source. We need to drop all of them to fully undo the turn. MP, however, was charged
// exactly once at action-confirm time, so we refund exactly once (based on the last
// action's weapon; for the duplication passives the cloned weapons share the same MP cost
// as the original).
void DungeonCrawl::UndoLastHeroActions()
{
    if (m_actions.empty())
        return;

    Actor*  lastSource = m_actions.back().source;
    Weapon* lastWeapon = m_actions.back().weapon;

    if (lastSource && lastWeapon)
    {
        const int cost = GetEffectiveMpCost(lastSource, lastWeapon);
        if (cost > 0)
        {
            lastSource->currentMp += cost;
            if (lastSource->currentMp > lastSource->totalMp)
                lastSource->currentMp = lastSource->totalMp;
        }
    }

    while (!m_actions.empty() && m_actions.back().source == lastSource)
        m_actions.pop_back();
}

void DungeonCrawl::NextHero()
{
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        m_heroes[index].isTurn = false;
        m_heroes[index].weapon1.selected = false;
        m_heroes[index].weapon2.selected = false;
        m_heroes[index].weapon3.selected = false;
        m_heroes[index].weapon4.selected = false;
    }

    do
    {
        m_heroIndex++;
        if (m_heroIndex == m_heroes.size())
        {
            SetState(State::STATE_COMBAT_RESOLVE);
            return;
        }
    } while (m_heroes[m_heroIndex].currentHp == 0);
    m_heroes[m_heroIndex].isTurn = true;

    PushCombat(&m_heroes[m_heroIndex]);
}

void DungeonCrawl::PrevHero()
{
    bool hasHeroes = false;
    int lowestIndex = 5; // Keep track of lowest available hero index
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        m_heroes[index].isTurn = false;
        m_heroes[index].weapon1.selected = false;
        m_heroes[index].weapon2.selected = false;
        m_heroes[index].weapon3.selected = false;
        m_heroes[index].weapon4.selected = false;
        if (m_heroes[index].currentHp > 0)
        {
            hasHeroes = true;
            lowestIndex = std::min<int>(lowestIndex, index);
        }
    }
    assert(hasHeroes == true && "All options unavailable");

    do
    {
        m_heroIndex--;
        if (m_heroIndex < 0)
        {
            m_heroIndex = lowestIndex;
            break;
        }
    } while (m_heroes[m_heroIndex].currentHp == 0);
    m_heroes[m_heroIndex].isTurn = true;

    PushCombat(&m_heroes[m_heroIndex]);
}

void DungeonCrawl::GetUniquePassive()
{
    while (true)
    {
        PassiveType type = PassiveType(GetRandomValue(0, (int)PassiveType::COUNT - 1));
        Passive selected = PASSIVE(type);

        // Do not show passives the player already owns
        if (OwnsPassive(type))
            continue;

        // Do not show passives that are the same within the list of options
        bool duplicate = false;
        for (auto passive : m_passiveOptions)
        {
            if (passive.x == selected.x
                && passive.y == selected.y
                && passive.tabIndex == selected.tabIndex)
            {
                duplicate = true;
                break;
            }
        }
        if (duplicate)
            continue;

        m_passiveOptions.push_back(selected);
        break;
    }
}

void DungeonCrawl::ReceiveXP(Hero& hero)
{
    // LEATHER_GOLD: each leather-wearer's payout per defeated monster scales
    // with both leather rarity and the monster's rarity, so tougher kills feel
    // meaningfully more rewarding.
    const bool leatherGold = OwnsPassive(PassiveType::LEATHER_GOLD)
        && hero.armor.target == Target::PLAYERAC_SPEED;

    // Boss fights award double hero level-up XP. Bosses are a much steeper fight, so
    // every monster's xp contribution is doubled before being subtracted from the
    // hero's xp-to-next-level pool.
    const int xpMultiplier = (m_currentRoom != nullptr && m_currentRoom->boss) ? 2 : 1;

    for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
    {
        Monster& monster = m_currentRoom->monsters[index];

        // Subtract xp from xp left to level
        hero.experience -= monster.experience * xpMultiplier;
        if (hero.experience <= 0)
            LevelUp(hero);

        if (leatherGold)
            m_gold += (int)hero.armor.rarity * (int)monster.rarity;
    }
}

void DungeonCrawl::LevelUp(Hero& hero)
{
    static int s_levels[20] = { 0, 10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240 };

    if (hero.level >= 20)
        return;

    hero.level++;
    hero.experience = s_levels[hero.level];

    int multLow = (hero.level / 3) + 1;
    int multHigh = (hero.level / 3) + 2;

    // Track this level's MP gain separately so we can do a partial-refill (rather than a full
    // refill) of current MP below. Otherwise MP weapons feel free because levels top off the pool.
    int mpGain = 0;

    if (hero.armor.target == Target::PLAYERAC_SLOW)
    {
        // If you equip PLATE you gain more HP during levels
        int hpGain = ROLL(multHigh, 10, hero.level + 1);
        if (OwnsPassive(PassiveType::PLATE_HPBOOST))
            hpGain *= 2; // PLATE_HPBOOST: doubles HP gained per level when wearing plate
        hero.totalHp += hpGain;
        // MP pool grows half as fast - mages who multiclass into plate shouldn't get a fat pool too.
        mpGain = Die(multLow, 10, hero.level + 1).Roll(nullptr) / 2;
        hero.totalMp += mpGain;
        // AC growth was the dominant late-game scaling. Was rarity + level/2 + 1 per level
        // (~180 at level 20 legendary). Now grows roughly half that, capped by rarity tier.
        hero.bonusAC += ((int)hero.armor.rarity + 1) / 2 + ((hero.level % 2) == 0 ? 1 : 0);
    }
    else if (hero.armor.target == Target::PLAYERAC_SPELL)
    {
        // If you equip ROBES you gain more MP during levels (still the mage class)
        hero.totalHp += ROLL(multLow, 10, hero.level + 1);
        // Halved so the player actually has to budget casts. Was ~65/level avg at level 20.
        mpGain = Die(multHigh, 10, hero.level + 1).Roll(nullptr) / 2;
        hero.totalMp += mpGain;
    }
    else if (hero.armor.target == Target::PLAYERAC_SPEED)
    {
        // If you equip LEATHER you lower the amount required for levels
        hero.experience /= 2;
        hero.totalHp += ROLL(multLow, 10, hero.level + 1);
        mpGain = Die(multLow, 10, hero.level + 1).Roll(nullptr) / 2;
        hero.totalMp += mpGain;
        // Was rarity per level (~60 at level 20 legendary). Now only counts higher rarities.
        hero.bonusAC += (int)hero.armor.rarity / 2;
    }
    else
    {
        // Equiping no ARMOR makes you gain little HP / MP for levels
        hero.totalHp += ROLL(multLow, 10, hero.level + 1);
        mpGain = Die(multLow, 10, hero.level + 1).Roll(nullptr) / 2;
        hero.totalMp += mpGain;
    }

    hero.levelUpTimeLeft = ToMilliseconds(1500);

    // HP still fully refills (standard RPG convention; not what the player complained about).
    // MP only gains back the rolled increase - no free refill - so mages have to budget casts.
    hero.currentHp = hero.totalHp;
    hero.currentMp += mpGain;
    if (hero.currentMp > hero.totalMp)
        hero.currentMp = hero.totalMp;
}

bool DungeonCrawl::OwnsPassive(PassiveType type)
{
    Passive selected = PASSIVE(type);
    for (auto passive : m_passives)
    {
        if (passive.x == selected.x
            && passive.y == selected.y
            && passive.tabIndex == selected.tabIndex)
        {
            return true;
        }
    }
    return false;
}

bool DungeonCrawl::EquippingWeapon(Actor* actor, WeaponType type, int count)
{
    int value = 0;
    if (actor->armor.weaponType == type)
        value++;
    if (actor->weapon1.weaponType == type)
        value++;
    if (actor->weapon2.weaponType == type)
        value++;
    if (actor->weapon3.weaponType == type)
        value++;
    if (actor->weapon4.weaponType == type)
        value++;
    return value >= count;
}

Die DungeonCrawl::CalculateDamageDie(Actor* actor, Weapon* weapon, bool baseDamage)
{
    // Invalid weapon
    if (!weapon)
        return Die(0, 0, 0);

    // Get the base damage die
    Die damageDie = weapon->die;
    if (baseDamage)
        return damageDie;

    // No damage is applied to conditions currently
    if (!actor)
        return damageDie;

    // Monsters don't have damage modifiers
    if (actor->GetType() == ActorType::ACTOR_MONSTER)
        return damageDie;

    // Add level bonus to damage die. Steeper than before (mult /3 -> /2, constant *1 -> *1.2)
    // so player DPS keeps pace with the monster scaling curve through mid-game.
    damageDie.multiplier += actor->level / 2;
    damageDie.constant   += (actor->level * 6) / 5; // integer-math version of level * 1.2

    // Apply passive bonuses
    if (OwnsPassive(PassiveType::GREATSWORD_HEAVYSWING))
    {
        if (EquippingWeapon(actor, WeaponType::GREATSWORD, 1)
            && weapon->weaponType == WeaponType::GREATSWORD)
        {
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
        }
    }
    if (OwnsPassive(PassiveType::GREATSWORD_ALONE))
    {
        if (EquippingWeapon(actor, WeaponType::GREATSWORD, 1) && EquippingWeapon(actor, WeaponType::INVALID, 1)
            && weapon->weaponType == WeaponType::GREATSWORD)
        {
            damageDie.multiplier *= 2;
        }
    }
    if (OwnsPassive(PassiveType::LEATHER_DAMAGE))
    {
        // "Daggers and Swords deal X extra multi. X is Level times Rarity."
        bool hasLeather = EquippingWeapon(actor, WeaponType::LEATHER, 1);
        if (hasLeather && EquippingWeapon(actor, WeaponType::DAGGER, 2))
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
        else if (hasLeather && EquippingWeapon(actor, WeaponType::SWORD, 2))
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
        else if (hasLeather && EquippingWeapon(actor, WeaponType::DAGGER, 1) && EquippingWeapon(actor, WeaponType::SWORD, 1))
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
    }
    if (OwnsPassive(PassiveType::ROBE_GLASSCANNON))
    {
        if ((EquippingWeapon(actor, WeaponType::WAND, 1) || EquippingWeapon(actor, WeaponType::STAFF, 1))
            && (weapon->weaponType == WeaponType::WAND || weapon->weaponType == WeaponType::STAFF))
        {
            damageDie.multiplier *= 2;
            damageDie.constant *= 2;
        }
    }
    if (OwnsPassive(PassiveType::SWORD_BATTLEWIZARD))
    {
        if ((EquippingWeapon(actor, WeaponType::SWORD, 1) || EquippingWeapon(actor, WeaponType::STAFF, 1))
            && (weapon->weaponType == WeaponType::SWORD || weapon->weaponType == WeaponType::STAFF))
        {
            damageDie.multiplier *= 2;
            damageDie.constant *= 2;
        }
    }
    if (OwnsPassive(PassiveType::WAND_ELEMENTALMASTER))
    {
        bool hasRobe = EquippingWeapon(actor, WeaponType::ROBE, 1);
        if (hasRobe && EquippingWeapon(actor, WeaponType::WAND, 1) && weapon->weaponType == WeaponType::WAND)
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
        else if (hasRobe && EquippingWeapon(actor, WeaponType::STAFF, 1) && weapon->weaponType == WeaponType::STAFF)
            damageDie.multiplier += (actor->level * (int)weapon->rarity);
    }
    if (OwnsPassive(PassiveType::GREATSWORD_HURT))
    {
        if (weapon->weaponType == WeaponType::GREATSWORD)
        {
            damageDie.multiplier += actor->damageTaken;
        }
    }
    if (OwnsPassive(PassiveType::STAFF_ELEMENTALMASTER))
    {
        if (weapon->weaponType == WeaponType::STAFF)
        {
            damageDie.multiplier *= 2;
        }
    }

    return damageDie;
}

void DungeonCrawl::UseWeapon(Action action)
{
    m_damageAttribute = 0x0004; // Red

    // Roll the damage to heal or deal
    DamageType damageType;
    Die damageDie = CalculateDamageDie(action.source, action.weapon, false);
    int damage = damageDie.Roll(&damageType);

    // Boss-dragon StrongerEachTurn modifier: outgoing damage scales +25% per round survived.
    if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
    {
        const Monster* boss = static_cast<const Monster*>(action.source);
        if (std::find(boss->modifiers.begin(), boss->modifiers.end(), BossModifier::StrongerEachTurn) != boss->modifiers.end())
        {
            damage = damage + (damage * boss->roundsAlive) / 4;  // *(1 + 0.25 * roundsAlive)
        }
    }

    // Keep track of damage before modifiers
    m_damageOriginal = damage;
    m_damageAttribute2 = ToAttribute(damageType);

    bool isHealing = action.weapon->target == Target::ALLPLAYERSHP
        || action.weapon->target == Target::PLAYERHP_CONSUME
        || action.weapon->target == Target::PLAYERHP_REUSE
        || action.weapon->target == Target::PLAYERHP_REVIVE_CONSUME;
    bool isRestoring = action.weapon->target == Target::ALLPLAYERSMP
        || action.weapon->target == Target::PLAYERMP_CONSUME
        || action.weapon->target == Target::PLAYERMP_REUSE;
    bool isDamaging = action.weapon->target == Target::ALLENEMIES
        || action.weapon->target == Target::ENEMY
        || action.weapon->target == Target::MONSTER_ALLENEMIES
        || !action.source;
    bool isDraining = action.weapon->target == Target::MONSTER_DRAINMP;

    bool consumesWeapon = action.weapon->target == Target::PLAYERMP_CONSUME
        || action.weapon->target == Target::PLAYERHP_CONSUME
        || action.weapon->target == Target::PLAYERHP_REVIVE_CONSUME;

    // Disable the animations
    bool isWeakness = false;
    if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
    {
        Hero* hero = static_cast<Hero*>(action.source);
        hero->isTurn = false;
    }
    if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
    {
        Monster* monster = static_cast<Monster*>(action.source);
        monster->attacking1 = false;
        monster->attacking2 = false;
    }

    // Switch positions
    //if (action.weapon->target == Target::MONSTER_SWITCH)
    //{
    //	if (m_currentRoom->monsters.size() != 4)
    //		m_currentRoom->monsters.push_back(Monster());
    //
    //	int index1 = GetRandomValue(0, m_currentRoom->monsters.size() - 1);
    //	int index2 = GetRandomValue(0, m_currentRoom->monsters.size() - 1);
    //	std::swap(m_currentRoom->monsters[index1], m_currentRoom->monsters[index2]);
    //	return;
    //}

    if (action.weapon->target == Target::MONSTER_RECRUIT)
        //|| action.weapon->target == Target::MONSTER_SWITCH)
    {
        Monster* monster = static_cast<Monster*>(action.source);
        for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
        {
            // Resurrect all dead common monsters to 10% HP
            if (m_currentRoom->monsters[index].currentHp == 0
                && m_currentRoom->monsters[index].rarity == Rarity::COMMON)
            {
                m_currentRoom->monsters[index].currentHp = (int)(m_currentRoom->monsters[index].totalHp * 0.1) + 1;
                m_currentRoom->monsters[index].spawning = ToMilliseconds(1600);
            }
        }
        return;
    }

    // Iterate over the targets and apply the result
    for (int index = 0; index < (int)action.targets.size(); index++)
    {
        damage = m_damageOriginal;

        Actor* actor = action.targets[index];

        // Family-slayer passives: hero attacks gain +1 Multi (one extra die roll) against the
        // matching family. Applied per-target so multi-target spells still get the bonus only
        // on the relevant targets in the splash. Scales through downstream weakness/resistance
        // like the rest of base damage.
        if (isDamaging && action.source && action.source->GetType() == ActorType::ACTOR_HERO
            && actor && actor->GetType() == ActorType::ACTOR_MONSTER
            && action.weapon && action.weapon->die.die > 0)
        {
            const Monster* mon = static_cast<const Monster*>(actor);
            if (mon->family == MonsterFamily::UNDEAD && OwnsPassive(PassiveType::SKELETON_SLAYER))
                damage += GetRandomValue(1, action.weapon->die.die);
            if (mon->family == MonsterFamily::GELATINOUS && OwnsPassive(PassiveType::BLOB_SLAYER))
                damage += GetRandomValue(1, action.weapon->die.die);
            if (mon->family == MonsterFamily::RODENT && OwnsPassive(PassiveType::BAT_SLAYER))
                damage += GetRandomValue(1, action.weapon->die.die);
            if (mon->family == MonsterFamily::ARACHNID && OwnsPassive(PassiveType::SPIDER_SLAYER))
                damage += GetRandomValue(1, action.weapon->die.die);
        }

        // Handle special actions
        if (action.weapon->target == Target::MONSTER_CONDITION)
        {
            if (action.weapon->die.type != DamageType::NORMAL)
            {
                Hero* hero = static_cast<Hero*>(actor);
                hero->condition = action.weapon->die;
                hero->conditionTurnsLeft = GetRandomValue(1, 3);
            }
            continue;
        }

        // Handle protection spell
        if (action.weapon->target == Target::PLAYER_PROTECT
            || action.weapon->target == Target::PLAYER_PROTECTALL)
        {
            if (actor->currentHp > 0)
            {
                actor->protect = true;
            }
            continue;
        }

        // Reduce or increase damage if the target's element makes it resistant or weak.
        //
        // Semantics:
        //   actor->armor.resistances[] holds the target's own elemental type(s).
        //   ToStrength(damageType) = the element the damage is STRONG against
        //     -> if the target IS that element, the damage hits it for double  (weakness).
        //   ToWeakness(damageType) = the element the damage is WEAK against
        //     -> if the target IS that element, the damage is halved          (resistance).
        //
        // (Prior to this fix the two assignments were swapped, doubling damage on resists and
        //  halving damage on weaknesses.)
        if (isDamaging || isDraining)
        {
            bool ignoreResistance = false;
            bool hasWeakness   = false;
            bool hasResistance = false;
            for (int res = 0; res < (int)actor->armor.resistances.size(); res++)
            {
                if (actor->armor.resistances[res] == ToStrength(damageType))
                {
                    hasWeakness = true;
                    break;
                }
                if (actor->armor.resistances[res] == ToWeakness(damageType))
                {
                    hasResistance = true;
                    break;
                }
            }
            if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
            {
                if (OwnsPassive(PassiveType::STAFF_IGNOREELEMENT) && action.weapon->weaponType == WeaponType::STAFF)
                {
                    ignoreResistance = true;
                }
            }

            if (hasResistance && !ignoreResistance)
                damage = int(damage / 2);
            if (hasWeakness)
                damage = int(damage * 2);

            // ROBE_GLASSCANNON: wearer of robe + wand/staff takes doubled damage
            // (paired tradeoff to the doubled magical damage that the same passive grants)
            if (OwnsPassive(PassiveType::ROBE_GLASSCANNON)
                && actor->GetType() == ActorType::ACTOR_HERO
                && actor->armor.target == Target::PLAYERAC_SPELL
                && (EquippingWeapon(actor, WeaponType::WAND, 1) || EquippingWeapon(actor, WeaponType::STAFF, 1)))
            {
                damage = int(damage * 2);
            }

            // Boss-dragon resistance modifiers: halve damage for matching weapon classes.
            // (The full-immunity modifiers - FirstTurnImmune and ElementImmune - are applied
            //  *after* the min-1 damage clamp further down, otherwise the clamp would push 0
            //  damage back up to 1 and the dragon would take chip damage on round 1.)
            if (actor->GetType() == ActorType::ACTOR_MONSTER && !static_cast<Monster*>(actor)->modifiers.empty())
            {
                const Monster* boss = static_cast<Monster*>(actor);
                auto hasMod = [&](BossModifier m)
                {
                    return std::find(boss->modifiers.begin(), boss->modifiers.end(), m) != boss->modifiers.end();
                };

                if (hasMod(BossModifier::ResistMelee) && action.weapon)
                {
                    const WeaponType wt = action.weapon->weaponType;
                    if (wt == WeaponType::SWORD || wt == WeaponType::DAGGER
                        || wt == WeaponType::GLOVES || wt == WeaponType::GREATSWORD)
                    {
                        damage = damage / 2;
                    }
                }
                if (hasMod(BossModifier::ResistMagic) && action.weapon)
                {
                    const WeaponType wt = action.weapon->weaponType;
                    if (wt == WeaponType::WAND || wt == WeaponType::STAFF)
                        damage = damage / 2;
                }
            }

            // Subtract armor class
            damage -= actor->armor.armorClass;

            // Subtract level
            if (actor->GetType() == ActorType::ACTOR_HERO)
            {
                Hero* hero = static_cast<Hero*>(actor);
                damage -= hero->level;
                damage -= hero->bonusAC;
            }

            // STAFF_MAGICSHIELD: robe + staff absorbs incoming damage by a staff roll
            if (actor->GetType() == ActorType::ACTOR_HERO
                && action.source && action.source->GetType() == ActorType::ACTOR_MONSTER
                && OwnsPassive(PassiveType::STAFF_MAGICSHIELD)
                && actor->armor.target == Target::PLAYERAC_SPELL)
            {
                Weapon* staff = nullptr;
                if (actor->weapon1.weaponType == WeaponType::STAFF)
                    staff = &actor->weapon1;
                else if (actor->weapon2.weaponType == WeaponType::STAFF)
                    staff = &actor->weapon2;
                else if (actor->weapon3.weaponType == WeaponType::STAFF)
                    staff = &actor->weapon3;
                else if (actor->weapon4.weaponType == WeaponType::STAFF)
                    staff = &actor->weapon4;

                if (staff)
                {
                    Die shieldDie = CalculateDamageDie(actor, staff, false);
                    damage -= shieldDie.Roll();
                }
            }

            if (damage <= 0)
                damage = 1;

            // Remove protection spell and reduce damage to 0
            if (actor->protect)
            {
                actor->protect = false;
                damage = 0;
            }

            // Boss-dragon full immunities applied AFTER the min-1 clamp so they actually zero
            // out the hit (Phasing/FirstTurnImmune on round 1, ElementImmune on matching element).
            if (actor->GetType() == ActorType::ACTOR_MONSTER && !static_cast<Monster*>(actor)->modifiers.empty())
            {
                const Monster* boss = static_cast<Monster*>(actor);
                auto hasMod = [&](BossModifier m)
                {
                    return std::find(boss->modifiers.begin(), boss->modifiers.end(), m) != boss->modifiers.end();
                };
                if (hasMod(BossModifier::FirstTurnImmune) && boss->firstTurnImmune)
                    damage = 0;
                if (hasMod(BossModifier::ElementImmune) && damageType == boss->immuneElement)
                    damage = 0;
            }
        }

        // The final damage
        m_damageFinal = damage;

        if (isRestoring || isHealing)
        {
            damage = damage * -1;
            m_damageAttribute = 0x0002; // Green
        }
        if (isDraining)
        {
            m_damageAttribute = 0x0001; // Blue
        }

        bool parry = false;
        bool reposte = false;
        bool ignored = false;
        if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
        {
            if (OwnsPassive(PassiveType::SWORD_PARRY) && EquippingWeapon(actor, WeaponType::SWORD, 1))
            {
                if (GetRandomValue(0, 3) == 0)
                    parry = true;
            }
            if (OwnsPassive(PassiveType::SWORD_RIPOSTE) && EquippingWeapon(actor, WeaponType::SWORD, 1))
            {
                if (GetRandomValue(0, 3) == 0)
                    reposte = true;
            }

            if (OwnsPassive(PassiveType::GREATSWORD_HURT) && EquippingWeapon(actor, WeaponType::GREATSWORD, 1))
            {
                actor->damageTaken++;
            }

            // PLATE_IGNOREDAMAGE: plate wearers have a 25% chance to ignore a hit
            if (OwnsPassive(PassiveType::PLATE_IGNOREDAMAGE)
                && actor->GetType() == ActorType::ACTOR_HERO
                && actor->armor.target == Target::PLAYERAC_SLOW)
            {
                if (GetRandomValue(0, 3) == 0)
                    ignored = true;
            }
        }

        // Parry or ignore negates the hit; reflect that in the damage popup
        if (parry || ignored)
        {
            m_damageFinal = 0;
            m_damageOriginal = 0;
        }

        // Apply damage/healing to actor
        if (isRestoring || isDraining)
        {
            actor->currentMp -= damage;
            if (actor->currentMp < 0)
                actor->currentMp = 0;
            if (actor->currentMp > actor->totalMp)
                actor->currentMp = actor->totalMp;
        }
        else
        {
            if (!parry && !ignored)
                actor->currentHp -= damage;
            if (actor->currentHp < 0)
                actor->currentHp = 0;
            if (actor->currentHp > actor->totalHp)
                actor->currentHp = actor->totalHp;

            // Boss-dragon DotAttacks modifier: every hit on a hero adds a damage-over-time
            // condition tied to the floor's element type. The condition naturally decays as
            // existing condition turns count down.
            if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER
                && actor->GetType() == ActorType::ACTOR_HERO
                && !parry && !ignored)
            {
                const Monster* boss = static_cast<const Monster*>(action.source);
                if (std::find(boss->modifiers.begin(), boss->modifiers.end(), BossModifier::DotAttacks) != boss->modifiers.end())
                {
                    Hero* hero = static_cast<Hero*>(actor);
                    DamageType dotType = (boss->element != DamageType::NORMAL) ? boss->element : DamageType::POISON;
                    hero->condition = Die(1, 4, m_floor / 3, dotType);
                    hero->conditionTurnsLeft = std::max<int>(hero->conditionTurnsLeft, 3);
                }
            }

            if (reposte)
            {
                // Find an equipped sword and counter-attack with its rolled damage
                Weapon* sword = nullptr;
                if (actor->weapon1.weaponType == WeaponType::SWORD)
                    sword = &actor->weapon1;
                else if (actor->weapon2.weaponType == WeaponType::SWORD)
                    sword = &actor->weapon2;
                else if (actor->weapon3.weaponType == WeaponType::SWORD)
                    sword = &actor->weapon3;
                else if (actor->weapon4.weaponType == WeaponType::SWORD)
                    sword = &actor->weapon4;

                if (sword)
                {
                    Die riposteDie = CalculateDamageDie(actor, sword, false);
                    int riposteDamage = riposteDie.Roll();
                    action.source->currentHp -= riposteDamage;
                    if (action.source->currentHp < 0)
                        action.source->currentHp = 0;
                    if (action.source->currentHp > action.source->totalHp)
                        action.source->currentHp = action.source->totalHp;
                }
            }
        }

        if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
        {
            Monster* monster = static_cast<Monster*>(actor);

            // Run-stat tracking: highest single-hit damage and per-rarity kill counter.
            if (m_damageFinal > m_currentRun.maxDamage)
                m_currentRun.maxDamage = m_damageFinal;
            if (monster->currentHp == 0)
            {
                switch (monster->rarity)
                {
                    case Rarity::COMMON:    m_currentRun.killsCommon++;    break;
                    case Rarity::RARE:      m_currentRun.killsRare++;      break;
                    case Rarity::EPIC:      m_currentRun.killsEpic++;      break;
                    case Rarity::LEGENDARY: m_currentRun.killsLegendary++; break;
                    case Rarity::ARTIFACT:  m_currentRun.killsArtifact++;  break;
                default: break;
                }
            }

            if (GetRandomValue(0, 3) == 0)
            {
                if (OwnsPassive(PassiveType::GLOVES_BASH) && action.weapon->weaponType == WeaponType::GLOVES && action.firstSwing)
                {
                    monster->stunned = true;
                }
                if (OwnsPassive(PassiveType::DAGGER_BLEED) && action.weapon->weaponType == WeaponType::DAGGER && action.firstSwing)
                {
                    monster->condition = damageDie;
                }
                if (OwnsPassive(PassiveType::GLOVES_PICKPOCKET)
                    && action.weapon->weaponType == WeaponType::GLOVES
                    && EquippingWeapon(action.source, WeaponType::LEATHER, 1))
                {
                    m_gold += ((int)monster->rarity + (int)action.weapon->rarity - 1);
                }
            }

            // Boss-dragon RetaliateMelee modifier: any melee hit on the dragon (sword, dagger,
            // glove, greatsword) is countered immediately for 1/10 of the dragon's primary
            // weapon damage. Fires every melee swing, not gated by the 25% on-hit roll above.
            if (action.weapon)
            {
                const WeaponType wt = action.weapon->weaponType;
                const bool isMelee = (wt == WeaponType::SWORD || wt == WeaponType::DAGGER
                    || wt == WeaponType::GLOVES || wt == WeaponType::GREATSWORD);
                if (isMelee
                    && std::find(monster->modifiers.begin(), monster->modifiers.end(),
                        BossModifier::RetaliateMelee) != monster->modifiers.end())
                {
                    int retaliate = monster->weapon1.die.Roll() / 10;
                    if (retaliate < 1) retaliate = 1;
                    action.source->currentHp -= retaliate;
                    if (action.source->currentHp < 0)
                        action.source->currentHp = 0;
                }
            }
        }
    }

    if (consumesWeapon)
    {
        // Consume the potion
        *action.weapon = WEAPON("Unarmed");
    }
}

void DungeonCrawl::ClampCursor(int maxIndex)
{
    //if (m_cursorIndex < 0)
    //	m_cursorIndex = 0;
    //if (m_cursorIndex > maxIndex)
    //	m_cursorIndex = maxIndex;
}

void DungeonCrawl::AddFairy()
{
    Fairy fairy;
    fairy.fairy = ANIMATION("fairy");
    fairy.fairy.SetStrobe(1, true);
    fairy.dust = ANIMATION("fairy_dust");
    fairy.dust.SetStrobe(0, true);
    fairy.dir = GetRandomValue(0, 1);
    fairy.timeLeft = ToMilliseconds(GetRandomValue(50, 100));
    fairy.x = GetRandomValue(5, 93);
    fairy.y = GetRandomValue(3, 13);
    fairy.x_dust = fairy.x;
    fairy.y_dust = fairy.y;

    m_fairies.push_back(fairy);
}

void DungeonCrawl::ApplyCondition()
{
    {
        // Deal condition damage to monsters
        m_mcondition1.die = Die(0, 0, 0, DamageType::NORMAL);
        m_mcondition2.die = Die(0, 0, 0, DamageType::NORMAL);
        m_mcondition3.die = Die(0, 0, 0, DamageType::NORMAL);
        m_mcondition4.die = Die(0, 0, 0, DamageType::NORMAL);
        for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
        {
            Weapon* condition = nullptr;
            if (index == 0)
            {
                m_mcondition1.die = m_currentRoom->monsters[index].condition;
                condition = &m_mcondition1;
            }
            if (index == 1)
            {
                m_mcondition2.die = m_currentRoom->monsters[index].condition;
                condition = &m_mcondition2;
            }
            if (index == 2)
            {
                m_mcondition3.die = m_currentRoom->monsters[index].condition;
                condition = &m_mcondition3;
            }
            if (index == 3)
            {
                m_mcondition4.die = m_currentRoom->monsters[index].condition;
                condition = &m_mcondition4;
            }
            if (condition->die.die == 0 && condition->die.constant == 0)
                continue;

            Action action;
            action.source = nullptr;
            action.targets.push_back(&m_currentRoom->monsters[index]);
            action.weapon = condition;
            m_actions.push_back(action);
        }
    }

    {
        // Deal condition damage to heroes
        m_condition1.die = Die(0, 0, 0, DamageType::NORMAL);
        m_condition2.die = Die(0, 0, 0, DamageType::NORMAL);
        m_condition3.die = Die(0, 0, 0, DamageType::NORMAL);
        m_condition4.die = Die(0, 0, 0, DamageType::NORMAL);
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            // Condition slowly goes away
            if (m_heroes[index].conditionTurnsLeft != 0)
                m_heroes[index].conditionTurnsLeft--;
            if (m_heroes[index].conditionTurnsLeft == 0)
            {
                m_heroes[index].condition = Die(0, 0, 0, DamageType::NORMAL);
                continue;
            }

            Weapon* condition = nullptr;
            if (index == 0)
            {
                m_condition1.die = m_heroes[index].condition;
                condition = &m_condition1;
            }
            if (index == 1)
            {
                m_condition2.die = m_heroes[index].condition;
                condition = &m_condition2;
            }
            if (index == 2)
            {
                m_condition3.die = m_heroes[index].condition;
                condition = &m_condition3;
            }
            if (index == 3)
            {
                m_condition4.die = m_heroes[index].condition;
                condition = &m_condition4;
            }

            Action action;
            action.source = nullptr;
            action.targets.push_back(&m_heroes[index]);
            action.weapon = condition;
            m_actions.push_back(action);

            //// Apply condition damage / healing
            //if (m_heroes[index].condition.die != 0)
            //{
            //	int damage = m_heroes[index].condition.Roll();
            //	if (m_heroes[index].condition.type == DamageType::HEALING)
            //		damage *= -1;
            //	m_heroes[index].currentHp -= damage;
            //	if (m_heroes[index].currentHp > m_heroes[index].totalHp)
            //		m_heroes[index].currentHp = m_heroes[index].totalHp;
            //	if (m_heroes[index].currentHp < 0)
            //		m_heroes[index].currentHp = 0;
            //}
        }
    }
}

void DungeonCrawl::SetState(State state)
{
    m_showAttributes = false;

    m_ui.Clear();
    CursorContext root;
    root.cursor = ANIMATION("select_hero");
    m_ui.PushBack(root);

    if (state == State::STATE_MAIN)
    {
        m_heroes.clear();
        m_floor = 0;
        m_gold = GetRandomValue(250, 300);
        m_input.Initialize();
        m_dungeonEx.Initialize();
        m_db = m_dungeonEx.GetDatabase();
        //TestDungeon();

        AddHero();
        m_initFloor.rooms.clear();

        std::vector<std::vector<Weapon>> weaponTable = { m_db.m_commonTable };
        std::vector<uint16_t> hiltColor = { 0x0006, 0x0008, 0x000E, 0x000F };

        Room room;

        // Roll 4 common weapons
        int count = 4;
        while (count-- > 0)
        {
            std::vector<Weapon> table = ROLLTABLE(weaponTable);
            Weapon weapon = ROLLTABLE(table);
            weapon.idle.SetAttributes(1, ROLLTABLE(hiltColor));
            weapon.Randomize();

            if (weapon.name.empty())
            {
                count++;
                continue;
            }

            room.shop.push_back(weapon);
        }

        m_initFloor.rooms.push_back(room);
        //m_currentFloor = &m_initFloor;
        m_currentFloor = std::move(m_initFloor);
        m_currentFloorPtr = &m_initFloor;
        m_currentRoom = &m_currentFloor.rooms[0];

        m_passives.clear();
        m_passivesCursor = ANIMATION("select_weapon");
        m_passivesX = 0;
        m_passivesY = 0;
        m_passivesTab = false;
        m_passivesTabIndex = 0;
        m_passiveXP = 5; // First combat will award a passive trait.

        // Reset live run-stat tracking for the new game.
        m_currentRun = RunStats();
        m_initials[0] = m_initials[1] = m_initials[2] = 'A';
        m_initialsPosition = 0;
        m_lastRank = 0;
    }
    else if (state == State::STATE_NEXT_FLOOR)
    {
        m_stairs = ANIMATION("stairs");
        m_timeLeft = ToMilliseconds(2000);
        m_floor++;
        m_currentRun.floor = m_floor;
        //m_currentFloor = &m_dungeon.GetFloor(m_floor);
        m_dungeonEx.GetNextFloor(std::move(m_currentFloor));
        m_currentFloorPtr = &m_currentFloor;

        // ARTIFACT_HUNTER passive: any fountain on this floor is rebranded as an Artifact door
        // and seeded with a random Artifact item. Fountains naturally appear ~once per 30 floors
        // (per the RollState table) so this hits that "1 every 30 floors" cadence automatically.
        // Gated to floor 20+ so the artifact tier stays a true late-game milestone (matches the
        // floor-20 wall where monster scaling first ramps hard).
        // FAIRY_FRIEND passive: bumps the fountain rate by +2% per door. Applied BEFORE the
        // artifact upgrade below so a freshly-converted fountain can also become an Artifact
        // door if the player also owns ARTIFACT_HUNTER.
        if (OwnsPassive(PassiveType::FAIRY_FRIEND))
        {
            m_dungeonEx.BoostFountainRate(m_currentFloor, 2);
        }

        // CONNOISSEUR passive: slight bump to shop rate (+5% per door) AND a per-item
        // chance (20%) to bump each shop weapon up one rarity tier. Run after the fountain
        // boost so the rooms FAIRY_FRIEND already claimed stay fountains.
        if (OwnsPassive(PassiveType::CONNOISSEUR))
        {
            m_dungeonEx.BoostShopRate(m_currentFloor, 5);
            m_dungeonEx.ImproveShopRarity(m_currentFloor, 20);
        }

        if (m_floor >= 25 && OwnsPassive(PassiveType::ARTIFACT_HUNTER))
        {
            m_dungeonEx.UpgradeFountainsToArtifact(m_currentFloor);
        }

        // MP regen now only happens at attribute-change boundaries (every 5 floors) instead of
        // every floor. Previous 25%/50% per-floor regen meant the pool effectively topped off
        // constantly and MP weapons felt free. Now it's a small refuel between elemental arcs -
        // mages still need to budget casts across 5 floors and can't rely on it alone.
        if ((m_floor % 5) == 0)
        {
            // ARCANE_BATTERY: per-hero bonus regen of +5% max MP per 5 levels, owned party-wide
            // but scaling off each hero's individual level (caps at +20% at level 20).
            const bool hasBattery = OwnsPassive(PassiveType::ARCANE_BATTERY);
            for (int index = 0; index < (int)m_heroes.size(); index++)
            {
                auto& hero = m_heroes[index];
                if (hero.currentHp == 0)
                    continue;
                int regen = hero.totalMp / 10;                               // 10% baseline (was 25%)
                if (hero.armor.target == Target::PLAYERAC_SPELL)
                    regen = hero.totalMp / 5;                                // 20% for robe wearers (was 50%)
                if (hasBattery)
                {
                    const int tiers = hero.level / 5;                        // 0..4 (capped at 4 since hero.level <= 20)
                    regen += (hero.totalMp * 5 * tiers) / 100;               // +5% per tier
                }
                hero.currentMp += regen;
                if (hero.currentMp > hero.totalMp)
                    hero.currentMp = hero.totalMp;
            }
        }

        // No longer display any passives as new
        //for (auto&& passive : m_passives)
        //{
        //    passive.bNew = false;
        //}
    }
    else if (state == State::STATE_DOORS)
    {
        PushDoors();
        m_currentRoom = nullptr;
    }
    else if (state == State::STATE_SHOP)
    {
        m_currentRun.shopsFound++;

        // At max heroes, "New Hero" items are converted to "Level Up 5" items
        if (m_heroes.size() == 4)
        {
            for (int index = 0; index < (int)m_currentRoom->shop.size(); index++)
            {
                if (m_currentRoom->shop[index].target == Target::NEWHERO)
                {
                    m_currentRoom->shop[index] = WEAPON("Level Up 5");
                    m_currentRoom->shop[index].Randomize(); // Give the item a price
                    m_currentRoom->shop[index].attack.SetStrobe(0, true);
                    m_currentRoom->shop[index].idle.SetStrobe(3, true);
                }
            }
        }

        // HAGGLER passive: 10% off every shop item. Applied once at shop entry (mutating the
        // stored gold field) so the discount flows through display, hover panels, and the
        // purchase-affordability check without extra special-casing.
        if (OwnsPassive(PassiveType::HAGGLER))
        {
            for (int index = 0; index < (int)m_currentRoom->shop.size(); index++)
            {
                int& g = m_currentRoom->shop[index].gold;
                if (g > 0)
                    g = (g * 9) / 10;
            }
        }

        PushShop();
    }
    else if (state == State::STATE_PASSIVE)
    {
        m_passiveOptions.clear();
        int numPassives = 3;
        while (numPassives-- > 0)
        {
            GetUniquePassive();
        }

        // Make the footer Passive XP icons trail/bob while the player is on the selection
        // screen, visually connecting them to the passive screen the player is now seeing.
        IMAGE("passive_xp").SetTrailing(true);

        PushPassive();
    }
    else if (state == State::STATE_COMBAT)
    {
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].weapon1.selected = false;
            m_heroes[index].weapon2.selected = false;
            m_heroes[index].weapon3.selected = false;
            m_heroes[index].weapon4.selected = false;
            m_heroes[index].damageTaken = 0;
        }

        //ApplyCondition();

        //for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
        //{
        //	if (index != 0)
        //		m_currentRoom->monsters[index].currentHp = 0;
        //	if (index == 0)
        //		m_currentRoom->monsters[index].currentHp = 1;
        //}

        m_heroIndex = -1; // Next hero will assign this to 0
        NextHero(); // Pushes combat selection
    }
    else if (state == State::STATE_COMBAT_RESOLVE)
    {
        PushMonsterActions();
        SortActions();
        ApplyCondition();
    }
    else if (state == State::STATE_TREASURE)
    {
        // Remove lingering conditions
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].condition = Die(0, 0, 0, DamageType::NORMAL);
            m_heroes[index].conditionTurnsLeft = 0;
        }

        m_chestClosed = true;
        m_chest = ANIMATION("chest_closed");
        static uint16_t s_chestAttributes[14] = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F, };
        m_chestAttribute = s_chestAttributes[GetRandomValue(0, 13)];

        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0)
                ReceiveXP(m_heroes[index]);
        }

        PushTreasure();
    }
    else if (state == State::STATE_FOUNTAIN)
    {
        m_fairies.clear();
        int fairies = GetRandomValue(5, 8);
        while (fairies-- > 0)
        {
            AddFairy();
        }

        m_fountain = ANIMATION("fountain");

        // Heal all heroes to max and revive dead heroes.
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].currentHp = m_heroes[index].totalHp;
            m_heroes[index].currentMp = m_heroes[index].totalMp;
            m_heroes[index].protect = true;
        }

        // FAIRY_FRIEND passive: finding a fountain awards +4 Passive XP. Accumulates silently
        // so the player gets a passive selection after their next combat (no mid-fountain state
        // transition needed).
        if (OwnsPassive(PassiveType::FAIRY_FRIEND))
        {
            m_passiveXP += 4;
        }

        PushFountain();
    }
    else if (state == State::STATE_TRAP)
    {
        m_currentRun.trapsFound++;
        m_trapInitiated = false;
        m_trapTriggered = false;
        PushTrap();
    }
    else if (state == State::STATE_HIGHSCORE)
    {
        // Ignore input for a brief moment so the killing-blow Enter (or any mashed input from
        // combat) doesn't auto-submit the default "AAA" initials before the player even sees
        // the screen.
        m_highScoreInputCooldown = ToMilliseconds(500);
        PushHighScoreEntry();
    }

    UpdateTiles();

    m_state = state;
}

void DungeonCrawl::UpdateTiles()
{
    m_tiles.clear();
    m_tiles.push_back(ANIMATION("side_tile_0"));
    m_tiles.push_back(ANIMATION("side_tile_1"));
    m_tiles.push_back(ANIMATION("side_tile_2"));

    bool direction = false;
    for (int index = 0; index < m_dungeonEx.GetSize(); index++)
    {
        Animation animation = direction ? ANIMATION("side_tile_3_left") : ANIMATION("side_tile_3_right");

        animation.SetAttributes(1, ToAttribute(m_dungeonEx.GetAttributes(index)));
        if (index > 4)
            animation.SetAttributes(0, 0x000C);
        if (index > 8)
            animation.SetAttributes(0, 0x000D);
        if (index > 12)
            animation.SetAttributes(0, 0x000E);
        if (index > 16)
            animation.SetAttributes(0, 0x000F);

        direction = !direction;
        m_tiles.push_back(animation);
    }
}

void DungeonCrawl::PushMonsterActions()
{
    for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
    {
        Monster& monster = m_currentRoom->monsters[index];
        if (monster.currentHp == 0)
            continue;

        CreateMonsterAction(monster);
    }
}

void DungeonCrawl::CreateMonsterAction(Monster& monster)
{
    // Bypass adding action if monster is stunned
    if (monster.stunned)
    {
        monster.stunned = false; // Stun only lasts for 1 turn
        return;
    }

    std::vector<Weapon*> weapons;
    if (monster.rarity >= Rarity::COMMON)
        weapons.push_back(&monster.weapon1);
    if (monster.rarity >= Rarity::RARE)
        weapons.push_back(&monster.weapon2);
    if (monster.rarity >= Rarity::EPIC)
        weapons.push_back(&monster.weapon3);
    if (monster.rarity >= Rarity::LEGENDARY)
        weapons.push_back(&monster.weapon4);

    // Roll random weapon based on rarity of monster
    Weapon* weapon = ROLLTABLE(weapons);

    // Boss-dragon AlwaysSecondary modifier: forces use of weapon2 (the multi-target attack).
    const bool hasAlwaysSecondary = std::find(monster.modifiers.begin(), monster.modifiers.end(),
        BossModifier::AlwaysSecondary) != monster.modifiers.end();
    if (hasAlwaysSecondary)
        weapon = &monster.weapon2;

    // Gather list of available targets
    std::vector<Actor*> targets;
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp > 0)
            targets.push_back(&m_heroes[index]);
    }

    // Select reduce the target list based on the weapon
    if (weapon->target == Target::ENEMY
        || weapon->target == Target::MONSTER_CONDITION
        || weapon->target == Target::MONSTER_DRAINMP)
    {
        // ROBE_BACKLINE: robe wearers are not targeted if a non-robe hero is alive
        if (OwnsPassive(PassiveType::ROBE_BACKLINE))
        {
            bool hasNonRobe = false;
            for (int index = 0; index < (int)m_heroes.size(); index++)
            {
                if (m_heroes[index].currentHp > 0
                    && m_heroes[index].armor.target != Target::PLAYERAC_SPELL)
                {
                    hasNonRobe = true;
                    break;
                }
            }
            if (hasNonRobe)
            {
                targets.erase(std::remove_if(targets.begin(), targets.end(),
                    [](Actor* a) { return a->armor.target == Target::PLAYERAC_SPELL; }),
                    targets.end());
            }
        }

        // If we are selecting 1 target, weight it by who has Plate (tank)
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0
                && m_heroes[index].armor.target == Target::PLAYERAC_SLOW)
            {
                // Higher quality armor have stronger weights
                if (m_heroes[index].armor.rarity >= Rarity::COMMON)
                    targets.push_back(&m_heroes[index]);
                if (m_heroes[index].armor.rarity >= Rarity::RARE)
                    targets.push_back(&m_heroes[index]);
                if (m_heroes[index].armor.rarity >= Rarity::EPIC)
                    targets.push_back(&m_heroes[index]);
                if (m_heroes[index].armor.rarity >= Rarity::LEGENDARY)
                    targets.push_back(&m_heroes[index]);
                if (m_heroes[index].armor.rarity >= Rarity::ARTIFACT)
                    targets.push_back(&m_heroes[index]);

                // PLATE_TAUNT: additional weighting on plate wearers (skipped if the attacking
                // monster has the boss-dragon TauntImmune modifier).
                const bool bossIsTauntImmune = std::find(monster.modifiers.begin(),
                    monster.modifiers.end(), BossModifier::TauntImmune) != monster.modifiers.end();
                if (OwnsPassive(PassiveType::PLATE_TAUNT) && !bossIsTauntImmune)
                {
                    if (m_heroes[index].armor.rarity >= Rarity::COMMON)
                        targets.push_back(&m_heroes[index]);
                    if (m_heroes[index].armor.rarity >= Rarity::RARE)
                        targets.push_back(&m_heroes[index]);
                    if (m_heroes[index].armor.rarity >= Rarity::EPIC)
                        targets.push_back(&m_heroes[index]);
                    if (m_heroes[index].armor.rarity >= Rarity::LEGENDARY)
                        targets.push_back(&m_heroes[index]);
                    if (m_heroes[index].armor.rarity >= Rarity::ARTIFACT)
                        targets.push_back(&m_heroes[index]);
                }
            }
        }

        Actor* target = ROLLTABLE(targets);
        targets.clear();
        targets.push_back(target);
    }

    // Create the action
    Action action;
    action.source = &monster;
    action.weapon = weapon;
    action.targets = targets;
    m_actions.push_back(action);
}

void DungeonCrawl::SortActions()
{
    std::sort(m_actions.begin(), m_actions.end(), [this](const Action& left, const Action& right)
    {
        int leftSpeed = left.weapon->speed;
        int rightSpeed = right.weapon->speed;

        // Apply armor speed
        if (left.source->armor.target == Target::PLAYERAC_SLOW)
            leftSpeed += left.source->armor.speed;
        if (left.source->armor.target == Target::PLAYERAC_SPEED)
            leftSpeed -= left.source->armor.speed;

        // Apply armor speed
        if (right.source->armor.target == Target::PLAYERAC_SLOW)
            rightSpeed += right.source->armor.speed;
        if (right.source->armor.target == Target::PLAYERAC_SPEED)
            rightSpeed -= right.source->armor.speed;

        const bool leftIsHero  = left.source  && left.source->GetType()  == ActorType::ACTOR_HERO;
        const bool rightIsHero = right.source && right.source->GetType() == ActorType::ACTOR_HERO;

        // WAND_QUICKSPELL: hero wand/staff swings cast faster
        if (OwnsPassive(PassiveType::WAND_QUICKSPELL))
        {
            if (leftIsHero && (left.weapon->weaponType == WeaponType::WAND || left.weapon->weaponType == WeaponType::STAFF))
                leftSpeed -= 2;
            if (rightIsHero && (right.weapon->weaponType == WeaponType::WAND || right.weapon->weaponType == WeaponType::STAFF))
                rightSpeed -= 2;
        }

        // STAFF_ELEMENTALMASTER: staffs hit harder but are slower
        if (OwnsPassive(PassiveType::STAFF_ELEMENTALMASTER))
        {
            if (leftIsHero && left.weapon->weaponType == WeaponType::STAFF)
                leftSpeed += 4;
            if (rightIsHero && right.weapon->weaponType == WeaponType::STAFF)
                rightSpeed += 4;
        }

        return leftSpeed < rightSpeed;
    });
}

int DungeonCrawl::GetEffectiveMpCost(Actor* actor, const Weapon* weapon)
{
    int cost = weapon->mpCost;

    // ROBE_MPCOST: cost reduced by robe rarity
    if (OwnsPassive(PassiveType::ROBE_MPCOST)
        && actor->armor.target == Target::PLAYERAC_SPELL)
    {
        cost -= (int)actor->armor.rarity;
    }

    // ROBE_GLASSCANNON: spells cost more (paired with the doubled-damage bonus)
    if (OwnsPassive(PassiveType::ROBE_GLASSCANNON)
        && actor->armor.target == Target::PLAYERAC_SPELL
        && (weapon->weaponType == WeaponType::WAND || weapon->weaponType == WeaponType::STAFF))
    {
        cost += 5;
    }

    // STAFF_ELEMENTALMASTER: staff casts cost more (paired with doubled multi)
    if (OwnsPassive(PassiveType::STAFF_ELEMENTALMASTER) && weapon->weaponType == WeaponType::STAFF)
    {
        cost += 5;
    }

    if (cost < 0)
        cost = 0;
    return cost;
}

void DungeonCrawl::UseSelectedItem()
{
    CursorContext& context = m_ui.GetContext();

    // Pay the cost
    context.source->currentMp -= GetEffectiveMpCost(context.source, context.weapon);
    
    // Apply robe buff to wand / staff weapon
    Die die = context.weapon->die;
    if (context.source->armor.target == Target::PLAYERAC_SPELL)
        die.multiplier += 1;

    // Perform use of item, staff, or wand
    context.weapon->selected = false;
    int amount = die.Roll();

    if (context.weapon->target == Target::ALLPLAYERSHP)
    {
        // Restore all non-dead heroes hp
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0)
                m_heroes[index].currentHp += amount;
        }
    }
    else if (context.weapon->target == Target::ALLPLAYERSMP)
    {
        // Restore all non-dead heroes mp
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0)
                m_heroes[index].currentMp += amount;
        }
    }
    else if (context.weapon->target == Target::PLAYER_PROTECTALL)
    {
        // Protect all
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp > 0)
                m_heroes[index].protect = true;
        }
    }
    else if (context.weapon->target == Target::PLAYERHP_CONSUME)
    {
        context.target->currentHp += amount;
        *context.weapon = WEAPON("Unarmed");
    }
    else if (context.weapon->target == Target::PLAYERHP_REVIVE_CONSUME)
    {
        context.target->currentHp += amount;
        context.target->currentMp += amount;
        *context.weapon = WEAPON("Unarmed");
    }
    else if (context.weapon->target == Target::PLAYERHP_REUSE)
    {
        context.target->currentHp += amount;
    }
    else if (context.weapon->target == Target::PLAYER_PROTECT)
    {
        context.target->protect = true;
    }
    else if (context.weapon->target == Target::PLAYERMP_CONSUME)
    {
        context.target->currentMp += amount;
        *context.weapon = WEAPON("Unarmed");
    }
    else if (context.weapon->target == Target::PLAYERMP_REUSE)
    {
        context.target->currentMp += amount;
    }

    // Avoid exceeding maximum amounts
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp > m_heroes[index].totalHp)
            m_heroes[index].currentHp = m_heroes[index].totalHp;
        if (m_heroes[index].currentHp < 0)
            m_heroes[index].currentHp = 0;
        if (m_heroes[index].currentMp > m_heroes[index].totalMp)
            m_heroes[index].currentMp = m_heroes[index].totalMp;
        if (m_heroes[index].currentMp < 0)
            m_heroes[index].currentMp = 0;
    }
}

void DungeonCrawl::PurchaseItem()
{
    m_gold -= m_ui.GetContext().weapon->gold;
    m_ui.GetContext().weapon->purchased = true;

    if (m_ui.GetContext().weapon->target == Target::NEWHERO)
    {
        AddHero();
    }
    else if (m_ui.GetContext().weapon->target == Target::PLAYERLEVEL)
    {
        Hero* hero = static_cast<Hero*>(m_ui.GetContext().target);
        LevelUp(*hero);
    }
    else if (m_ui.GetContext().weapon->target == Target::PLAYERLEVEL5)
    {
        Hero* hero = static_cast<Hero*>(m_ui.GetContext().target);
        LevelUp(*hero); LevelUp(*hero); LevelUp(*hero); LevelUp(*hero); LevelUp(*hero);
    }
    else if (m_ui.GetContext().weapon->target == Target::PLAYERAC_SLOW
        || m_ui.GetContext().weapon->target == Target::PLAYERAC_SPEED
        || m_ui.GetContext().weapon->target == Target::PLAYERAC_SPELL)
    {
        m_ui.GetContext().target->armor = *m_ui.GetContext().weapon;
    }
    else
    {
        if (m_ui.GetCursorIndex() == 0)
        {
            m_ui.GetContext().target->weapon1 = *m_ui.GetContext().weapon;
        }
        if (m_ui.GetCursorIndex() == 1)
        {
            m_ui.GetContext().target->weapon2 = *m_ui.GetContext().weapon;
        }
        if (m_ui.GetCursorIndex() == 2)
        {
            m_ui.GetContext().target->weapon3 = *m_ui.GetContext().weapon;
        }
        if (m_ui.GetCursorIndex() == 3)
        {
            m_ui.GetContext().target->weapon4 = *m_ui.GetContext().weapon;
        }
    }
}

void DungeonCrawl::PushDoors()
{
    CursorContext context;
    context.state = CursorState::DOOR;
    context.cursor = ANIMATION("select_door");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.maxIndex = (int)m_currentFloorPtr->rooms.size() - 1;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushShop()
{
    CursorContext context;
    context.state = CursorState::SHOP;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 4; // Start at the exit
    context.maxIndex = (int)m_currentRoom->shop.size();
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushPassive()
{
    CursorContext context;
    context.state = CursorState::PASSIVE_SELECT;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0; // Start at the exit
    context.maxIndex = (int)m_passiveOptions.size() - 1;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushFountain()
{
    CursorContext context;
    context.state = CursorState::FOUNTAIN;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);

    // Artifact fountains have a second cursor slot for claiming the artifact reward. Cursor
    // index layout: 0 = exit (left), 1 = artifact (middle). Default fountains keep the
    // single-slot exit-only behavior. The cursor starts hovered on the artifact so the player
    // sees it highlighted on entry (per design ask).
    const bool artifact = (m_currentRoom != nullptr
        && m_currentRoom->door.rarity == Rarity::ARTIFACT
        && !m_currentRoom->rewardWeapons.empty());
    context.index    = artifact ? 1 : 0;
    context.maxIndex = artifact ? 1 : 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushTrap()
{
    CursorContext context;
    context.state = CursorState::TRAP;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0; // Start at the exit
    context.maxIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushTrapInitiated()
{
    // Trap evasion is driven by an armor-based "dexterity" score for the alive party.
    // LOWER score = better chance of slipping past unharmed.
    //   LEATHER (rogue):  -(4 + rarity*2)   stacks per rogue, so a 4-rogue party is nearly immune
    //   PLATE   (tank):   +(4 + rarity*2)   stacks per plate, so a heavy party is nearly always hit
    //   ROBE    (mage):    0                 neutral - mages perform "average" regardless of staves
    //   no armor:         +2                 small clumsiness penalty
    // Weapon speeds are deliberately NOT factored in - they're a combat-pacing stat, and including
    // them made staff-wielding mages feel as slow as plate, which the rebalance set out to fix.
    int dexterity  = 0;
    int aliveCount = 0;
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        const Hero& hero = m_heroes[index];
        if (hero.currentHp == 0)
            continue;
        aliveCount++;

        const int r = (int)hero.armor.rarity;
        switch (hero.armor.target)
        {
        case Target::PLAYERAC_SPEED: dexterity -= (4 + r * 2); break; // leather: rogue bonus
        case Target::PLAYERAC_SLOW:  dexterity += (4 + r * 2); break; // plate: tank penalty
        case Target::PLAYERAC_SPELL:                           break; // robe: neutral
        default:                     dexterity += 2;           break; // no armor: small penalty
        }
    }
    if (aliveCount == 0)
        aliveCount = 1;

    // DC tightens with depth: at floor 0 it sits around +10 (any party with positive dex is at
    // risk), at floor 20 it crashes to ~+3 (only rogue-leaning parties expect to pass), and at
    // floor 25+ it dips negative (only heavy rogue stacks pass reliably).
    const int min = std::max<int>( 0 - (m_floor / 3), -20);
    const int max = std::max<int>(20 - (m_floor / 3),   0);
    const int speedRollDC = GetRandomValue(min, max);

    m_trapDC   = speedRollDC;
    m_trapRoll = dexterity;

    // If the party can't beat the DC, the trap fires and damages everyone (alive or dead). Damage
    // scales harder than before so traps stay scary at the floor depths where the party HP pool
    // is large.  Per-hero damage at representative depths:
    //   floor  0 : 1d6+0  avg 3.5
    //   floor 10 : 3d6+3  avg 13.5
    //   floor 20 : 6d6+6  avg 27
    //   floor 25 : 7d6+8  avg 32.5
    bool willTrigger = (dexterity > speedRollDC);

    // TRAP_DISARMER passive: on a failed dexterity check, a 20% override flips the result back
    // to a success. This realises the +20% pass-rate intent without coupling to the floor-
    // dependent DC range. Combined with the gold reward below it makes the passive feel like a
    // dedicated trap specialist.
    const bool hasDisarmer = OwnsPassive(PassiveType::TRAP_DISARMER);
    if (willTrigger && hasDisarmer && GetRandomValue(0, 99) < 20)
        willTrigger = false;

    if (willTrigger)
    {
        m_trapTriggered = true;
        int damage = ROLL((m_floor / 4) + 1, 6, (m_floor / 3));
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].currentHp -= damage;
            if (m_heroes[index].currentHp < 0)
                m_heroes[index].currentHp = 0;

            m_heroes[index].currentMp -= damage;
            if (m_heroes[index].currentMp < 0)
                m_heroes[index].currentMp = 0;

            if (m_currentFloorPtr->type != DamageType::NORMAL)
            {
                m_heroes[index].condition = Die((m_floor / 5) + 1, 2, (m_floor / 5), m_currentFloorPtr->type);
                m_heroes[index].conditionTurnsLeft = GetRandomValue(1, 2);
            }
        }
    }
    else if (hasDisarmer)
    {
        // Successful disarm with the passive: small gold reward, scales mildly with floor.
        m_gold += 10 + m_floor;
    }

    m_trapInitiated = true;

    CursorContext context;
    context.state = CursorState::TRAP_INITIATED;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0; // Start at the exit
    context.maxIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushTreasure()
{
    CursorContext context;
    context.state = CursorState::CHEST;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0; // Start at the exit
    context.maxIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushReward()
{
    CursorContext context;
    context.state = CursorState::REWARD;
    context.cursor = ANIMATION("select_weapon");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0; // Start at the exit

    // Money-only rewards: exit at cursor 0 (maxIndex = 0).
    // Weapon rewards: weapons occupy [0..N-1], exit at N. So maxIndex = N for size-1 (=1)
    // and maxIndex = 2 for size-2.
    if (m_currentRoom->reward == Reward::MONEY
        || m_currentRoom->reward == Reward::RARE_MONEY
        || m_currentRoom->reward == Reward::EPIC_MONEY
        || m_currentRoom->reward == Reward::LEGENDARY_MONEY)
        context.maxIndex = 0;
    else
        context.maxIndex = (int)m_currentRoom->rewardWeapons.size();

    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushRewardHero()
{
    // Snapshot the stack depth before pushing the hero picker so the eventual completion
    // pop can restore the player to whichever screen kicked off the reward flow:
    //   reward room  -> [ROOT, CHEST, REWARD] (depth 3) -> back to REWARD
    //   artifact fountain -> [ROOT, FOUNTAIN] (depth 2) -> back to FOUNTAIN
    m_rewardReturnDepth = m_ui.GetSize();

    CursorContext context;
    context.cursor = ANIMATION("select_hero");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = (int)m_heroes.size() - 1;
    context.minIndex = 0;
    context.source = nullptr;
    context.state = CursorState::REWARD_HERO;
    context.target = nullptr;
    context.weapon = nullptr;
    context.direction = CursorIndexDirection::HORIZONTAL;

    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp == 0)
            context.excludeIndexes.push_back(index);
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushRewardHeroItem()
{
    // Route the reward the player actually selected on the REWARD screen (index 0 or 1) -
    // not always rewardWeapons[0], which was the old single-reward assumption.
    Weapon* picked = &m_currentRoom->rewardWeapons[m_selectedRewardIndex];

    // Rewards that don't need to occupy a weapon slot apply directly to the selected hero:
    //   - Armor (PLAYERAC_*) overwrites the hero's armor slot
    //   - New Hero / Level Up / Level Up 5 just modify the hero / party; no slot picker needed
    // For these, jump straight to PurchaseItem() and pop the UI back to the reward screen.
    const Target target = picked->target;
    if (target == Target::PLAYERAC_SLOW
        || target == Target::PLAYERAC_SPEED
        || target == Target::PLAYERAC_SPELL
        || target == Target::NEWHERO
        || target == Target::PLAYERLEVEL
        || target == Target::PLAYERLEVEL5)
    {
        m_ui.GetContext().source = &m_heroes[m_ui.GetCursorIndex()];
        m_ui.GetContext().target = &m_heroes[m_ui.GetCursorIndex()];
        m_ui.GetContext().weapon = picked;
        PurchaseItem();
        // Pop back to whichever screen launched this reward flow (depth captured in
        // PushRewardHero): REWARD for reward rooms, FOUNTAIN for artifact fountains.
        m_ui.PopBackTo(m_rewardReturnDepth);
        return;
    }

    CursorContext context;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = 1;
    context.minIndex = 0;
    context.source = &m_heroes[m_ui.GetCursorIndex()];
    context.state = CursorState::REWARD_HERO_ITEM;
    context.target = &m_heroes[m_ui.GetCursorIndex()];
    context.weapon = picked;
    context.direction = CursorIndexDirection::VERTICAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushCombat(Actor* hero)
{
    for (int index = 0; index < (int)m_heroes.size(); index++)
        m_heroes[index].isTurn = false;
    static_cast<Hero*>(hero)->isTurn = true;

    CursorContext context;
    context.state = CursorState::COMBAT;
    context.minIndex = 0;
    context.maxIndex = 1;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.direction = CursorIndexDirection::VERTICAL;
    context.source = hero;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushCombatSelection()
{
    Weapon* weapon = nullptr;
    if (m_ui.GetCursorIndex() == 0)
        weapon = &m_heroes[m_heroIndex].weapon1;
    else if (m_ui.GetCursorIndex() == 1)
        weapon = &m_heroes[m_heroIndex].weapon2;
    else if (m_ui.GetCursorIndex() == 2)
        weapon = &m_heroes[m_heroIndex].weapon3;
    else if (m_ui.GetCursorIndex() == 3)
        weapon = &m_heroes[m_heroIndex].weapon4;

    // Replace attack with unarmed if there isn't enough MP
    if (m_heroes[m_heroIndex].currentMp < GetEffectiveMpCost(&m_heroes[m_heroIndex], weapon))
        weapon = &WEAPON("Unarmed");

    // MP is charged at the moment an action is actually queued, NOT at weapon-select time.
    // For weapons that fire immediately below (ALLENEMIES / ALLPLAYERSHP / PLAYER_PROTECTALL)
    // we charge here. For single-target weapons (ENEMY / PLAYER) the charge happens in the
    // COMBAT_MONSTERS / COMBAT_HERO confirm handlers so pressing Back refunds nothing
    // (because nothing was spent). Duplicate-attack passives like WAND_FINESSE and
    // DAGGER_MULTIATTACK clone the queued action rather than re-routing through this function,
    // so charging once here covers all duplicates for free.
    auto chargeMp = [this](Weapon* w)
    {
        const int cost = GetEffectiveMpCost(&m_heroes[m_heroIndex], w);
        if (cost > 0)
        {
            m_heroes[m_heroIndex].currentMp -= cost;
            if (m_heroes[m_heroIndex].currentMp < 0)
                m_heroes[m_heroIndex].currentMp = 0;
        }
    };

    // If we can create an action already
    if (weapon->target == Target::ALLENEMIES)
    {
        chargeMp(weapon);
        Action action;
        action.source = &m_heroes[m_heroIndex];
        action.weapon = weapon;
        for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
            action.targets.push_back(&m_currentRoom->monsters[index]);
        m_actions.push_back(action);

        m_ui.PopBackTo(2);
        NextHero();
        return;
    }
    if (weapon->target == Target::ALLPLAYERSHP)
    {
        chargeMp(weapon);
        Action action;
        action.source = &m_heroes[m_heroIndex];
        action.weapon = weapon;
        for (int index = 0; index < (int)m_heroes.size(); index++)
            action.targets.push_back(&m_heroes[index]);
        m_actions.push_back(action);

        m_ui.PopBackTo(2);
        NextHero();
        return;
    }
    if (weapon->target == Target::PLAYER_PROTECTALL)
    {
        chargeMp(weapon);
        Action action;
        action.source = &m_heroes[m_heroIndex];
        action.weapon = weapon;
        for (int index = 0; index < (int)m_heroes.size(); index++)
            action.targets.push_back(&m_heroes[index]);
        m_actions.push_back(action);

        m_ui.PopBackTo(2);
        NextHero();
        return;
    }

    CursorContext context;
    context.direction = CursorIndexDirection::HORIZONTAL;
    context.source = &m_heroes[m_heroIndex];
    context.minIndex = 0;
    context.weapon = weapon;

    if (weapon->target == Target::ENEMY)
    {
        context.state = CursorState::COMBAT_MONSTERS;
        context.cursor = ANIMATION("select_door");
        context.cursor.SetAttributes(0, 0x0007);
        context.cursor.SetAttributes(1, 0x0008);
        context.maxIndex = (int)m_currentRoom->monsters.size() - 1;
        for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
        {
            if (m_currentRoom->monsters[index].currentHp == 0)
                context.excludeIndexes.push_back(index);
        }
    }
    else
    {
        context.state = CursorState::COMBAT_HERO;
        context.cursor = ANIMATION("select_hero");
        context.cursor.SetAttributes(0, 0x0007);
        context.cursor.SetAttributes(1, 0x0008);
        context.maxIndex = (int)m_heroes.size() - 1;
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp == 0)
                context.excludeIndexes.push_back(index);
        }
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushHero()
{
    CursorContext context;
    context.cursor = ANIMATION("select_hero");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = (int)m_heroes.size() - 1;
    context.minIndex = 0;
    context.source = nullptr;
    context.state = CursorState::HERO;
    context.target = nullptr;
    context.weapon = nullptr;
    context.direction = CursorIndexDirection::HORIZONTAL;

    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp == 0)
            context.excludeIndexes.push_back(index);
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushHeroItem()
{
    CursorContext& prevContext = m_ui.GetContext();

    CursorContext context;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = 1;
    context.minIndex = 0;
    context.source = &m_heroes[prevContext.index];
    context.state = CursorState::HERO_ITEM;
    context.target = nullptr;
    context.weapon = nullptr;
    context.direction = CursorIndexDirection::VERTICAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushUseItem()
{
    CursorContext& prevContext = m_ui.GetContext();

    // Select the weapon from the hero
    Weapon* currentWeapon = nullptr;
    if (prevContext.index == 0)
        currentWeapon = &prevContext.source->weapon1;
    if (prevContext.index == 1)
        currentWeapon = &prevContext.source->weapon2;
    if (prevContext.index == 2)
        currentWeapon = &prevContext.source->weapon3;
    if (prevContext.index == 4)
        currentWeapon = &prevContext.source->weapon4;

    // Cannot afford to cast this
    if (prevContext.source->currentMp < GetEffectiveMpCost(prevContext.source, currentWeapon))
        return;

    // Staff doesn't require a target can go strait to using item
    if (currentWeapon->target == Target::ALLPLAYERSHP
        || currentWeapon->target == Target::PLAYER_PROTECTALL)
    {
        m_ui.GetContext().weapon = currentWeapon;
        UseSelectedItem();
        return;
    }

    CursorContext context;
    context.cursor = ANIMATION("select_hero");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = (int)m_heroes.size() - 1;
    context.minIndex = 0;
    context.source = prevContext.source;
    context.state = CursorState::USE_ITEM;
    context.target = nullptr;
    context.weapon = currentWeapon;
    context.direction = CursorIndexDirection::HORIZONTAL;

    // Unless its a resurrection potion exclude dead heroes
    if (currentWeapon->target != Target::PLAYERHP_REVIVE_CONSUME)
    {
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            if (m_heroes[index].currentHp == 0)
                context.excludeIndexes.push_back(index);
        }
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushShopHero()
{
    CursorContext& prevContext = m_ui.GetContext();

    CursorContext context;
    context.cursor = ANIMATION("select_hero");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = (int)m_heroes.size() - 1;
    context.minIndex = 0;
    context.source = nullptr;
    context.state = CursorState::SHOP_HERO;
    context.target = nullptr;
    context.weapon = &m_currentRoom->shop[m_ui.GetCursorIndex()];
    context.direction = CursorIndexDirection::HORIZONTAL;

    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp == 0)
            context.excludeIndexes.push_back(index);
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushShopItem()
{
    CursorContext& prevContext = m_ui.GetContext();

    if (prevContext.weapon->target == Target::NEWHERO
        || prevContext.weapon->target == Target::PLAYERLEVEL
        || prevContext.weapon->target == Target::PLAYERLEVEL5
        || prevContext.weapon->target == Target::NEWHERO
        || prevContext.weapon->target == Target::PLAYERAC_SLOW
        || prevContext.weapon->target == Target::PLAYERAC_SPEED
        || prevContext.weapon->target == Target::PLAYERAC_SPELL)
    {
        prevContext.source = &m_heroes[m_ui.GetCursorIndex()];
        prevContext.target = &m_heroes[m_ui.GetCursorIndex()];
        PurchaseItem();
        m_ui.PopBackTo(2);
        return;
    }

    CursorContext context;
    context.cursor = ANIMATION("select_nothing");
    context.cursor.SetAttributes(0, 0x0007);
    context.cursor.SetAttributes(1, 0x0008);
    context.index = 0;
    context.maxIndex = 1;
    context.minIndex = 0;
    context.source = &m_heroes[m_ui.GetCursorIndex()];
    context.state = CursorState::SHOP_HERO_ITEM;
    context.target = &m_heroes[m_ui.GetCursorIndex()];
    context.weapon = prevContext.weapon;
    context.direction = CursorIndexDirection::VERTICAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushMenu()
{
    CursorContext context;
    context.state = CursorState::MENU;
    context.cursor = ANIMATION("select_nothing");
    context.index = 0;
    context.maxIndex = 2;
    context.direction = CursorIndexDirection::VERTICAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushMenuRestart()
{
    CursorContext context;
    context.state = CursorState::MENU_RESTART;
    context.cursor = ANIMATION("select_nothing");
    context.index = 0;
    context.maxIndex = 1;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::TestDungeon()
{
    for (int index = 0; index < 100; index++)
    {
        Floor floor;
        m_dungeonEx.GetNextFloor(std::move(floor));
        PrintFloor(floor);
    }

    exit(1);
}

void DungeonCrawl::PrintFloor(Floor floor)
{
    printf("[%d] %s %s %s Rm:%d\n",
        floor.floorNumber,
        ToString(floor.type).c_str(),
        ToString(floor.rarity).c_str(),
        ToString(floor.family).c_str(),
        floor.rooms.size());

    for (int index = 0; index < floor.rooms.size(); index++)
    {
        PrintRoom(index, floor.rooms[index]);
    }
}

void DungeonCrawl::PrintRoom(int index, Room room)
{
    printf("  Rm:%d %s %s ",
        index,
        room.door.label.c_str(),
        room.door.stateLabel.c_str());
    if (room.reward != Reward::INVALID)
    {
        printf("Rw:%s Gld:%d", ToString(room.reward).c_str(), room.gold);
        for (size_t i = 0; i < room.rewardWeapons.size(); i++)
        {
            printf(" Wpn[%zu]:%s %s", i,
                ToString(room.rewardWeapons[i].rarity).c_str(),
                room.rewardWeapons[i].name.c_str());
        }
        printf(" ");
    }
    if (room.trap != TrapType::INVALID)
    {
        printf("Trap:%s", room.trap == TrapType::TRAP_SPIKES ? "spikes" : "swinging axe");
    }
    printf("\n");

    for (int index = 0; index < room.monsters.size(); index++)
    {
        //PrintMonster(index, room.monsters[index]);
    }
    for (int index = 0; index < room.shop.size(); index++)
    {
        //PrintWeapon(index, room.shop[index]);
    }
}

void DungeonCrawl::PrintMonster(int index, Monster monster)
{
    printf("    Monster[%d] %s %s\n",
        index,
        ToString(monster.rarity).c_str(),
        monster.name.c_str());
}

void DungeonCrawl::PrintWeapon(int index, Weapon weapon)
{
    printf("    Wpn[%d] %s %s\n",
        index,
        ToString(weapon.rarity).c_str(),
        weapon.name.c_str());
}