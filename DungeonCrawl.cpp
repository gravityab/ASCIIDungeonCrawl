///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DungeonCrawl.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "DungeonCrawl.h"

/// Dungeon Crawl Library Headers
#include "Sleep.h"

// --------------------------------------------------------------------------------------------------------------------
Time s_blinkTime = ToMilliseconds(600);

#define IMAGE(x)     m_db.m_imageDb[x]
#define FRAME(x)     m_db.m_frameDb[x]
#define ANIMATION(x) m_db.m_animationDb[x]
#define WEAPON(x)    m_db.m_weaponDb[x] 
#define MONSTER(x)   m_db.m_monsterDb[x]
#define BLINK(x)     m_blink ? x : x | 0xC000
#define SOLID(x)     x | 0xC000

// --------------------------------------------------------------------------------------------------------------------
DungeonCrawl::DungeonCrawl()
    : m_db(m_dungeon.GetDatabase())
{}

DungeonCrawl::~DungeonCrawl()
{}

bool DungeonCrawl::Initialize(HANDLE handle, int frameLimit)
{
    m_handle = handle;

    if (frameLimit > 0)
        m_frameTimeLimit = ToSeconds(1.f / static_cast<float>(frameLimit));
    else
        m_frameTimeLimit = Time::Zero;

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
        m_console.SetData(' ', 0, 0, 100, 30, 0x0007);

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
            case State::STATE_FOUNTAIN:
                DrawFountain(delta);
                break;
            case State::STATE_TRAP:
                DrawTrapRoom(delta);
                break;
        }

        m_console.Draw(m_handle);
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void DungeonCrawl::DrawMainScreen(Time delta)
{
    std::string mainMenu =
        "XXXXXXXX$$xxXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX&&XXXXXXXXXXXXXX$$$XXXXXXX&$XXXXXXXXXXXXxXXXXXXXXxxXXX" \
        "XXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&&&XXXX$XXXXXXXXX$$XXXXXXXX&XXXXXXXXXXXXXXXXXXXXXXXxXXX" \
        "XXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXX$$XXXXXXXXX$&&$XXX$XXXXXXXXXXXXXXXXXXX&$XXXXXXXXXXXXXXXXXXXXXXXXX$" \
        "XXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXX$$XXXXXXXXXX&&$XXXXXXXXXXXXXXXXXXXXXXX&$XXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "XXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&&$XXXXXXXXXXXXXXXXXXXXXXX&$XXXXXXX$$XXXXXXXXXXXXXXXXX" \
        "XXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$$&&$XXXXXXXXXXXXXXXXXXXXXX$&$$XXXXXXXXXXXXXXXXXXXXXXXXX" \
        "XXXXXX$$&$XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$$&&$XXXXXXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "XXXX$$$&&&$$XXXXXXXXXXXXXXXXXXXXXXXXXXX$$$$$$$&&$XXXXXXXXXXXXXXXXXXXXXX$&$$$$$$$$$$$$$$$$$$$$$$$$$$$" \
        "$$$$&&&&&&$$$$$$$$$$$$$$$$$$$&&$$$$$$$&&&&$$$$$$$$$$$$&&&&&&&&&&&&$$$$$$$$$$$$$$$$$&&&&&&&&&&&&&&&&&" \
        "XXx+$$Xx+++++++++++++++++++++x&&&&$X++++x++++++++++++++++++xX$$&&$x++++++++++++++xx+$&$XXXxxxxxxxxx+" \
        "XXXX$$+xxXXXXXXx+xXXXXXXXXXXXx+&$x+xXXXXxxXXXXXXXXXXXXXXXXXx++X&$xXXXXXXXXXXXXXXXX$X$$x+xXXXXXXXXXXX" \
        "XXX$&XXXXXXXXXXXXXXXXXXXXXXXXX+$x+XXXXXXXXXXXXXXXXX$XXXXXXXXXxX&xxXXXXXXXXXXXXXXXXXX$x+xxxXXXXXXXXXX" \
        "XXX&&$XXXXXXXXXXXXXXXXXXXXXXXX+$XXXXXXXXXXXXXX$XXXX$XXXXXXXXXX$&$XXXXXXXXXXXXXXXXXX$&XXXxXXXXXXXXXXX" \
        "XXX$&$XXXXXXXXXXXXX$$$XXXXXXXXX&XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXX" \
        "XXXX$$XXXXXXXXXXXXXX$$XXXXXXXX$&XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&$XXXXX$XXXXXXXXXXXX$&$XXXXXXXXXXXXXX" \
        "XXXX$$$$$XXXXXXXXXXXXXXXXXXXX$$&XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&$$XXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXX" \
        "XXX$&&&$XXXXXXXXXXXXXXXXXXX$$$$&XXXXXXXXXXXXXXXXXXXXXXXXXXXXX$&&&$XXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXX" \
        "XXX$&&$$$$$$$$$$$$$$$X$$$$$$$$$&&$XXXXXXXXXXXXXXXXXXXXXXXXXXX$&&&$$$$$XXXXXXXXXXXX$$&$XXXXXXXXXX$XXX" \
        "$$$&&&&&&$&&&&$&&&&&$$$$$$$$$$&&&&&&$$$$$$&&&&&&$$$$$$$$$$$$$$&&&$&$&$$$$$$$$$$$$$$$&&$XXXXXXXXXX$XX" \
        "&&$xx++++++++++++++x$$$x+++++++++++++++++++xxX&&&&&&X++++++++++++++++$&&&$$$$$$$$$$$$$$$$$$$$$$$$$$$" \
        "$x+xXXXXXXXXXXXXXXXx+xXX+++++++++XXXXXXXXXXXXX$&&$++XXXXXXXXXXXXXXXXXXX&$+++++++++xXXXXXXX++++++++++" \
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX$XXXXXXXXXXXXXXX$&&$+XXXXXXXXXXXXXXXXXXXX&$++XXXXXXXXXXXXXXXXXXXXXXXXx" \
        "$X$XXXXXXXXXXXXXXXXX$$XXXXXXXXXXXXXXXXXXXXXXXXx$&$XXXXXXXXXXXXXXXXXXXXX&X+XXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "$XX$XXXXXXXX$$$$XXXX$$XXXXXXXXXXXXXXXXXXXXXXXXx$&$XXXXXXXXXXXXXXXXXXXX$&X+XXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "$XXXXXXXXXXXXXXX$XXX$$$XXXXXXXXXXXXXXXXXXXXXXXx$&$XXXXXXXXXXXXXXXXXXXX$&XxXXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "$$XXXXXXXXXXXXXXXXXX$$$XXXXXXXXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXXXXXXXXXXXXXXX" \
        "$$XXXXXXXXXXXXXXXXXX$$$$XX$XXXXXXXXXXXXXXXXXXX$$&$XXXXXXXXXXXXXXXXXXXX$&$XXXXXXXXXXXXXXX$XXXXXXXXXXX" \
        "$$$XXXXXXXXXXXXXXXXX$$$$X$XXXXXXXXXXXXXXXXXXXX$$&$XXXXX$X$$$$$XXXXXXXX$&$XXXXXXXXXXXXXX$$XXXXXXXXXXX";

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

    static int colorSwap = 0;
    if (++colorSwap > 400)
        colorSwap = 0;

    m_console.WriteData(mainMenu.data(), 0, 0, 100, 28, 0x0008);
    m_console.WriteData(title.data(), 10, 4, 81, 12, 0x0006);
    m_console.WriteData(pressEnter.data(), 17, 23, 66, 4, colorSwap > 200 ? 0x0008 : 0x000B);

    if (m_input.Released(Button::BUTTON_SELECT))
        SetState(State::STATE_SHOP);
}

void DungeonCrawl::DrawDoorsScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 0, ToAttribute(m_currentFloor->type));

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
        SetState(m_currentRoom->door.state);
}

void DungeonCrawl::DrawShopScreen(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 1, ToAttribute(m_currentFloor->type));

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
    DrawBackground(delta, 1, ToAttribute(m_currentFloor->type));

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
    DrawBackground(delta, 2, ToAttribute(m_currentFloor->type));

    // Draw the rewards
    DrawReward(delta);

    // Draw the party
    DrawHero(delta);

    // Handle cursor state
    DrawCursor();
}

void DungeonCrawl::DrawFountain(Time delta)
{
    // Draw the doors first
    DrawBackground(delta, 1, ToAttribute(m_currentFloor->type));

    bool complete;
    m_fountain.WriteData(m_console, delta, 74, 4, complete);

    // Draw the party
    DrawHero(delta);

    // Draw the fairies
    DrawFairies(delta);

    // Draw cursor
    DrawCursor();
}

void DungeonCrawl::DrawTrapRoom(Time delta)
{
    // Draw the doors first
    DrawTrap(delta, ToAttribute(m_currentFloor->type), m_trapInitiated, m_trapTriggered);

    // Draw the party
    DrawHero(delta);

    // Draw cursor
    DrawCursor();
}

void DungeonCrawl::NextFloor(Time delta)
{
    // Draw the doors first
    DrawStairs(delta, ToAttribute(m_currentFloor->type));

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
        //if (m_cursorIndex == 0)
        //	m_cursor.WriteData(m_console, delta, 2, 3, complete);

        m_console.WriteData(4, 4, 0x0007, "CHALANGE: %d", m_trapDC);
        m_console.WriteData(4, 5, m_trapRoll > m_trapDC ? 0x0004 : 0x0002, "ROLL: %d", m_trapRoll);
    }
}

void DungeonCrawl::DrawDoors(Time delta)
{
    bool complete;
    for (int index = 0; index < (int)m_currentFloor->rooms.size(); index++)
    {
        Door& door = m_currentFloor->rooms[index].door;
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
            uint16_t attribute = ToAttribute(m_currentFloor->rarity);
            for (int rm = 0; rm < (int)m_currentFloor->rooms.size(); rm++)
            {
                Room& room = m_currentFloor->rooms[rm];
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
                        m_console.WriteData(x + 14, 7, attribute, "%d Monster", room.monsters.size());
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

    //if (m_cursorIndex == 4)
    //	m_cursor.WriteData(m_console, delta, x, y, complete);
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

        // Draw border and condition
        hero.idle.SetAttributes(0, borderColor);
        hero.idle.WriteData(m_console, delta, x, y, complete);
        if (hero.condition.die != 0)
            m_console.WriteData(x + 6, y, ToAttribute(hero.condition.type), " %s ", ToConditionString(hero.condition.type).c_str());

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
                m_console.WriteData(x + 4, y + 5, ToAttribute(hero.armor.rarity), "(AC %d) %s", hero.armor.armorClass + hero.level, hero.armor.name.c_str());
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
            Die buffDie = Die((hero.armor.target == Target::PLAYERAC_SPELL && hero.weapon1.mpCost) 
				? 1 + levelMultiplier: 0 + levelMultiplier,
				0,
				hero.level);

            if (hero.currentMp >= hero.weapon1.mpCost)
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

            if (hero.currentMp >= hero.weapon2.mpCost)
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

        //if (index == m_cursorMonsterIndex && m_cursorState == CursorState::MONSTERS)
        //	m_cursor.WriteData(m_console, delta, x + 2, y - 1, complete);

        if (index == m_ui.GetCursorIndex() && m_ui.GetState() == CursorState::COMBAT_MONSTERS)
            m_ui.GetAnimation().WriteData(m_console, delta, x + 2, y - 1, complete);

        m_console.WriteData(x + 3, y, ToAttribute(monster.rarity), monster.name.c_str());

        if (monster.attacking)
            monster.attack.WriteData(m_console, delta, x, y, complete);
        else
            monster.idle.WriteData(m_console, delta, x, y, complete);

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
            || m_currentRoom->reward == Reward::EPIC_WEAPON
            || m_currentRoom->reward == Reward::LEGENDARY_MONEY)
        {}
        else
        {
            int x = 21;
            int y = 4;
            DrawItem(delta, &m_currentRoom->rewardWeapon, x, y);

            if (m_ui.GetState() == CursorState::REWARD)
            {
                if (m_ui.GetCursorIndex() == 0)
                    m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
            }
        }

        // Draw the exit button
        {
            int x = 62;
            int y = 4;

            int exitIndex = (m_currentRoom->reward == Reward::MONEY
                || m_currentRoom->reward == Reward::RARE_MONEY
                || m_currentRoom->reward == Reward::EPIC_WEAPON
                || m_currentRoom->reward == Reward::LEGENDARY_MONEY) ? 0 : 1;

            ANIMATION("exit").WriteData(m_console, delta, x, y, complete);
            m_console.WriteData(x + 8, y + 8, 0x0006, "+o   ");
            m_console.WriteData(x + 11, y + 8, 0x0007, "%d", m_currentRoom->gold);

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
    if (m_ui.GetCursorIndex() == 0)
        m_ui.GetAnimation().WriteData(m_console, delta, x, y, complete);
    //if (m_cursorIndex == 0)
    //	m_cursor.WriteData(m_console, delta, x, y, complete);
}

void DungeonCrawl::DrawAction(Time delta)
{
    if (m_actions.size() > 0)
    {
        bool complete;
        auto iter = m_actions.begin();
        Action& action = (*iter);

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
                monster->attacking = true;
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

        if (m_damageTimeLeft > Time::Zero)
        {
            action.weapon->attack.Reset();
            action.weapon->attack.SetAnimating(true);

            if (action.source && action.source->GetType() == ActorType::ACTOR_MONSTER)
            {
                Monster* monster = static_cast<Monster*>(action.source);
                monster->attacking = false;
                monster->attack.Reset();
                monster->attack.SetAnimating(true);
            }
            else if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
            {
                Hero* hero = static_cast<Hero*>(action.source);
                hero->isTurn = false;
            }

            m_damageTimeLeft -= delta;
            if (m_damageTimeLeft < Time::Zero)
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
                    monster->attacking = false;
                    monster->attack.Reset();
                    monster->attack.SetAnimating(true);
                }
                else if (action.source && action.source->GetType() == ActorType::ACTOR_HERO)
                {
                    Hero* hero = static_cast<Hero*>(action.source);
                    hero->isTurn = false;
                }

                // Asumming the actor isn't dead, use the weapon
                if (!action.source || action.source->currentHp > 0)
                {
                    UseWeapon(action);
                    m_damageTimeLeft = ToMilliseconds(1000);
                }
                else
                {
                    m_actions.erase(iter);
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
            SetState(State::STATE_COMBAT);
        if (heroesAlive && !monstersAlive)
            SetState(State::STATE_TREASURE);
        if (!heroesAlive)
        {
            // Game over. Display the screen until the player presses start
            if (m_input.Released(Button::BUTTON_SELECT))
            {
                SetState(State::STATE_MAIN);
            }
        }
    }
}

void DungeonCrawl::ProcessInput()
{
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

        if (m_ui.GetState() == CursorState::REWARD)
            m_ui.PopBack(3);
        else
            m_ui.PopBack(2);
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

    // Door
    {
        if (m_ui.GetState() == CursorState::DOOR && m_input.Released(Button::BUTTON_SELECT))
        {
            m_currentRoom = &m_currentFloor->rooms[m_ui.GetCursorIndex()];
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
            m_actions.push_back(action);

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

            m_ui.PopBackTo(2);
            PushReward();
            return;
        }

        if (m_ui.GetState() == CursorState::REWARD && m_input.Released(Button::BUTTON_SELECT))
        {
            if (m_ui.GetCursorIndex() == 1)
            {
                m_gold += m_currentRoom->gold;
                SetState(State::STATE_NEXT_FLOOR);
                return;
            }

            if ((m_currentRoom->reward == Reward::MONEY
                || m_currentRoom->reward == Reward::RARE_MONEY
                || m_currentRoom->reward == Reward::EPIC_MONEY
                || m_currentRoom->reward == Reward::LEGENDARY_MONEY)
                && m_ui.GetCursorIndex() == 0)
            {
                m_gold += m_currentRoom->gold;
                SetState(State::STATE_NEXT_FLOOR);
                return;
            }
            else
            {
                if (!m_currentRoom->rewardWeapon.purchased)
                {
                    PushRewardHero();
                }
                return;
            }
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
                m_ui.PopBackTo(3);
                return;
            }

            m_ui.GetContext().source->weapon1.selected = m_ui.GetCursorIndex() == 0;
            m_ui.GetContext().source->weapon2.selected = m_ui.GetCursorIndex() == 1;
            m_ui.GetContext().source->weapon3.selected = m_ui.GetCursorIndex() == 2;
            m_ui.GetContext().source->weapon4.selected = m_ui.GetCursorIndex() == 3;
        }
    }

    m_console.WriteData(0, 0, 0x0008, "%s ", ToString(m_ui.GetState()).c_str());
}

void DungeonCrawl::DoorInput()
{
    

    if (m_input.Released(Button::BUTTON_UP))
    {
        //if (m_ui.GetState() == CursorState::DOOR)
    }

}

void DungeonCrawl::DrawCursor()
{
    ProcessInput();
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

void DungeonCrawl::ReceiveXP(Hero& hero)
{
    for (int index = 0; index < (int)m_currentRoom->monsters.size(); index++)
    {
        Monster& monster = m_currentRoom->monsters[index];

        // Subtract xp from xp left to level
        hero.experience -= monster.experience;
        if (hero.experience <= 0)
            LevelUp(hero);
    }
}

void DungeonCrawl::LevelUp(Hero& hero)
{
    static int s_levels[20] = { 0, 10, 20, 40, 80, 160, 320, 640, 1280, 2560, 5120, 10240 };

    if (hero.level >= 20)
        return;

    hero.level++;
    hero.experience = s_levels[hero.level];

    if (hero.armor.target == Target::PLAYERAC_SLOW)
    {
        // If you equip PLATE you gain more HP during levels
        hero.totalHp += ROLL(2, 10, hero.level + 1);
        hero.totalMp += ROLL(1, 10, hero.level + 1);
    }
    else if (hero.armor.target == Target::PLAYERAC_SPELL)
    {
        // If you equip ROBES you gain more MP during levels
        hero.totalHp += ROLL(1, 10, hero.level + 1);
        hero.totalMp += ROLL(2, 10, hero.level + 1);
    }
    else if (hero.armor.target == Target::PLAYERAC_SPEED)
    {
        // If you equip LEATHER you lower the amount required for levels
        hero.experience /= 2;
        hero.totalHp += ROLL(1, 10, hero.level + 1);
        hero.totalMp += ROLL(1, 10, hero.level + 1);
    }
    else
    {
        // Equiping no ARMOR makes you gain little HP / MP for levels
        hero.totalHp += ROLL(1, 10, hero.level + 1);
        hero.totalMp += ROLL(1, 10, hero.level + 1);
    }

    hero.levelUpTimeLeft = ToMilliseconds(1500);

    hero.currentHp = hero.totalHp;
    hero.currentMp = hero.totalMp;
}

void DungeonCrawl::UseWeapon(Action action)
{
    m_damageAttribute = 0x0004; // Red

    // Roll the damage to heal or deal
    DamageType damageType;
    Die damageDie = action.weapon->die;
    int damage = 0;

    if (action.source)
    {
        // Apply robe buff to wand / staff weapon
        if (action.weapon->mpCost > 0
            && action.source
            && action.source->GetType() == ActorType::ACTOR_HERO)
        {
            Hero* hero = static_cast<Hero*>(action.source);
            if (hero->armor.target == Target::PLAYERAC_SPELL)
            {
                damageDie.multiplier += 1;
                m_damageAttribute = 0x0006; // Gold
            }
        }

		if (action.source->GetType() == ActorType::ACTOR_HERO)
		{
			damageDie.multiplier += (action.source->level / 3); // Every 3 levels we add 1 mult
			damageDie.constant += action.source->level; // Ever level we add 1 const
		}

        damage = damageDie.Roll(&damageType);

        action.source->currentMp -= action.weapon->mpCost;
        if (action.source->currentMp < 0)
            action.source->currentMp = 0; // It's possible some moves drain MP
    }
    else
    {
        // Condition damage
        damage = damageDie.Roll(&damageType);
    }

    // Keep track of damage before modifiers
    m_damageOriginal = damage;
    m_damageAttribute2 = ToAttribute(damageType);

    bool isHealing = action.weapon->target == Target::ALLPLAYERSHP
        || action.weapon->target == Target::PLAYERHP_CONSUME
        || action.weapon->target == Target::PLAYERHP_REUSE;
    bool isRestoring = action.weapon->target == Target::ALLPLAYERSMP
        || action.weapon->target == Target::PLAYERMP_CONSUME
        || action.weapon->target == Target::PLAYERMP_REUSE;
    bool isDamaging = action.weapon->target == Target::ALLENEMIES
        || action.weapon->target == Target::ENEMY
        || action.weapon->target == Target::MONSTER_ALLENEMIES
        || !action.source;
    bool isDraining = action.weapon->target == Target::MONSTER_DRAINMP;

    bool consumesWeapon = action.weapon->target == Target::PLAYERMP_CONSUME
        || action.weapon->target == Target::PLAYERHP_CONSUME;

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
        monster->attacking = false;
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

    //if (action.weapon->target == Target::MONSTER_RECRUIT
    //	|| action.weapon->target == Target::MONSTER_SWITCH)
    //{
    //	Monster* monster = static_cast<Monster*>(action.source);
    //	Monster copy = *monster;
    //	if (m_currentRoom->monsters.size() != 4)
    //		m_currentRoom->monsters.push_back(copy);
    //	return;
    //}

    // Iterate over the targets and apply the result
    for (int index = 0; index < (int)action.targets.size(); index++)
    {
        damage = m_damageOriginal;

        Actor* actor = action.targets[index];

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

        // Reduce or increase damage to if resistant or weak
        if (isDamaging || isDraining)
        {
            bool hasWeakness = false;
            bool hasResistance = false;
            for (int res = 0; res < (int)actor->armor.resistances.size(); res++)
            {
                if (actor->armor.resistances[res] == damageType)
                {
                    hasResistance = true;
                    break;
                }
                if (actor->armor.resistances[res] == ToWeakness(damageType))
                {
                    hasWeakness = true;
                    break;
                }
            }
            if (hasResistance)
                damage = int(damage / 2);
            if (hasWeakness)
                damage = int(damage * 2);

            // Subtract armor class
            damage -= actor->armor.armorClass;

            // Subtract level
            if (actor->GetType() == ActorType::ACTOR_HERO)
            {
                Hero* hero = static_cast<Hero*>(actor);
                damage -= hero->level;
            }

            if (damage <= 0)
                damage = 1;
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
            actor->currentHp -= damage;
            if (actor->currentHp < 0)
                actor->currentHp = 0;
            if (actor->currentHp > actor->totalHp)
                actor->currentHp = actor->totalHp;
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
    m_condition1.die = Die(0, 0, 0, DamageType::NORMAL);
    m_condition2.die = Die(0, 0, 0, DamageType::NORMAL);
    m_condition3.die = Die(0, 0, 0, DamageType::NORMAL);
    m_condition4.die = Die(0, 0, 0, DamageType::NORMAL);

    // Deal condition damage
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
        m_dungeon.Reset();
        m_floor = 0;

        m_gold = GetRandomValue(250, 300);
        m_input.Initialize();
        m_dungeon.Initialize();
        m_dungeon.Generate();
        m_db = m_dungeon.GetDatabase();
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
        m_currentFloor = &m_initFloor;
        m_currentRoom = &m_initFloor.rooms[0];
    }
    else if (state == State::STATE_NEXT_FLOOR)
    {
        m_stairs = ANIMATION("stairs");
        m_timeLeft = ToMilliseconds(2000);
        m_floor++;
        m_currentFloor = &m_dungeon.GetFloor(m_floor);
    }
    else if (state == State::STATE_DOORS)
    {
        PushDoors();
        m_currentRoom = nullptr;
    }
    else if (state == State::STATE_SHOP)
    {
        // At max heroes, "New Hero" items are converted to "Level Up 5" items
        if (m_heroes.size() == 4)
        {
            for (int index = 0; index < m_currentRoom->shop.size(); index++)
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

        PushShop();
    }
    else if (state == State::STATE_COMBAT)
    {
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].weapon1.selected = false;
            m_heroes[index].weapon2.selected = false;
            m_heroes[index].weapon3.selected = false;
            m_heroes[index].weapon4.selected = false;
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
        }
        PushFountain();
    }
    else if (state == State::STATE_TRAP)
    {
        m_trapInitiated = false;
        m_trapTriggered = false;
        PushTrap();
    }

    m_state = state;
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
    std::sort(m_actions.begin(), m_actions.end(), [](const Action& left, const Action& right)
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

        return leftSpeed < rightSpeed;
    });
}

void DungeonCrawl::UseSelectedItem()
{
    CursorContext& context = m_ui.GetContext();
    
    // Pay the cost
    context.source->currentMp -= context.weapon->mpCost;
    
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
    else if (context.weapon->target == Target::PLAYERHP_CONSUME)
    {
        context.target->currentHp += amount;
        *context.weapon = WEAPON("Unarmed");
    }
    else if (context.weapon->target == Target::PLAYERHP_REUSE)
    {
        context.target->currentHp += amount;
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
    context.maxIndex = (int)m_currentFloor->rooms.size() - 1;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushShop()
{
    CursorContext context;
    context.state = CursorState::SHOP;
    context.cursor = ANIMATION("select_weapon");
    context.index = 4; // Start at the exit
    context.maxIndex = (int)m_currentRoom->shop.size();
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushFountain()
{
    CursorContext context;
    context.state = CursorState::FOUNTAIN;
    context.cursor = ANIMATION("select_weapon");
    context.index = 0; // Start at the exit
    context.maxIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushTrap()
{
    CursorContext context;
    context.state = CursorState::TRAP;
    context.cursor = ANIMATION("select_weapon");
    context.index = 0; // Start at the exit
    context.maxIndex = 0;
    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushTrapInitiated()
{
    // Calculate total player speed. Lower the better
    int totalHeroes = 0;
    int totalSpeed = 0;
    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp > 0)
        {
            totalHeroes++;
            totalSpeed += m_heroes[index].weapon1.speed;
            totalSpeed += m_heroes[index].weapon2.speed;
            totalSpeed += m_heroes[index].weapon3.speed;
            totalSpeed += m_heroes[index].weapon4.speed;
            if (m_heroes[index].armor.target == Target::PLAYERAC_SPEED)
                totalSpeed -= m_heroes[index].armor.speed;
            if (m_heroes[index].armor.target == Target::PLAYERAC_SLOW)
                totalSpeed += m_heroes[index].armor.speed;
        }
    }
    totalSpeed /= totalHeroes;

    // Calculate the trap DC
    int min = std::max<int>(5 - (m_floor / 5), 0);
    int max = std::max<int>(25 - (m_floor / 5), 0);
    int speedRollDC = GetRandomValue(min, max);

    m_trapDC = speedRollDC;
    m_trapRoll = totalSpeed;

    // If the roll fails, damage everyone
    if (totalSpeed > speedRollDC)
    {
        m_trapTriggered = true;
        int damage = ROLL((m_floor / 5) + 1, 4, (m_floor / 5));
        for (int index = 0; index < (int)m_heroes.size(); index++)
        {
            m_heroes[index].currentHp -= damage;
            if (m_heroes[index].currentHp < 0)
                m_heroes[index].currentHp = 0;

            m_heroes[index].currentMp -= damage;
            if (m_heroes[index].currentMp < 0)
                m_heroes[index].currentMp = 0;

            if (m_currentFloor->type != DamageType::NORMAL)
            {
                m_heroes[index].condition = Die((m_floor / 5) + 1, 2, (m_floor / 5), m_currentFloor->type);
                m_heroes[index].conditionTurnsLeft = GetRandomValue(1, 2);
            }
        }
    }

    m_trapInitiated = true;

    CursorContext context;
    context.state = CursorState::TRAP_INITIATED;
    context.cursor = ANIMATION("select_weapon");
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
    context.index = 0; // Start at the exit

    if (m_currentRoom->reward == Reward::MONEY
        || m_currentRoom->reward == Reward::RARE_MONEY
        || m_currentRoom->reward == Reward::EPIC_MONEY
        || m_currentRoom->reward == Reward::LEGENDARY_MONEY)
        context.maxIndex = 0;
    else
        context.maxIndex = 1;

    context.direction = CursorIndexDirection::HORIZONTAL;
    m_ui.PushBack(context);
}

void DungeonCrawl::PushRewardHero()
{
    CursorContext context;
    context.cursor = ANIMATION("select_hero");
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
    // Check if the reward is armor
    if (m_currentRoom->rewardWeapon.target == Target::PLAYERAC_SLOW
        || m_currentRoom->rewardWeapon.target == Target::PLAYERAC_SPEED
        || m_currentRoom->rewardWeapon.target == Target::PLAYERAC_SPELL)
    {
        m_ui.GetContext().source = &m_heroes[m_ui.GetCursorIndex()];
        m_ui.GetContext().target = &m_heroes[m_ui.GetCursorIndex()];
        m_ui.GetContext().weapon = &m_currentRoom->rewardWeapon;
        PurchaseItem();
        m_ui.PopBackTo(3);
        return;
    }

    CursorContext context;
    context.cursor = ANIMATION("select_nothing");
    context.index = 0;
    context.maxIndex = 1;
    context.minIndex = 0;
    context.source = &m_heroes[m_ui.GetCursorIndex()];
    context.state = CursorState::REWARD_HERO_ITEM;
    context.target = &m_heroes[m_ui.GetCursorIndex()];
    context.weapon = &m_currentRoom->rewardWeapon;
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
    if (m_heroes[m_heroIndex].currentMp < weapon->mpCost)
        weapon = &WEAPON("Unarmed");

    // If we can create an action already 
    if (weapon->target == Target::ALLENEMIES)
    {
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
    if (prevContext.source->currentMp < currentWeapon->mpCost)
        return;

    // Staff doesn't require a target can go strait to using item
    if (currentWeapon->target == Target::ALLPLAYERSHP)
    {
        m_ui.GetContext().weapon = currentWeapon;
        UseSelectedItem();
        return;
    }

    CursorContext context;
    context.cursor = ANIMATION("select_hero");
    context.index = 0;
    context.maxIndex = (int)m_heroes.size() - 1;
    context.minIndex = 0;
    context.source = prevContext.source;
    context.state = CursorState::USE_ITEM;
    context.target = nullptr;
    context.weapon = currentWeapon;
    context.direction = CursorIndexDirection::HORIZONTAL;

    for (int index = 0; index < (int)m_heroes.size(); index++)
    {
        if (m_heroes[index].currentHp == 0)
            context.excludeIndexes.push_back(index);
    }

    m_ui.PushBack(context);
}

void DungeonCrawl::PushShopHero()
{
    CursorContext& prevContext = m_ui.GetContext();

    CursorContext context;
    context.cursor = ANIMATION("select_hero");
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
