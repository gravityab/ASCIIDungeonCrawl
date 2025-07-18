///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Database.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Database.h"

#define WEAPON_DB_COMMON(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::NORMAL, Rarity::COMMON)] \
        = Weapon(a, DamageType::NORMAL, b, e, Rarity::COMMON, c, d, f).As(DamageType::NORMAL, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::COLD, Rarity::COMMON)] \
        = Weapon(a, DamageType::COLD, b, e, Rarity::COMMON, c, d, f).As(DamageType::COLD, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::FIRE, Rarity::COMMON)] \
        = Weapon(a, DamageType::FIRE, b, e, Rarity::COMMON, c, d, f).As(DamageType::FIRE, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::LIGHTNING, Rarity::COMMON)] \
        = Weapon(a, DamageType::LIGHTNING, b, e, Rarity::COMMON, c, d, f).As(DamageType::LIGHTNING, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::NECROTIC, Rarity::COMMON)] \
        = Weapon(a, DamageType::NECROTIC, b, e, Rarity::COMMON, c, d, f).As(DamageType::NECROTIC, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::POISON, Rarity::COMMON)] \
        = Weapon(a, DamageType::POISON, b, e, Rarity::COMMON, c, d, f).As(DamageType::POISON, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::PSYCHIC, Rarity::COMMON)] \
        = Weapon(a, DamageType::PSYCHIC, b, e, Rarity::COMMON, c, d, f).As(DamageType::PSYCHIC, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::HOLY, Rarity::COMMON)] \
        = Weapon(a, DamageType::HOLY, b, e, Rarity::COMMON, c, d, f).As(DamageType::HOLY, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::STEEL, Rarity::COMMON)] \
        = Weapon(a, DamageType::STEEL, b, e, Rarity::COMMON, c, d, f).As(DamageType::STEEL, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::DARK, Rarity::COMMON)] \
        = Weapon(a, DamageType::DARK, b, e, Rarity::COMMON, c, d, f).As(DamageType::DARK, Rarity::COMMON); \
    m_weaponDb[Weapon::ToString(a, DamageType::WATER, Rarity::COMMON)] \
        = Weapon(a, DamageType::WATER, b, e, Rarity::COMMON, c, d, f).As(DamageType::WATER, Rarity::COMMON);

#define WEAPON_DB_RARE(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::NORMAL, Rarity::RARE)] \
        = Weapon(a, DamageType::NORMAL, b, e, Rarity::RARE, c, d, f).As(DamageType::NORMAL, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::COLD, Rarity::RARE)] \
        = Weapon(a, DamageType::COLD, b, e, Rarity::RARE, c, d, f).As(DamageType::COLD, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::FIRE, Rarity::RARE)] \
        = Weapon(a, DamageType::FIRE, b, e, Rarity::RARE, c, d, f).As(DamageType::FIRE, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::LIGHTNING, Rarity::RARE)] \
        = Weapon(a, DamageType::LIGHTNING, b, e, Rarity::RARE, c, d, f).As(DamageType::LIGHTNING, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::NECROTIC, Rarity::RARE)] \
        = Weapon(a, DamageType::NECROTIC, b, e, Rarity::RARE, c, d, f).As(DamageType::NECROTIC, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::POISON, Rarity::RARE)] \
        = Weapon(a, DamageType::POISON, b, e, Rarity::RARE, c, d, f).As(DamageType::POISON, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::PSYCHIC, Rarity::RARE)] \
        = Weapon(a, DamageType::PSYCHIC, b, e, Rarity::RARE, c, d, f).As(DamageType::PSYCHIC, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::HOLY, Rarity::RARE)] \
        = Weapon(a, DamageType::HOLY, b, e, Rarity::RARE, c, d, f).As(DamageType::HOLY, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::STEEL, Rarity::RARE)] \
        = Weapon(a, DamageType::STEEL, b, e, Rarity::RARE, c, d, f).As(DamageType::STEEL, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::DARK, Rarity::RARE)] \
        = Weapon(a, DamageType::DARK, b, e, Rarity::RARE, c, d, f).As(DamageType::DARK, Rarity::RARE); \
    m_weaponDb[Weapon::ToString(a, DamageType::WATER, Rarity::RARE)] \
        = Weapon(a, DamageType::WATER, b, e, Rarity::RARE, c, d, f).As(DamageType::WATER, Rarity::RARE);

#define WEAPON_DB_EPIC(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::NORMAL, Rarity::EPIC)] \
        = Weapon(a, DamageType::NORMAL, b, e, Rarity::EPIC, c, d, f).As(DamageType::NORMAL, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::COLD, Rarity::EPIC)] \
        = Weapon(a, DamageType::COLD, b, e, Rarity::EPIC, c, d, f).As(DamageType::COLD, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::FIRE, Rarity::EPIC)] \
        = Weapon(a, DamageType::FIRE, b, e, Rarity::EPIC, c, d, f).As(DamageType::FIRE, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::LIGHTNING, Rarity::EPIC)] \
        = Weapon(a, DamageType::LIGHTNING, b, e, Rarity::EPIC, c, d, f).As(DamageType::LIGHTNING, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::NECROTIC, Rarity::EPIC)] \
        = Weapon(a, DamageType::NECROTIC, b, e, Rarity::EPIC, c, d, f).As(DamageType::NECROTIC, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::POISON, Rarity::EPIC)] \
        = Weapon(a, DamageType::POISON, b, e, Rarity::EPIC, c, d, f).As(DamageType::POISON, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::PSYCHIC, Rarity::EPIC)] \
        = Weapon(a, DamageType::PSYCHIC, b, e, Rarity::EPIC, c, d, f).As(DamageType::PSYCHIC, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::HOLY, Rarity::EPIC)] \
        = Weapon(a, DamageType::HOLY, b, e, Rarity::EPIC, c, d, f).As(DamageType::HOLY, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::STEEL, Rarity::EPIC)] \
        = Weapon(a, DamageType::STEEL, b, e, Rarity::EPIC, c, d, f).As(DamageType::STEEL, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::DARK, Rarity::EPIC)] \
        = Weapon(a, DamageType::DARK, b, e, Rarity::EPIC, c, d, f).As(DamageType::DARK, Rarity::EPIC); \
    m_weaponDb[Weapon::ToString(a, DamageType::WATER, Rarity::EPIC)] \
        = Weapon(a, DamageType::WATER, b, e, Rarity::EPIC, c, d, f).As(DamageType::WATER, Rarity::EPIC);

#define WEAPON_DB_LEGENDARY(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::NORMAL, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::NORMAL, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::NORMAL, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::COLD, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::COLD, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::COLD, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::FIRE, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::FIRE, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::FIRE, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::LIGHTNING, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::LIGHTNING, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::LIGHTNING, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::NECROTIC, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::NECROTIC, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::NECROTIC, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::POISON, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::POISON, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::POISON, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::PSYCHIC, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::PSYCHIC, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::PSYCHIC, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::HOLY, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::HOLY, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::HOLY, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::STEEL, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::STEEL, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::STEEL, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::DARK, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::DARK, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::DARK, Rarity::LEGENDARY); \
    m_weaponDb[Weapon::ToString(a, DamageType::WATER, Rarity::LEGENDARY)] \
        = Weapon(a, DamageType::WATER, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::WATER, Rarity::LEGENDARY);

#define WEAPON_DB_COMMON_HEALING(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::HEALING, Rarity::COMMON)] \
        = Weapon(a, DamageType::HEALING, b, e, Rarity::COMMON, c, d, f).As(DamageType::HEALING, Rarity::COMMON); \

#define WEAPON_DB_COMMON_PROTECT(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::PROTECT, Rarity::COMMON)] \
        = Weapon(a, DamageType::PROTECT, b, e, Rarity::COMMON, c, d, f).As(DamageType::PROTECT, Rarity::COMMON);

#define WEAPON_DB_RARE_HEALING(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::HEALING, Rarity::RARE)] \
        = Weapon(Weapon::ToString(a, Rarity::RARE), DamageType::HEALING, b, e, Rarity::RARE, c, d, f).As(DamageType::HEALING, Rarity::RARE);

#define WEAPON_DB_RARE_PROTECT(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::PROTECT, Rarity::RARE)] \
        = Weapon(Weapon::ToString(a, Rarity::RARE), DamageType::PROTECT, b, e, Rarity::RARE, c, d, f).As(DamageType::PROTECT, Rarity::RARE);

#define WEAPON_DB_EPIC_HEALING(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::HEALING, Rarity::EPIC)] \
        = Weapon(Weapon::ToString(a, Rarity::EPIC), DamageType::HEALING, b, e, Rarity::EPIC, c, d, f).As(DamageType::HEALING, Rarity::EPIC);

#define WEAPON_DB_EPIC_PROTECT(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::PROTECT, Rarity::EPIC)] \
        = Weapon(Weapon::ToString(a, Rarity::EPIC), DamageType::PROTECT, b, e, Rarity::EPIC, c, d, f).As(DamageType::PROTECT, Rarity::EPIC);

#define WEAPON_DB_LEGENDARY_HEALING(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::HEALING, Rarity::LEGENDARY)] \
        = Weapon(Weapon::ToString(a, Rarity::LEGENDARY), DamageType::HEALING, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::HEALING, Rarity::LEGENDARY);

#define WEAPON_DB_LEGENDARY_PROTECT(a, e, b, c, d, f) \
    m_weaponDb[Weapon::ToString(a, DamageType::PROTECT, Rarity::LEGENDARY)] \
        = Weapon(Weapon::ToString(a, Rarity::LEGENDARY), DamageType::PROTECT, b, e, Rarity::LEGENDARY, c, d, f).As(DamageType::PROTECT, Rarity::LEGENDARY);

#define WEAPON_TABLE(a, b) \
    m_weaponDb[Weapon::ToString(a, DamageType::NORMAL, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::COLD, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::FIRE, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::LIGHTNING, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::NECROTIC, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::POISON, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::PSYCHIC, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::HOLY, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::STEEL, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::DARK, b)], \
    m_weaponDb[Weapon::ToString(a, DamageType::WATER, b)],

#define WEAPON_TABLE_HEALING(a, b) \
    m_weaponDb[Weapon::ToString(a, DamageType::HEALING, b)],

#define WEAPON_TABLE_PROTECT(a, b) \
    m_weaponDb[Weapon::ToString(a, DamageType::PROTECT, b)],

// --------------------------------------------------------------------------------------------------------------------
void Database::Initialize()
{
    // ----------------------------------------------------------------------------------------------------------------
    m_imageDb.clear();
    {
        m_imageDb["side_tile_background_1"] = Image(
            "####################" \
            "####################" \
            "####################" \
            "####################",
            0, 0, 20, 4, 0x000B);
        m_imageDb["side_tile_background_2"] = Image(
            "####################" \
            "####################" \
            "####################" \
            "####################",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_background_3"] = Image(
            "####################" \
            "####################" \
            "####################" \
            "####################",
            0, 0, 20, 4, 0x0008);
        m_imageDb["side_tile_background_4"] = Image(
            "####################" \
            "####################" \
            "####################" \
            "####################",
            0, 0, 20, 4, 0x000C);
        m_imageDb["side_tile_background_4_right"] = Image(
            "# # # # # # \\\\# # ##" \
            "## # # # # # # # # #",
            0, 0, 20, 2, 0x0008);
        m_imageDb["side_tile_background_4_left"] = Image(
            "# # # //# # # # # ##" \
            "## # # # # # # # # #",
            0, 0, 20, 2, 0x0008);
        m_imageDb["side_tile_mountains"] = Image(
            "...../\\....../\\....." \
            "...-/##\\./\\-/##/\\..." \
            ".-/#####\\##/#/-##/\\." \
            "/#########/########\\",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_1"] = Image(
            ".((oo))ooo)...((ooo)" \
            "....(ooo)......((o)." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_2"] = Image(
            ").((oo))ooo)...((ooo" \
            ".....(ooo)......((o)" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_3"] = Image(
            "o).((oo))ooo)...((oo" \
            ").....(ooo)......((o" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_4"] = Image(
            "oo).((oo))ooo)...((o" \
            "o).....(ooo)......((" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_5"] = Image(
            "ooo).((oo))ooo)...((" \
            "(o).....(ooo)......(" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_6"] = Image(
            "(ooo).((oo))ooo)...(" \
            "((o).....(ooo)......" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_7"] = Image(
            "((ooo).((oo))ooo)..." \
            ".((o).....(ooo)....." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_8"] = Image(
            ".((ooo).((oo))ooo).." \
            "..((o).....(ooo)...." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_9"] = Image(
            "..((ooo).((oo))ooo)." \
            "...((o).....(ooo)..." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_10"] = Image(
            "...((ooo).((oo))ooo)" \
            "....((o).....(ooo).." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_11"] = Image(
            ")...((ooo).((oo))ooo" \
            ".....((o).....(ooo)." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_12"] = Image(
            "o)...((ooo).((oo))oo" \
            "......((o).....(ooo)" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_13"] = Image(
            "oo)...((ooo).((oo))o" \
            ")......((o).....(ooo" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_14"] = Image(
            "ooo)...((ooo).((oo))" \
            "o)......((o).....(oo" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_15"] = Image(
            ")ooo)...((ooo).((oo)" \
            "oo)......((o).....(o" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_16"] = Image(
            "))ooo)...((ooo).((oo" \
            "ooo)......((o).....(" \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_17"] = Image(
            "o))ooo)...((ooo).((o" \
            "(ooo)......((o)....." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_18"] = Image(
            "oo))ooo)...((ooo).((" \
            ".(ooo)......((o)...." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_19"] = Image(
            "(oo))ooo)...((ooo).(" \
            "..(ooo)......((o)..." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_tile_cloud_20"] = Image(
            "((oo))ooo)...((ooo)." \
            "...(ooo)......((o).." \
            "...................." \
            "....................",
            0, 0, 20, 4, 0x0007);
        m_imageDb["side_bird_1"] = Image(
            "-",
            0, 0, 1, 1, 0x0008);
        m_imageDb["side_bird_2"] = Image(
            "^",
            0, 0, 1, 1, 0x0008);
        m_imageDb["side_tile_tree_top_1"] = Image(
            "...................." \
            "...................." \
            "...................." \
            "....^........^.....^",
            0, 0, 20, 4, 0x0002);
        m_imageDb["side_tile_tree_top_2"] = Image(
            "...................." \
            "...................." \
            "...................." \
            ".^......^......^....",
            0, 0, 20, 4, 0x000A);
        m_imageDb["side_tile_tree_body_1"] = Image(
            "....^^......^^....^^" \
            "...^^^^....^^^...^^^" \
            "...^..^...^^..^..^.." \
            "....................",
            0, 0, 20, 4, 0x0002);
        m_imageDb["side_tile_tree_body_2"] = Image(
            "...................." \
            "...................." \
            "....||......||....||" \
            "....||......||....||",
            0, 0, 20, 4, 0x000C);
        m_imageDb["side_tile_tree_body_3"] = Image(
            ".^^.....^^.....^^..." \
            "^^^^...^^^.....^^..." \
            "^..^..^...^...^..^.." \
            "....................",
            0, 0, 20, 4, 0x000A);
        m_imageDb["side_tile_tree_body_4"] = Image(
            "...................." \
            "...................." \
            ".||.....||.....||..." \
            ".||.....||.....||...",
            0, 0, 20, 4, 0x0006);
        m_imageDb["side_tile_dungeon_right"] = Image(
            "............\\\\......" \
            "..../--------\\\\-\\..." \
            "....|           |..." \
            "....\\-----------/...",
            0, 0, 20, 4, 0x000C);
        m_imageDb["side_tile_dungeon_left"] = Image(
            "......//............" \
            ".../-//--------\\...." \
            "...|           |...." \
            "...\\-----------/....",
            0, 0, 20, 4, 0x000C);

        m_imageDb["main"] = Image(
            "##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#####%@@%##########" \
            "%@@@@%%%%%%%%%%%%%%%%%%%%%%%%%@@@%%%%%%%%%%%%%%%%%%%%%%@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@@@@%%%%%%%%%%%%@%%%%%%%%%%%%" \
            "##%%#******#%%#**************#*******#%%%%#************************##%@%%%###############%%%##*******************##%%###" \
            "**##***#######****###**###############**%#**##########################%%#***###############%#***#####**#*********##%%***" \
            "###%########%%##########################%%###########################%@%###################%#####################*#%%###" \
            "##%%%#######%%#########################%%%############################%%###################%#######################%%###" \
            "##%%%######%%%#########################%%%############################%%###################%#######################%@%##" \
            "##%%%######%@%#########################%%%############################%%##################%%%######################%%%##" \
            "##%%%######%@%#########################%%%###########################%%%##################%%#######################%@%##" \
            "##%%%%%%###%@%%%%%%%%###############%%%%@%%%%##################%%%%%%%@@%################%%%%%%%%%%%%%%%%%%%%%%%%%%@@%%%" \
            "+++*##**++**%@%#*++*******+++******+*%###*++++++++++++++*%@@@%%%%%#+++++++*##%%%%%%@@%*+++++++++++**%@%%%%%%%%%%#*++++++" \
            "##*##########%#+**###################%*+******++*######**+%%*+****+*##*****####**++%%*################++****#########***" \
            "############%%######################%%###################*#+*#############%########%#*##############%*++*###############" \
            "############%%%#####################%%###################*%########################%%##############%%%##################" \
            "############%%%######################%####################%########################%%##############%%%##################" \
            "############%@%######################%%%#################%%########################%%##############%%%##################" \
            "############%@%%##########%#########%@@%###############%%%@%######################%@@%#############%%%##################" \
            "###%########%@%#######%%%%%%##%#####%@%%%%%%%%%%%%%%%%%%%%@@%#######%%###########%%@@%%%%%########%%%%############%%%###" \
            "%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%#############%%%#################%@@@@%#****#########@@@%%%%%%%%%%%#######%%%%%%%%%%" \
            "**%##******#######***#####*****#%%#**************+*#*+++++++*#*****###%@%****************#%*+++++++******+++++++++#%%***" \
            "**%#*#%%%%%%%%%%%%%%%%%%%%%%#**#%%#####################################%%#################%*+*##################*++#*+**" \
            "##%%%%%%%%%%%%%%%%%%%%%%%%%%%##%%%####################################*%%################%%**#####################**####" \
            "##%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%################%####################%%################%%*#######################*%###" \
            "##%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%###############%####################%%################%%########################*%###" \
            "#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%###############%%##################%%%################%%#########################%###" \
            "#%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%##############%%%###############%%%%%####%%%%%%######%%%##########%%###########%%%%%" \
            "%%%@@%%%%%%%%@%@@%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%@@%%%%%%%%%%%%%%%%%%%%%%%%@@%%%" \
            "%%%%%%%%%%%%%%###################%%%%%@@@@%%%%%%###############%%@@%%%%%%%%%%%#####%%%#%@@%%%%%%%%%%%%%%%%%%%%@%%%######" \
            "****########%#*********###%#####***#%%%#%%%##################****%%**###%%%%###########*#@%%%%%###############%#######**" \
            "%%%%%%%%%%%%%##%%%%%%%%%%%%%############%%###%%%%%%%%%%%%%%%%%%%#%%##%%%%%%%%%%%%%%%%%%#%@%%%%%%%%%%%%%%%%%%%%%%%%%#####" \
            "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%###%%%%#%%%%#%%%%%%%%%%##%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
            0, 0, 120, 30, 0x0008);

        m_imageDb["passives"] = Image(
            "......................................................................................................................" \
            "......................................................................................................................" \
            "_____________________________________________________________________________________________________________________." \
            "|                                                                                                                    |" \
            "|  /--------------\\   /--------------\\   /--------------\\   /--------------\\   /--------------\\   /--------------\\   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  \\--------------/   \\--------------/   \\--------------/   \\--------------/   \\--------------/   \\--------------/   |" \
            "|  /--------------\\   /--------------\\   /--------------\\   /--------------\\   /--------------\\   /--------------\\   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  |              |   |              |   |              |   |              |   |              |   |              |   |" \
            "|  \\--------------/   \\--------------/   \\--------------/   \\--------------/   \\--------------/   \\--------------/   |" \
            "|  ________________________________________________________________________________________________________________  |" \
            "| |                                                                                                                | |" \
            "| |                                                                                                                | |" \
            "| |________________________________________________________________________________________________________________| |" \
            "|____________________________________________________________________________________________________________________|",
            0, 0, 118, 29, 0x000F);

        m_imageDb["passive_dialog_short"] = Image(
            ".________________________________________________________________________________________________." \
            "|                                                                                                |" \
            "|                                                                                                |" \
            "|________________________________________________________________________________________________|",
            0, 0, 98, 4, 0x0007);

        m_imageDb["passives_tab"] = Image(
            "..____________.." \
            "./            \\." \
            "/              \\",
            0, 0, 16, 3, 0x0008);

        m_imageDb["background"] = Image(
            "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "|..................................................................................................|" \
            "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --",
            0, 0, 100, 18, 0x0007);
        m_imageDb["background_1"] = Image(
            "...................................................................................................." \
            ".   :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+############################." \
            ".     :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+###########################." \
            ".       :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#########################." \
            ".         :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#######################." \
            ".           :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#####################." \
            ".             :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+###################." \
            ".               :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#################." \
            ".                 :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+###############." \
            ".                   :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+############." \
            ".                     :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+##########." \
            ".                       :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+########." \
            ".                         :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+######." \
            ".                           :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+####." \
            ". __ __ __ __ __ __ __ __ __ _: _ : _ : _ : _ : _ :_:_:_:_:_:_:+:+_+:+:_:+++_++++_+#+#_#+#_#+#_#+#_." \
            ".                                   :   :   :   :   :   :   :   :   :   : : : : : : : : : : :+:+:+:." \
            ".                                         :   :   :   :   :   :   :   :   :   :   : : : : : : : : :." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["background_2"] = Image(
            "...................................................................................................." \
            ".                  :   :   :   :   :   : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#############." \
            ".                :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+################." \
            ".              :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+##################." \
            ".            :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+####################." \
            ".          :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+######################." \
            ".        :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+########################." \
            ".      :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+##########################." \
            ".    :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+############################." \
            ".  :   :   :   :   :  : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+##############################." \
            ".:   :   :   :   : : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#################################." \
            ".  :   :   :   : : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+###################################." \
            ".:   :   :   : : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#####################################." \
            ".  :   :   : : : : : : : :+:+:+:+:+:++++++++#+#+#+#+#+#+#+#+#######################################." \
            ". _ : _ :_:_:_:_:_:_:+:+_+:+:_:+++_++++_+#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+#_#+." \
            ". :+:+:+: : : : : : : : : : :   :   :   :   :   :   :   :                                          ." \
            ". :+:+:+::+:+:+: : : : : : : : : :   :   :   :   :   :   :   :   :                                 ." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["trap"] = Image(
            "...................................................................................................." \
            ".:        :     --    :- _:_  : |    :  : + + +# # # ############| : _:_ -::  --  :      :        :." \
            ".:        :        :    :   :-:-|      : : : +==+==+ +# #########|-:- :    :      :      :        :." \
            ".:----    :        :    :   : : |        : +|   |   |+ # # ######| :  :    :      :      :   ---- :." \
            ".:    :   -  --- - ___  :- _: : |        +|     |     |+ +# # ###| : :__ -:: ___ - ---  -   :     :." \
            ".:    :          :    :   : --:-|        |      |      |: +# # # |-:  :   :   :    :        :     :." \
            ".:    :          :    :   : : : |        |      |      | : : +# #| :  :   :   :    :        :     :." \
            ".:    :          : __ :___:_- :-|        |    -+|+-    |   : : +#|-: -_:___: __  :          :     :." \
            ".:___ :  ------    :    :  : :  |        |    -+|+-    |     : : | : :  :    :    :  ------ : ___ :." \
            ".:        :        :    :  : : _|:       |      |      |       : |_: :  :    :    :    :          :." \
            ".:        :        :    :_ :_-  | :  :   |      |      |         | : -:_:__  :    :    :          :." \
            ".:        :        __ :   : : :_|+ :  :  |      |      |         |_:  :   :  ___  :    :          :." \
            ".:        : ___  :    :   :_--: |#+_:__:_+______|______+________ | :--_   :   :    : ___          :." \
            ".: ___ ---       :   -:_--: : _-^: :+++                :  : +####^-_ : --_:-  :    :     :    ___ :." \
            ".:    :       ___:_--  :  :_-^ : :++                       :  : +###^-_ :   --:_:___     :        :." \
            ".:    :    ---     :   :_-^: : :+                              :  : +##^-_  :     : --- _:__      :." \
            ".:    ___ :   :    : _-^ : : :                                     :  : +#^-_     :      :    __  :." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);

        m_imageDb["swinging_axe_1"] = Image(
            "...........................................oo......................................................." \
            "........................................oo.........................................................." \
            "......................................oo............................................................" \
            "...................................oo..............................................................." \
            ".................................oo................................................................." \
            ".....................-^.......oo...................................................................." \
            "..................../ / \\.-^^......................................................................." \
            "....................| \\    /........................................................................" \
            ".....................\\_^-___\\......................................................................." \
            ".......................^---__>......................................................................" \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["swinging_axe_2"] = Image(
            ".............................................o......................................................" \
            "...........................................oo......................................................." \
            "..........................................o........................................................." \
            "........................................oo.........................................................." \
            ".......................................o............................................................" \
            ".....................................oo............................................................." \
            "....................................o..............................................................." \
            "...........................^=-_...oo................................................................" \
            "...........................| | -^ |................................................................." \
            "............................\\ ^____--__............................................................." \
            "..............................=_____=..............................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["swinging_axe_3"] = Image(
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "................................................o..................................................." \
            "...............................................| |.................................................." \
            ".........................................\\^=--^   ^--=^/............................................" \
            "..........................................\\_=--___--=_/............................................." \
            "............................................^^_____^^..............................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["swinging_axe_4"] = Image(
            "...................................................o................................................" \
            "....................................................oo.............................................." \
            "......................................................o............................................." \
            ".......................................................oo..........................................." \
            ".........................................................o.........................................." \
            "..........................................................oo........................................" \
            "............................................................o......................................." \
            ".............................................................oo.._-=^..............................." \
            ".............................................................| ^- | |..............................." \
            ".........................................................__--____^ /................................" \
            "...........................................................=_____=.................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["swinging_axe_5"] = Image(
            "....................................................oo.............................................." \
            ".......................................................oo..........................................." \
            ".........................................................oo........................................." \
            "............................................................oo......................................" \
            "..............................................................oo...................................." \
            ".................................................................oo.......^-........................" \
            "....................................................................^^-./ \\ \\......................." \
            ".....................................................................\\    / |......................." \
            "..................................................................../___-^_/........................" \
            "...................................................................<__---^.........................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);

        m_imageDb["spikes_1"] = Image(
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "........................................o..o..o.o.o..o..o..........................................." \
            "......................................o.o...o...o...o...o.o........................................." \
            "....................................o...o....o..o..o....o...o......................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["spikes_2"] = Image(
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "...................................................................................................." \
            "........................................^..^..^.^.^..^..^..........................................." \
            "......................................^.|^.|^.|.|.|.^|.^|.^........................................." \
            "....................................^.|.^|.||^|.^.|^||.|^.|.^......................................." \
            "....................................|.|.||.||||.|.||||.||.|.|......................................." \
            "....................................|.|.||.||||.|.||||.||.|.|......................................." \
            "....................................|.|.||.o||o.|.o||o.||.|.|......................................." \
            "....................................|.o.|...o|..|..|o...|.o.|......................................." \
            "....................................o...o....o..o..o....o...o......................................." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);

        m_imageDb["stairs_1"] = Image(
            "...................................................................................................." \
            ".###+       :            : ___-+####-########__#############           :    -      +++#############." \
            ".##+        :         __--   :   ++#######################--|          :       -   :      +++######." \
            ".##         :      _--       :    -   +---+#######--########|         :             -          ++##." \
            ".#+         :__--:         /        ---    --+###--#####--##|\\        :                 -       +##." \
            ".#+        /     :     /       /       _      : +###########|  \\      :                       -  #_." \
            ".#      --       :   :      /  _ -            :  +---###--##|   \\    :                     :      #." \
            ".#   --/         :/  :    /  /     - :      --    -+-#######|   :\\   :                     :      #." \
            ".# -/  :        -    : /  :/ :   -   :  --     :     +#####-|   :  \\                       :      #." \
            ". /    :      /      :    :  :  |     -    --        --+####|\\  :    \\                    :       #." \
            ".      :     /     /     /   : |     -  :--    : _______ ###|:\\ :      \\                  :       #." \
            ".      :    -     |     |     |    -    : ______/ +++###----|: \\         \\                :       #." \
            ".      :  / :    /     /      |  -  :____/   ++##-------__+#|:  \\        : \\              :       #." \
            ".      : /  :   | :   | :    -  -   /     -------________   |:   \\       :    \\           :       #." \
            ".      :/   :  /  :   / :   - _ ___/___________           --|\\     \\     :       \\        :       #." \
            ".           :  |  :   | :  -   /               -------------| \\    :\\    :          \\     :       #." \
            ".     /     : /   :  /  :  -  /                             |  \\   :  \\  :              \\:         ." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);
        m_imageDb["stairs_2"] = Image(
            "...................................................................................................." \
            ".###+            :            -+############################|    :     - :         +++#############." \
            ".##+   :         :         ___   ++#-########__#############|    :          -             +++######." \
            ".##    :         :    __--   :        +###################--|   :              -               ++##." \
            ".#+    :         : _--       :    -    ---   +####--########|   :                   -          :+##." \
            ".#+    :     __--    :     /        ---    --   +--#####--##|\\  :                 :     -      : ##." \
            ".#         /:        : / :     /       _      :  +##########|  \\                  :           -   _." \
            ".#      --  :        :   :  /  _ -            :  --+-###--##|:  \\                 :               #." \
            ".#   --/    :     /      :/  /     - :      --    - -+######|:   \\                :               #." \
            ".  -/       :   -      /  :/     -   :  --     :      +####-|:     \\             :                #." \
            ". /         : /           :   - |     -    --            ##_|\\       \\           :                #." \
            ".           : /:    /:    /  /:       -  :--    : ______----| \\      :  \\        :                #." \
            ".           -  :  |  :  |  :  |    -    : _____/     ++#----|  \\     :   \\       :                #." \
            ".         /    : /   : /   :  |  -  :___/      ++-------__+#|   \\    :     \\     :                #." \
            ".        /     :|     |    : -  -  /    --------_________   |   :\\   :        \\  :                #." \
            ".       /      /      /     - _ __/__________             --|\\  :  \\ :           \\                #." \
            ".       :      |      |    -  /              ---------------| \\:    \\               \\              ." \
            "....................................................................................................",
            0, 0, 100, 18, 0x0007);

        m_imageDb["fountain"] = Image(
            "........................." \
            "........................." \
            "................____....." \
            ".............._/\\__/\\_..." \
            "..............\\_\\__/ / .." \
            "................|  |....." \
            "........_______ \\  / ____" \
            "....___/        |  |     " \
            ".../            |  |     " \
            "..|\\_                    " \
            "..|   -----______________" \
            ".._\\_                    " \
            ".....-----_______________" \
            "........................." \
            ".........................",
            0, 0, 25, 15, 0x0007);
        m_imageDb["fountain_water"] = Image(
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........;;;;;;;;....;;;;;" \
            "....;;;;;;;;;;;;....;;;;;" \
            ".....;;;;;;;;;;;;==;;;;;;" \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            ".........................",
            0, 0, 25, 15, 0x0001);
        m_imageDb["fountain_water_1"] = Image(
            "........................." \
            "..............#..#...#..." \
            "............#..#.##.#..#." \
            "...........#.#..........#" \
            "............#...####...#." \
            "..........#......##......" \
            ".............#........#.." \
            ".........^..#..^.##.#.^.#" \
            ".........#..^..#..#..#..." \
            "......^......#..##..^.#.." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            ".........................",
            0, 0, 25, 15, 0x0009);
        m_imageDb["fountain_water_2"] = Image(
            ".................==......" \
            ".............#..####..#.." \
            "...........#...#.##.#...#" \
            "...........%..#......#..%" \
            "..........#..%...%%...%.." \
            "...........#....=..=....#" \
            ".........#..#..%.##.%..#." \
            "........%..^#...#..#.^..%" \
            ".........#..^..#..#..#..^" \
            ".......^.....#..##...^#.." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            ".........................",
            0, 0, 25, 15, 0x0009);

        m_imageDb["fairy_1"] = Image(
            "....." \
            "(\\o/)" \
            "(/|\\)" \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_1"] = Image(
            "....." \
            "(\\./)" \
            "(/.\\)" \
            ".....",
            0, 0, 5, 4, 0x0009);
        m_imageDb["fairy_2"] = Image(
            "....." \
            ".|o|." \
            ".|||." \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_2"] = Image(
            "....." \
            ".|.|." \
            ".|.|." \
            ".....",
            0, 0, 5, 4, 0x0009);

        m_imageDb["fairy_3"] = Image(
            "(\\o/)" \
            "(/|\\)" \
            "....." \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_3"] = Image(
            "(\\./)" \
            "(/.\\)" \
            "....." \
            ".....",
            0, 0, 5, 4, 0x0009);
        m_imageDb["fairy_4"] = Image(
            ".|o|." \
            ".|||." \
            "....." \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_4"] = Image(
            ".|.|." \
            ".|.|." \
            "....." \
            ".....",
            0, 0, 5, 4, 0x0009);

        m_imageDb["fairy_5"] = Image(
            "....." \
            "(\\o/)" \
            "(/|\\)" \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_5"] = Image(
            "....." \
            "(\\./)" \
            "(/.\\)" \
            ".....",
            0, 0, 5, 4, 0x0009);
        m_imageDb["fairy_6"] = Image(
            "....." \
            ".|o|." \
            ".|||." \
            ".....",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_6"] = Image(
            "....." \
            ".|.|." \
            ".|.|." \
            ".....",
            0, 0, 5, 4, 0x0009);

        m_imageDb["fairy_7"] = Image(
            "....." \
            "....." \
            "(\\o/)" \
            "(/|\\)",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_7"] = Image(
            "....." \
            "....." \
            "(\\./)" \
            "(/.\\)",
            0, 0, 5, 4, 0x0009);
        m_imageDb["fairy_8"] = Image(
            "....." \
            "....." \
            ".|o|." \
            ".|||.",
            0, 0, 5, 4, 0x0007);
        m_imageDb["fairy_wings_8"] = Image(
            "....." \
            "....." \
            ".|.|." \
            ".|.|.",
            0, 0, 5, 4, 0x0009);

        m_imageDb["fairy_dust_1"] = Image(
            ",",
            0, 0, 1, 1, 0x0009);
        m_imageDb["fairy_dust_2"] = Image(
            ";",
            0, 0, 1, 1, 0x0009);

        m_imageDb["background_panel"] = Image(
            "# # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # #" \
            " # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # # " \
            "# # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # #" \
            " # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # # " \
            "# # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # #" \
            " # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # # " \
            "# # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # ## # # # # # # # # # # # #" \
            " # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # #  # # # # # # # # # # # # ",
            0, 18, 100, 8, 0x0008);
        m_imageDb["background_footer"] = Image(
            "-/--------\\-/--------\\-/--------\\-/--------\\-/--------\\-/--------\\-/--------\\-/--------\\-/--------\\-" \
            "X                                                                                                  X" \
            "X                                                                                                  X" \
            "-\\--------/-\\--------/-\\--------/-\\--------/-\\--------/-\\--------/-\\--------/-\\--------/-\\--------/-",
            0, 26, 100, 4, 0x0008);

        m_imageDb["common_door_closed"] = Image(
            "..................................." \
            "............. .==.==. ............." \
            ".......... .|    |    |. .........." \
            "........ .|      |      |. ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........" \
            "........ |     ..|..     | ........" \
            "........ |     ..|..     | ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........" \
            "........ |       |       | ........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["common_door_details_closed"] = Image(
            "..................................." \
            "..............+..+..+.............." \
            "...........+.    .    .+..........." \
            ".........+.      .      .+........." \
            "..........       .       .........." \
            "..........       .       .........." \
            "..........       .       .........." \
            "..........     -+.+-     .........." \
            "..........     -+.+-     .........." \
            "..........       .       .........." \
            "..........       .       .........." \
            "..........       .       .........." \
            "..........       .       .........." \
            "..........       .       ..........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["common_door_opened"] = Image(
            "..................................." \
            " .==. ....... .==.==. ....... .==. " \
            " |    |. . .|.........|. . .|    | " \
            " |      |.|.............|.|      | " \
            " |       |...............|       | " \
            " |       |...............|       | " \
            " |       |...............|       | " \
            " |..     |...............|     ..| " \
            " |..     |...............|     ..| " \
            " |       |...............|       | " \
            " |       |...............|       | " \
            " |       |...............|       | " \
            " |       |...............|       | " \
            " |       |...............|       | ",
            0, 0, 35, 14, 0x0008);
        m_imageDb["common_door_details_opened"] = Image(
            "..................................." \
            ".+..+.........+..+..+.........+..+." \
            ".......+...+.+:+++++++.+...+......." \
            ".........+.+:+:+:+++++++.+........." \
            "..........:+:+:+:+:++++++.........." \
            "..........:+:+:+:+:+:++++.........." \
            "..........: :+:+:+:+:+:++.........." \
            "..+-......: : :+:+:+:+:+:......-+.." \
            "..+-......: : : :+:+:+:+:......-+.." \
            ".......... : : : : :+:+:+.........." \
            ".......... : : : : : :+:+.........." \
            ".......... : : : : : : :+.........." \
            "..........   : : : : : : .........." \
            ".......... :   : : : : : ..........",
            0, 0, 35, 14, 0x0008);

        m_imageDb["rare_door_closed"] = Image(
            "..................................." \
            ".............. .=.=. .............." \
            "........... .|   |   |. ..........." \
            "......... .:     |     :. ........." \
            "......... |      |      | ........." \
            "........ :   .   |   .   : ........" \
            "........ |       |       | ........" \
            "........ :   .   |   .   : ........" \
            "........ |  . .  |  . .  | ........" \
            "........ :       |       : ........" \
            "........ |   .   |   .   | ........" \
            "........ :       |       : ........" \
            "........ |   .   |   .   | ........" \
            "........ :       |       : ........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["rare_door_details_closed"] = Image(
            "..................................." \
            "...............+.+.+..............." \
            "............+.   .   .+............" \
            "..........+.     .     .+.........." \
            "...........      .      ..........." \
            "..........   +   .   +   .........." \
            "..........       .       .........." \
            "..........   +   .   +   .........." \
            "..........  _ _  .  _ _  .........." \
            "..........       .       .........." \
            "..........   +   .   +   .........." \
            "..........       .       .........." \
            "..........   +   .   +   .........." \
            "..........       .       ..........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["rare_door_opened"] = Image(
            "..................................." \
            " .=. ......... .=.=. ......... .=. " \
            " |   |. ... .|.......|. ... .|   | " \
            " |     :. .:...........:. .:     | " \
            " |      | |.............| |      | " \
            " |   .   :...............:   .   | " \
            " |       |...............|       | " \
            " |   .   :...............:   .   | " \
            " |  . .  |...............|  . .  | " \
            " |       :...............:       | " \
            " |   .   |...............|   .   | " \
            " |       :...............:       | " \
            " |   .   |...............|   .   | " \
            " |       :...............:       | ",
            0, 0, 35, 14, 0x0008);
        m_imageDb["rare_door_details_opened"] = Image(
            "..................................." \
            ".+.+..........+..+..+..........+.+." \
            "..   .+....+.+:+++++++.+....+.   .." \
            "..     .+..+:+:+:+++++++..+.     .." \
            "..      ..:+:+:+:+:++++++..      .." \
            "..   +   .:+:+:+:+:+:++++.   +   .." \
            "..       .: :+:+:+:+:+:++.       .." \
            "..   +   .: : :+:+:+:+:+:.   +   .." \
            "..  _ _  .: : : :+:+:+:+:.  _ _  .." \
            "..       . : : : : :+:+:+.       .." \
            "..   +   . : : : : : :+:+.   +   .." \
            "..       . : : : : : : :+.       .." \
            "..   +   .   : : : : : : .   +   .." \
            "..       . :   : : : : : .       ..",
            0, 0, 35, 14, 0x0008);

        m_imageDb["epic_door_closed"] = Image(
            "............ _-^^^^^-_ ............" \
            "......... _/...\\^^^/...\\_ ........." \
            "........ -.\\.=`  |  `=./.- ........" \
            "....... |_.-     |     -._| ......." \
            "...... |..| . .  |  . . |..| ......" \
            "..... -|_-       |       -_|- ....." \
            "..... |..|  . .  |  . .  |..| ....." \
            "..... -__-      .|.      -__- ....." \
            "..... |..|  . . .|. . .  |..| ....." \
            "..... -__-      .|.      -__- ....." \
            "..... |..|  . . .|. . .  |..| ....." \
            "..... -__-       |       -__- ....." \
            "..... |..|  . .  |  . .  |..| ....." \
            "..... -__-       |       -__- .....",
            0, 0, 35, 14, 0x0008);
        m_imageDb["epic_door_details_closed"] = Image(
            "..................................." \
            "............###.....###............" \
            "..........#....  .  ....#-........." \
            "............     .     ............" \
            "........##. ` `  .  ` ` .##........" \
            "..........       .       .........." \
            ".......##.  ` `  .  ` `  .##......." \
            "..........      |.|      .........." \
            ".......##.  ` ` |.| ` `  .##......." \
            "..........      |.|      .........." \
            ".......##.  ` ` |.| ` `  .##......." \
            "..........       .       .........." \
            ".......%%.  ` `  .  ` `  .%%......." \
            "..........       .       ..........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["epic_door_opened"] = Image(
            "............._-^^^^^-_............." \
            ".^^-...._._/...\\^^^/...\\_......-^^." \
            ".|  `=...-.\\.=`.....`=./.-...=`  |." \
            ".|     -._.-...........-._.-     |." \
            ".|  . . |.|.............|.| . .  |." \
            ".|       -...............-       |." \
            ".|  . .  |...............|  . .  |." \
            ".|.      -...............-      .|." \
            ".|. ` .  |...............|  . . .|." \
            ".|.      -...............-      .|." \
            ".|. . .  |...............|  . . .|." \
            ".|       -...............-       |." \
            ".|  . .  |...............|  . .  |." \
            ".|       -...............-       |.",
            0, 0, 35, 14, 0x0008);
        m_imageDb["epic_door_details_opened"] = Image(
            "..................................." \
            "............###.....###............" \
            ".........-#....+++++....#-........." \
            "............:+:+:++++++............" \
            "....`.`....+:+:+:+:+++++....`.`...." \
            "..........:+:+:+:+:+:++++.........." \
            "....`.`...: :+:+:+:+:+:++...`.`...." \
            "..|.......: : :+:+:+:+:+:.......|.." \
            "..|.`.`...: : : :+:+:+:+:...`.`.|.." \
            "..|....... : : : : :+:+:+.......|.." \
            "..|.`.`... : : : : : :+:+...`.`.|.." \
            ".......... : : : : : : :+.........." \
            "....`.`...   : : : : : : ...`.`...." \
            ".......... :   : : : : : ..........",
            0, 0, 35, 14, 0x0008);

        m_imageDb["legendary_door_closed"] = Image(
            "......=-=-=-=-=-=-=-=-=-=-=-=......" \
            ".....|.____/...\\^^^/...\\____.|....." \
            "......\\....\\.=`  .  `=./..../......" \
            "......|____-    ...    -____|......" \
            "......|...|      .      |...|......" \
            "......-__-      ...      -__-......" \
            "......|..|       .       |..|......" \
            "......-__-    . ... .    -__-......" \
            "......|..|     . . .     |..|......" \
            "......-__-     .....     -__-......" \
            "......|..|    .  .  .    |..|......" \
            "......-__-      ...      -__-......" \
            "......|..|       .       |..|......" \
            "......-__-      ...      -__-......",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_details_closed"] = Image(
            "..................................." \
            "......#.....###.....###.....#......" \
            ".......####.#....|....#.####......." \
            "................:|:................" \
            ".......###.......|.......###......." \
            "................:|:................" \
            ".......##........|........##......." \
            "..............\\.:|:./.............." \
            ".......##......|.|.|......##......." \
            "...............|:|:|..............." \
            ".......##...../..|..\\.....##......." \
            "................:|:................" \
            ".......%%........|........%%......." \
            "................:|:................",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_spike_closed"] = Image(
            "..................................." \
            "..................................." \
            "...............^.|.^..............." \
            "............^.^..|..^.^............" \
            "...........^.^.^.|.^.^.^..........." \
            "..........^.^.^..|..^.^.^.........." \
            "...........^.^.^.|.^.^.^..........." \
            "..........^.^....|....^.^.........." \
            "...........^.^...|...^.^..........." \
            "..........^.^.^..|..^.^.^.........." \
            "...........^.^.^.|.^.^.^..........." \
            "..........^.^.^..|..^.^.^.........." \
            "...........^.^.^.|.^.^.^..........." \
            "..........______...______..........",
            0, 0, 35, 14, 0x0004);

        m_imageDb["legendary_door_opened"] = Image(
            "......=-=-=-=-=-=-=-=-=-=-=-=......" \
            ".^^\\.|#____/###\\^^^/###\\____#|./^^." \
            ".| ^`=#/###\\#=`+++++`=#/###\\#=`^ |." \
            ".|: ^ ^-___-:+:+:++++++-___-^ ^ :|." \
            ".| ^ ^ ^|#|+:+:+:+:+++++|#|^ ^ ^ |." \
            ".|: ^ ^ ^-:+:+:+:+:+:++++-^ ^ ^ :|." \
            ".| ^ ^ ^ |: :+:+:+:+:+:++| ^ ^ ^ |." \
            ".|: / ^ ^-: : :+:+:+:+:+:-^ ^ \\ :|." \
            ".| | ^ ^ |: : : :+:+:+:+:| ^ ^ | |." \
            ".|:|^ ^ ^- : : : : :+:+:+-^ ^ ^|:|." \
            ".| ^\\^ ^ | : : : : : :+:+| ^ ^/^ |." \
            ".|: ^ ^ ^- : : : : : : :+-^ ^ ^ :|." \
            ".| ^ ^ ^ |   : : : : : : | ^ ^ ^ |." \
            ".|: ^ ^ ^- :   : : : : : -^ ^ ^ :|.",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_details_opened"] = Image(
            "..................................." \
            "......#.....###.....###.....#......" \
            "......#.###.#..+++++..#.###.#......" \
            "..:.........:+:+:++++++.........:.." \
            ".........#.+:+:+:+:+++++.#........." \
            "..:.......:+:+:+:+:+:++++-......:.." \
            "..........: :+:+:+:+:+:++.........." \
            "..:./.....: : :+:+:+:+:+:.....\\.:.." \
            "...|......: : : :+:+:+:+:......|..." \
            "..:|...... : : : : :+:+:+......|:.." \
            "....\\..... : : : : : :+:+...../...." \
            "..:....... : : : : : : :+.......:.." \
            "..........   : : : : : : .........." \
            "..:....... :   : : : : : .......:..",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_spike_opened"] = Image(
            "..................................." \
            "..................................." \
            "...^...........................^..." \
            "....^.^.....................^.^...." \
            "...^.^.^...................^.^.^..." \
            "....^.^.^.................^.^.^...." \
            "...^.^.^...................^.^.^..." \
            "......^.^.................^.^......" \
            ".....^.^...................^.^....." \
            "....^.^.^.................^.^.^...." \
            "...^.^.^...................^.^.^..." \
            "....^.^.^.................^.^.^...." \
            "...^.^.^...................^.^.^..." \
            "...______.................______...",
            0, 0, 35, 14, 0x0004);

        m_imageDb["legendary_door_smoke_1"] = Image(
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "......%......................%....." \
            ".........%.^%..%%%%%..%^.%........." \
            "........%..^%%..%.%..%%^...%........",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_smoke_2"] = Image(
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "....%...%..%.^.%%.%%.^.%..%...%...." \
            "......%...%.%%..%.%..%%.%...%......",
            0, 0, 35, 14, 0x0008);
        m_imageDb["legendary_door_smoke_3"] = Image(
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "..................................." \
            "......%.^%%%.^^%%.%%^^.%%%^........" \
            ".....%....%^.%..%.%..%.^%...%......",
            0, 0, 35, 14, 0x0008);

        m_imageDb["torch_base"] = Image("[]| ", 0, 1, 2, 2, 0x0007);
        m_imageDb["torch_fire_1"] = Image(".%", 0, 0, 2, 1, 0x0004);
        m_imageDb["torch_fire_2"] = Image(".^", 0, 0, 2, 1, 0x0004);
        m_imageDb["torch_fire_3"] = Image("%.", 0, 0, 2, 1, 0x0004);
        m_imageDb["torch_fire_4"] = Image("^.", 0, 0, 2, 1, 0x0004);

        m_imageDb["fist_attack_1"] = Image(
            ".................." \
            ".................." \
            "........ ........." \
            "....... | ........" \
            "..... - o - ......" \
            "....... | ........" \
            "........ ........." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["fist_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........... ......" \
            ".......... | ....." \
            "........ - o - ..." \
            ".......... | ....." \
            "........... ......" \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["fist_attack_3"] = Image(
            ".................." \
            ".................." \
            ".... ............." \
            "... | ............" \
            ". - o - .........." \
            "... | ............" \
            ".... ............." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["fist_attack_4"] = Image(
            ".................." \
            "............. ...." \
            "............ | ..." \
            ".......... - o - ." \
            "............ | ..." \
            "............. ...." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["fist_attack_5"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "......... ........" \
            "........ | ......." \
            "...... - o - ....." \
            "........ | ......." \
            "......... ........" \
            "..................",
            0, 0, 18, 10, 0x0007);

        m_imageDb["blast_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........ob........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_2"] = Image(
            ".................." \
            ".................." \
            "........;........." \
            ".......;t;;......." \
            ".....;;obo;;;....." \
            ".......;t;........" \
            "........;........." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_3"] = Image(
            ".................." \
            ".................." \
            "........b........." \
            ".......ttoo......." \
            ".....totootoo....." \
            ".......otot......." \
            "........o........." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_4"] = Image(
            ".................." \
            ".................." \
            ".......;bt;......." \
            "......;ttoo;......" \
            "....;totootoo;...." \
            ".....;tototo;....." \
            ".......;ot;......." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_5"] = Image(
            ".................." \
            ".................." \
            ".......obtt......." \
            "......ottooo......" \
            "....btotootoot...." \
            ".....otototoo....." \
            ".......otoo......." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_6"] = Image(
            ".................." \
            "........;;........" \
            "......;oobt;......" \
            ".....;otoboo;....." \
            "...;oootootooo;..." \
            "....;otoootoo;...." \
            "......;otoo;......" \
            "........;;......." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_7"] = Image(
            ".................." \
            "........ot........" \
            "......tototo......" \
            ".....toboobot....." \
            "...ooototoootoo..." \
            "....totooootto...." \
            "......o7oo7o......" \
            "........oo........" \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_8"] = Image(
            ".................." \
            "........ot........" \
            ".....o..to..o....." \
            "......oboobo......" \
            "...ooototoootoo..." \
            "...ototoooottot..." \
            "......o7oo7o......" \
            ".....t..oo..o....." \
            "........uu........" \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_9"] = Image(
            "........ot........" \
            "...oo...oo...ot..." \
            "....to..ot..oo...." \
            ".....ooboobot....." \
            ".ooootoootooootoo." \
            ".ttottootootttoto." \
            ".....to7oo7oo....." \
            "....to..oo..ot...." \
            "...ot...oo...to..." \
            "........uu........",
            0, 0, 18, 10, 0x0007);
        m_imageDb["blast_attack_10"] = Image(
            "........--........" \
            "........oo........" \
            ".....o..oo..o....." \
            "......ob;;bo......" \
            "...oooo;..;oooo..." \
            "...tttt;..;tttt..." \
            "......o7;;7o......" \
            ".....o..oo..o....." \
            "........uu........" \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_1"] = Image(
            "............ / ..." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_2"] = Image(
            "............ / ..." \
            "........... / ...." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_3"] = Image(
            "............ / ..." \
            "........... / ...." \
            ".......... / ....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_4"] = Image(
            "............ / ..." \
            "........... / ...." \
            ".......... / ....." \
            "......... / ......" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_5"] = Image(
            ".................." \
            "........... / ...." \
            ".......... / ....." \
            "......... / ......" \
            "........ / ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_6"] = Image(
            ".................." \
            ".................." \
            ".......... / ....." \
            "......... / ......" \
            "........ / ......." \
            "....... / ........" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_7"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "......... / ......" \
            "........ / ......." \
            "....... / ........" \
            "...... / ........." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_8"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........ / ......." \
            "....... / ........" \
            "...... / ........." \
            "..... / .........." \
            ".................." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_9"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....... / ........" \
            "...... / ........." \
            "..... / .........." \
            ".... / ..........." \
            "..................",
            0, 0, 18, 10, 0x0007);
        m_imageDb["slash_attack_10"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... / ........." \
            "..... / .........." \
            ".... / ..........." \
            "... / ............",
            0, 0, 18, 10, 0x0007);
        m_imageDb["weapon_border"] = Image(
            "/--------------\\" \
            "|              |" \
            "|              |" \
            "|              |" \
            "|              |" \
            "|              |" \
            "|              |" \
            "|              |" \
            "|              |" \
            "\\--------------/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["rare_1"] = Image(
            "................" \
            "................" \
            "..........^....." \
            "................" \
            "..........%....." \
            ".......%........" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["rare_2"] = Image(
            "................" \
            "................" \
            "..........^....." \
            "................" \
            "......%........." \
            ".........%......" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["rare_3"] = Image(
            "................" \
            "................" \
            "...........%...." \
            "........^......." \
            "..........%....." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["rare_4"] = Image(
            "................" \
            "................" \
            ".........%......" \
            ".......^........" \
            "........^......." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["epic_1"] = Image(
            "................" \
            "..........i....." \
            "........--o--..." \
            "..........i....." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["epic_2"] = Image(
            "................" \
            "................" \
            "................" \
            "......|........." \
            "....--o--......." \
            "......|........." \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["epic_3"] = Image(
            "................" \
            "................" \
            "...........|...." \
            ".........--o--.." \
            "...........|...." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["epic_4"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["legendary_1"] = Image(
            "......####......" \
            "...##......##..." \
            ".#............#." \
            "#..............#" \
            "#..............#" \
            "#..............#" \
            "#..............#" \
            ".#............#." \
            "...##......##..." \
            "......####......",
            0, 0, 16, 10, 0x0007);
        m_imageDb["legendary_2"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["legendary_3"] = Image(
            "................" \
            "......%%%%......" \
            "...%%......%%..." \
            "..%..........%.." \
            ".%............%." \
            ".%............%." \
            "..%..........%.." \
            "...%%......%%..." \
            "......%%%%......" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["legendary_4"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["robe"] = Image(
            "................" \
            "................" \
            "......./\\......." \
            "....._|^.|......" \
            "..._/..||..\\\\..." \
            "...\\/..||..|\\..." \
            "..../..||..\\...." \
            ".../..|..\\..|..." \
            "...|^^____^^|..." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["robe_lining"] = Image(
            "................" \
            "................" \
            "................" \
            ".......^z......." \
            "................" \
            "................" \
            "................" \
            ".......zz......." \
            "....^^____^^...." \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["leather"] = Image(
            "................" \
            "................" \
            "................" \
            "......_.._......" \
            "..../##||##\\...." \
            "....}##||##{...." \
            ".....|#||#|....." \
            "..../##||##|...." \
            "....^^/..\\^^...." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["leather_lining"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            ".....##||##....." \
            ".....##||##....." \
            "......#||#......" \
            ".....##||##....." \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["plate"] = Image(
            "................" \
            "................" \
            "................" \
            ".......__......." \
            "....(.,)(,.)...." \
            "....}(.)(.){...." \
            ".....(.)(.)....." \
            ".....^^--^^....." \
            ".....######....." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["plate_lining"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "......,..,......" \
            "................" \
            "................" \
            ".....^^..^^....." \
            ".....######....." \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["gloves"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            ".......||||....." \
            ".....|.........." \
            ".....^....}....." \
            "......|__|......" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["knuckles"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            ".......^^^^....." \
            "......####......" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["wand_handle"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "......yy........" \
            ".....||........." \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["wand"] = Image(
            "................" \
            "................" \
            ".........//....." \
            "........//......" \
            "........||......" \
            ".......//......." \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["staff_handle"] = Image(
            "................" \
            "................" \
            "..........//...." \
            ".........//....." \
            "........//......" \
            ".......//......." \
            "......//........" \
            ".....//........." \
            "....\\^.........." \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["staff"] = Image(
            "...........__..." \
            "..........|##;.." \
            "............^..." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["dagger_hilt"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            ".....//........." \
            "....\\/.........." \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["dagger_blade"] = Image(
            "................" \
            "................" \
            "........._......" \
            "........//......" \
            ".......//......." \
            "....._//........" \
            ".......\\........" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["sword_hilt"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "....-//........." \
            "....\\/.........." \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["sword_blade"] = Image(
            "................" \
            ".........._....." \
            ".........//....." \
            "........//......" \
            ".....-.//......." \
            ".....\\//........" \
            ".......\\-......." \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["great_sword_hilt"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "....//.........." \
            "...\\/..........." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["great_sword_blade"] = Image(
            "..........._...." \
            "..........//...." \
            ".........//....." \
            "........//......" \
            ".......//......." \
            "......//........" \
            "...-\\//........." \
            "......\\-........" \
            "................" \
            "................",
            0, 0, 16, 10, 0x000C);
        m_imageDb["lesser_potion_bottle"] = Image(
            "................" \
            ".              ." \
            ".              ." \
            ".      |-|     ." \
            ".      / \\     ." \
            ".     /^^^\\    ." \
            ".    |     |   ." \
            ".     \\___/    ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["lesser_healing_potion"] = Image(
            "................" \
            ".              ." \
            ".              ." \
            ".      ...     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".     .....    ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0004);
        m_imageDb["lesser_magic_potion"] = Image(
            "................" \
            ".              ." \
            ".              ." \
            ".      ...     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".     .....    ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0001);
        m_imageDb["lesser_resurrect_potion"] = Image(
            "................" \
            ".              ." \
            ".              ." \
            ".      ...     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".     .....    ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x000F);
        m_imageDb["potion_bottle"] = Image(
            "................" \
            ".       _      ." \
            ".      | |     ." \
            ".      | |     ." \
            ".     /^^^\\    ." \
            ".    /     \\   ." \
            ".   |       |  ." \
            ".    \\_____/   ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["healing_potion"] = Image(
            "................" \
            ".       .      ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".   .#######.  ." \
            ".    .......   ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0004);
        m_imageDb["magic_potion"] = Image(
            "................" \
            ".       .      ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".   .#######.  ." \
            ".    .......   ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x0001);
        m_imageDb["resurrect_potion"] = Image(
            "................" \
            ".       .      ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .^^^.    ." \
            ".    .#####.   ." \
            ".   .#######.  ." \
            ".    .......   ." \
            ".              ." \
            "................",
            0, 0, 16, 10, 0x000F);
        m_imageDb["greater_potion_bottle"] = Image(
            "................" \
            ".      |-|     ." \
            ".      | |     ." \
            ".      | |     ." \
            ".     /   \\    ." \
            ".    /^^^^^\\   ." \
            ".   /       \\  ." \
            ".  /         \\ ." \
            ".  \\_________/ ." \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["greater_healing_potion"] = Image(
            "................" \
            ".      ...     ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .   .    ." \
            ".    .^^^^^.   ." \
            ".   .#######.  ." \
            ".  .#########. ." \
            ".  ........... ." \
            "................",
            0, 0, 16, 10, 0x0004);
        m_imageDb["greater_magic_potion"] = Image(
            "................" \
            ".      ...     ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .   .    ." \
            ".    .^^^^^.   ." \
            ".   .#######.  ." \
            ".  .#########. ." \
            ".  ........... ." \
            "................",
            0, 0, 16, 10, 0x0001);
        m_imageDb["greater_resurrect_potion"] = Image(
            "................" \
            ".      ...     ." \
            ".      . .     ." \
            ".      . .     ." \
            ".     .   .    ." \
            ".    .^^^^^.   ." \
            ".   .#######.  ." \
            ".  .#########. ." \
            ".  ........... ." \
            "................",
            0, 0, 16, 10, 0x000F);
        m_imageDb["hero_card"] = Image(
            "................" \
            ".# # # # # # # ." \
            ". # # # # # # #." \
            ".# # # # # # # ." \
            ". # # # # # # #." \
            ".# # # # # # # ." \
            ". # # # # # # #." \
            ".# # # # # # # ." \
            ". # # #        ." \
            "................",
            0, 0, 16, 10, 0x0008);
        m_imageDb["new_hero"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "... NEW HERO ..." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);
        m_imageDb["level_up"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "... LEVEL UP ..." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["level_up_5"] = Image(
            "................" \
            "................" \
            "................" \
            "... LEVEL UP...." \
            "..... ,___ ....." \
            "..... |__  ....." \
            "..... ,__) ....." \
            "......    ......" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0005);

        m_imageDb["exit"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "...   EXIT   ..." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0007);

        m_imageDb["slime_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".....__---___....." \
            "...-^         ^-.." \
            "../  |      |  |.." \
            ".|   :      :   \\." \
            "/      -__-      |" \
            "\\-_--____-___-__-/" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x000B);
        m_imageDb["slime_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".....         ...." \
            "...  |      |  ..." \
            "..   :      :   .." \
            ".      -__-      ." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["slime_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....__---___......" \
            "..-^        ^-...." \
            "../  |      |  |.." \
            ".|   :      :   \\." \
            "/      -__-      |" \
            "\\-_--____-___-__-/" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x000B);
        m_imageDb["slime_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....         ....." \
            "...  |      |  ..." \
            "..   :      :   .." \
            ".      -__-      ." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["slime_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....__---___......" \
            "..-^        ^-...." \
            "./  |      |  |..." \
            ".|  :      :   \\.." \
            "/     -__-      |." \
            "\\-_--____-___-__-/" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x000B);
        m_imageDb["slime_eyes_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....        ......" \
            "..  |      |  ...." \
            "..  :      :   ..." \
            ".     -__-      .." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["slime_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".....__---___....." \
            "...-^        ^-..." \
            "../            |.." \
            ".|              \\." \
            "/                |" \
            "\\-_--____-___-__-/" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x000B);
        m_imageDb["slime_attack_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".....        ....." \
            "...  \\      /  ..." \
            "..   |      |   .." \
            ".      ____      ." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["slime_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......____......." \
            "....._^    ^_....." \
            ".../^        ^^..." \
            "._| /        \\ \\_." \
            "\\    ^     ^    /." \
            ".|     ^     ^  |." \
            ".|  \\        / |.." \
            "..|       ^    |.." \
            "..\\-_-____-__-/...",
            0, 0, 18, 15, 0x000B);
        m_imageDb["slime_attack_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......    ......." \
            ".....        ....." \
            "... .        . ..." \
            ".    .     .    .." \
            "..     .     .  .." \
            "..  .        . ..." \
            "...  |    . |  ..." \
            "..................",
            0, 0, 18, 15, 0x000B);

        m_imageDb["spawn_skeleton_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "......... \\/ ....." \
            "....... -  \\\\ ...." \
            "..... (o,o)// ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spawn_skeleton_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......o,o........" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["spawn_skeleton_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o) ....." \
            ".... --_|=|_ ....." \
            "... //.:=|() \\\\ .." \
            "... \\\\.(_=|| // .." \
            "... /\\# ..'==/\\ .." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spawn_skeleton_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........=........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["spawn_skeleton_3"] = Image(
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o)......" \
            ".... -__|=|__- ..." \
            "... //.:=|=: \\\\ .." \
            "... \\\\..\\|/ .// .." \
            "... /\\.:=|=: /\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spawn_skeleton_eyes_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........=........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["spawn_skeleton_4"] = Image(
            "........ - ......." \
            "...... (o,o) ....." \
            "....... |=| ......" \
            "...... __|__ ....." \
            ".... //:=|=:\\\\ ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\.:=|=:.// .." \
            "... /\\.(_=_)./\\ .." \
            "...... :|.|: ....." \
            "...... ().() ....." \
            "...... ||.|| ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spawn_skeleton_eyes_4"] = Image(
            ".................." \
            "........o,o......." \
            ".........-........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["skeleton_1"] = Image(
            "........ - ......." \
            "...... (o,o) ....." \
            "....... |=| ......" \
            "...... __|__ ....." \
            ".... //:=|=:\\\\ ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\.:=|=:.// .." \
            "... /\\.(_=_)./\\ .." \
            "...... :|.|: ....." \
            "...... ().() ....." \
            "...... ||.|| ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["skeleton_eyes_1"] = Image(
            ".................." \
            "........o,o......." \
            ".........-........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["skeleton_2"] = Image(
            "........ - ......." \
            "...... (o,o) ....." \
            "....... |=| ......" \
            "..... -__|__- ...." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\.:=|=:.// .." \
            "... /\\.:=|=:./\\ .." \
            "...... (_=_) ....." \
            "...... //.\\\\ ....." \
            "...... ().() ....." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["skeleton_eyes_2"] = Image(
            ".................." \
            "........o,o......." \
            ".........-........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["skeleton_attack_1"] = Image(
            ".................." \
            ".................." \
            "... \\/ . - ......." \
            "... //.(o,o) ....." \
            "... \\\\-_|=|__- ..." \
            "...... :=|=:.\\\\ .." \
            "....... \\|/..// .." \
            "...... :=|=:./\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["skeleton_eyes_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........=........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["skeleton_attack_2"] = Image(
            ".................." \
            ".................." \
            "........ - . \\/ .." \
            "...... (o,o).\\\\ .." \
            ".... -__|=|_-// .." \
            "... //.:=|=: ....." \
            "... \\\\..\\|/ ......" \
            "... /\\.:=|=: ....." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["skeleton_eyes_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........=........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);


        m_imageDb["alt_skeleton_attack_1"] = Image(
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o) ....." \
            ".....-__|u|__- ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\..\\|/..// .." \
            "... /\\.:=|=:./\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_skeleton_eyes_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........u........" \
            ".................." \
            ".........#........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["alt_skeleton_attack_2"] = Image(
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o) ....." \
            ".....-__|u|__- ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\..\\|/..// .." \
            "... /\\.:=|=:./\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_skeleton_eyes_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........u........" \
            ".................." \
            ".........#........" \
            ".........#........" \
            ".........%........" \
            ".........%........" \
            "........%#%......." \
            ".........^........" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["alt_skeleton_attack_3"] = Image(
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o) ....." \
            ".....-__|u|__- ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\..\\|/..// .." \
            "... /\\.:=|=:./\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_skeleton_eyes_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            ".........u........" \
            ".................." \
            ".........#........" \
            ".........%........" \
            ".........#........" \
            ".....%.#.#.#.%...." \
            "......%.###.%....." \
            ".......#%#%#......" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["alt_skeleton_attack_4"] = Image(
            ".................." \
            ".................." \
            "........ - ......." \
            "...... (o,o) ....." \
            ".....-__|u|__- ..." \
            "... //.:=|=:.\\\\ .." \
            "... \\\\..\\|/..// .." \
            "... /\\.:=|=:./\\ .." \
            "...... //.\\\\ ....." \
            "..... ()...() ...." \
            "...... \\\\.// ....." \
            "..... =='.'== ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_skeleton_eyes_attack_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "........o,o......." \
            "..#......u......#." \
            "...#.%.......#.%.." \
            "..%.%.#.#.#.#.#.%." \
            "...#.#.#.#.#.%.#.." \
            "..#.%.#.%.#.#.#.#." \
            "...#.#.#.#.#.%.#.." \
            "....#.%.###.%....." \
            "...#%#.#%#%#.#%#.." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);


        m_imageDb["blob_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _-__ ......" \
            "..... /.  .\\ ....." \
            "... |   ..   |...." \
            "... \\________/ ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            "........__........" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..... _-__ ......." \
            ".... / .  .\\ ....." \
            "... |   ..  | ...." \
            ".... \\______/ ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            "........__........" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....... _-__......" \
            "..... /.  . \\ ...." \
            ".... |  ..   | ..." \
            ".... \\______/ ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            "........__........" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _-__ ......" \
            ".... / .... \\ ...." \
            ".... \\______/ ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O__O......." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _-__ ......" \
            "..... /.  .\\ ....." \
            ".... |      | ...." \
            ".... |  ..  | ...." \
            "..... \\____/ ....." \
            "......      ......" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            ".................." \
            "........__........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..... __-___ ....." \
            ".... |      | ...." \
            "... | .    . | ..." \
            ".. |   ....   | .." \
            "... \\________/ ..." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "......O....O......" \
            ".......____......." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["blob_attack_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..... __-___ ....." \
            "... /        \\ ..." \
            ".. |  ......  | .." \
            ".. \\__________/ .." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["blob_eyes_attack_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "......O____O......" \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);

        m_imageDb["alt_blob_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _-__ ......" \
            "..... /O  O\\ ....." \
            "... |   __   |...." \
            "... \\________/ ...." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_blob_eyes_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            "........__........" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["alt_blob_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "......._-__......." \
            ".... /  __  \\ ...." \
            "... /  /vv\\  \\ ..." \
            ".. |   |  |   | .." \
            ".. \\   \\__/   / .." \
            "... \\________/ ..." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_blob_eyes_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O__o......." \
            "......./vv\\......." \
            ".......|  |......." \
            ".......\\__/......." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);
        m_imageDb["alt_blob_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..... __-___ ....." \
            ".... / O  O \\ ...." \
            ".. |          | .." \
            ".. \\  ______  / .." \
            "... \\________/ ..." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_blob_eyes_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".......O..O......." \
            ".................." \
            "......______......" \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0008);


        m_imageDb["spider_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\.. __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " / / |      | \\ \\ " \
            ". / . \\____/ . \\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_1"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\.. __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " \\ /.|      |.\\ / " \
            ".. \\ .\\____/. / .." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_2"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\ . __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " / /.|      |.\\ \\ " \
            ". /. .\\____/. .\\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_3"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\ . __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " \\ /.|      |.\\./." \
            ".. \\ .\\____/. / .." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_4"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["spider_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\ . __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " / /.|      |.\\ \\ " \
            ". / . \\_.._/ ..\\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_attack_1"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........##........" \
            "........##........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["spider_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\....... /\\ .." \
            ". / ../    \\.. \\ ." \
            ".....|      |....." \
            ". / ..\\_.._/ . \\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_attack_2"] = Image(
            ".........|........" \
            ".........#........" \
            "....#....#...#...." \
            ".....#...#..#....." \
            "......#.##.#......" \
            ".....#.#..#.#....." \
            ".####...##...####." \
            "....#...##...#...." \
            ".....#.#..#.#....." \
            "......#.##.#......" \
            ".....#...#..#....." \
            "....#....#...#...." \
            ".........#........" \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["spider_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. .\\ . .. . /. .." \
            ". / ../    \\.. \\ ." \
            ".....|      |....." \
            ". / . \\_.._/ . \\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["spider_eyes_attack_3"] = Image(
            "..#....####....#.." \
            "#..#.##..#.##.#..#" \
            "..#.#....#...#.#.." \
            ".#...#...#..#...#." \
            "#..#..#.##.#..#..#" \
            "#....#.#..#.#....#" \
            "#####...##...#####" \
            "#...#.._##_..#...#" \
            "#..#.#.#..#.#.#..#" \
            ".#....#.##.#....#." \
            "#.##.#...#..#..#.#" \
            "....##..###..##..." \
            "...#..##.#.##.#..." \
            "..#......#.....#.." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["alt_spider_attack_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\.. __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            " \\ /.|      |.\\ / " \
            ".. \\ .\\____/. / .." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_spider_eyes_attack_1"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_spider_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\.. __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            ". \\ /|      |\\ / ." \
            "... \\ \\____/ / ..." \
            ".... \\ .... / ...." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_spider_eyes_attack_2"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".......oooo......." \
            "......./vv\\......." \
            ".......|  |......." \
            ".......\\__/......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_spider_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".. /\\.. __ . /\\ .." \
            ". / .\\/    \\/. \\ ." \
            ". \\ /|      |\\ / ." \
            "... \\ \\____/ / ..." \
            ".... \\ .... / ...." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_spider_eyes_attack_3"] = Image(
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".........|........" \
            ".................." \
            ".......oooo......." \
            "........__........" \
            "........vv........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["bat_1"] = Image(
            ".................." \
            ".................." \
            "....... _  __ ...." \
            "...... |( /_( ...." \
            "...... \\,,|\\( ...." \
            "....... **`/ ....." \
            "....... ;; ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........**........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "..... _ ... __ ..." \
            ".... |(_,,_/_( ..." \
            ".... `-.**-`` ...." \
            "....... ;; ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........**........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _,,__ ....." \
            "..... /,**-,\\ ...." \
            "..... \\(;; )/ ...." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_eyes_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........**........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "....... ,,_ ......" \
            "...... |**,\\ ....." \
            "...... \\;;|( ....." \
            "....... \\(\\( ....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_eyes_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........**........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);

        m_imageDb["bat_attack_1"] = Image(
            ".................." \
            ".................." \
            "....... _..__ ...." \
            "...... |(./_( ...." \
            "...... \\,,|\\( ...." \
            "....... **`/ ....." \
            "....... ;; ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_attack_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".....(......)....." \
            "....(........)...." \
            "...(....**....)..." \
            "....(........)...." \
            ".....(......)....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            "..... _.... __ ..." \
            ".... |(_,,_/_( ..." \
            ".... `-.**-`` ...." \
            "....... ;; ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_attack_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".....(......)....." \
            "....(........)...." \
            "...(..........)..." \
            "..(.....**.....).." \
            "...(..........)..." \
            "....(........)...." \
            ".....(......)....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "...... _,,__ ....." \
            "..... /,**-,\\ ...." \
            "..... \\(;;.)/ ...." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_attack_eyes_3"] = Image(
            ".................." \
            ".....(......)....." \
            "....(........)...." \
            "...(..........)..." \
            "..(............).." \
            ".(......**......)." \
            "..(............).." \
            "...(..........)..." \
            "....(........)...." \
            ".....(......)....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["bat_attack_4"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........,,_ ......" \
            "...... |**,\\ ....." \
            "...... \\;;|( ....." \
            "....... \\(\\( ....." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["bat_attack_eyes_4"] = Image(
            ".....(......)....." \
            "....(........)...." \
            "...(..........)..." \
            "..(............).." \
            ".(..............)." \
            "(.......**.......)" \
            ".(. ............)." \
            "..(............).." \
            "...(..........)..." \
            "....(........)...." \
            ".....{......)....." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);

        m_imageDb["alt_bat_attack_1"] = Image(
            ".................." \
            "....... _..__ ...." \
            "...... |(./_( ...." \
            "...... \\,,|\\( ...." \
            "....... **`/ ....." \
            "....... ;; ......." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_bat_attack_eyes_1"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "........**........" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);

        m_imageDb["alt_bat_attack_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "... _............." \
            ". /_/ ............" \
            ". ) \\ ............" \
            "{  ; |} .........." \
            ". )_/ ............" \
            ". \\_\\ ............" \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_bat_attack_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "-................." \
            "-................." \
            "-................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);
        m_imageDb["alt_bat_attack_3"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".............. _.." \
            "............ /_/ ." \
            "............ ) \\ ." \
            "...........{  ; |}" \
            "............ )_/ ." \
            "............ \\_\\ ." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);
        m_imageDb["alt_bat_attack_eyes_2"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "---............---" \
            "---\\........../---" \
            "---\\----------/---" \
            "...\\----------/..." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0004);

        m_imageDb["empty"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["dragon_1"] = Image(
            ".......................................................____________......" \
            ".................................(`-,,________,,,,---''  ____,,,_,-`....." \
            "..................................\\\\`,_______,,_,,---'''     ,'.........." \
            "..................................; )`,      __,,-'`-,      /............" \
            "................................./ /     _,-' _,,;;,_ `-,_,'............." \
            "................................/ /   ,-' _,-'  //   ``--,_``,_.........." \
            "..............................,','_,-' ,-' _,- (( =-    -, `-,_`-,_____.." \
            "............................,;,''__,,-'   _,,--,\\\\,--'````--,,_``-,`-,_`," \
            "............._..........|\\,' ,-''        ```-'`---'`-,,,__,,_  ``-,`-,`-," \
            ".._....._,-,'(__)\\__)\\-'' `     ___  ,          `     \\      `--,_......." \
            ",',)---' /|)          `     `      ``-,   `     /     /     `     `-,...." \
            "\\_____--,  '`  `               __,,-,  \\     , (   < _,,,-----,,,_   `,.." \
            ".\\_,--,,__, \\\\ ,-`,\\----'';``,,,-,__ \\  \\      ,`_, `,,-'`--'`---''`,  )." \
            ".............,\\`,\\  `_,-,,' ,'   _,-,,'  /,,,-''(, ,' ; ( _______`___,,'." \
            "...................((,(,__(....((,(,__,'..``'--.`'`,(\\  `,,,,______ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_1"] = Image(
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........./|)............................................................." \
            "._____--,................................................................" \
            "........................................................................." \
            "........................................................................." \
            "...................((,(,..(....((.(,..,..................................",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_2"] = Image(
            ".......................................................____________......" \
            ".................................(`-,,________,,,,---''  ____,,,_,-`....." \
            "..................................\\\\`,_______,,_,,---'''     ,'.........." \
            "..................................; )`,      __,,-'`-,      /............" \
            "................................./ /     _,-' _,,;;,_ `-,_,'............." \
            "................................/ /   ,-' _,-'  //   ``--,_``,_.........." \
            "..............................,','_,-' ,-' _,- (( =-    -, `-,_`-,_____.." \
            "............................,;,''__,,-'   _,,--,\\\\,--'````--,,_``-,`-,_`," \
            "....................._..|\\,' ,-''        ```-'`---'`-,,,__,,_  ``-,`-,`-," \
            ".........._...._,-,,`(_)-'' `   ___  ,          `     \\      `--,_......." \
            "........,,)-' /%)     `     `      ``-,   `     /     /     `     `-,...." \
            "........\\___-,  '`  `          __,,-,  \\     , (   < _,,,-----,,,_   `,.." \
            ".........\\,-,,_, \\\\ ,---'';``,,,-,__ \\  \\      ,`_, `,,-'`--'`---''`,  )." \
            ".............,\\`,\\  `_,-,,' ,'   _,-,,'  /,,,-''(, ,' ; ( _______`___,,'." \
            "...................((,(,__(....((,(,__,'..``'--.`'`,(\\  `,,,,______ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_2"] = Image(
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "............../%)........................................................" \
            ".........___-,..........................................................." \
            "........................................................................." \
            "........................................................................." \
            "...................((,(,..(....((,(,..,..................................",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_3"] = Image(
            "........................................................................." \
            ".......................................................____________......" \
            "..................................(`-,,________,,,,---''  ____,,,_,-`...." \
            "...................................\\\\`,_______,,_,,---'''     ,'........." \
            "................................../ / `,      __,,-'`-,     /............" \
            ".................................//       _,-' _,,;;,_ `-,,'............." \
            "...............................,`'_,-',-' _,-'  //   ``--,_``,_.........." \
            "............................,;,''__,,- ,-' _,- (( =-    -, `-,_`-,_____.." \
            "................./|......|\\' ,-''     '   _,,--,\\\\,--'````--,,_``-,`-,_`," \
            ".................`,'(__)-'' `   ___  ,          `     \\      `--,_......." \
            "..................|%    %|  `      ``-,   `     /     /     `     `-,...." \
            "................./   __   \\    __,,-   \\     , (   < _,,,-----,,,_   `,.." \
            ".................\\\\ ^__^ /;``,,,-,__-,  \\      ,`_, `,,-'`--'`---''`,  \\." \
            "..................._`__-_,' ,'   _,-,,' /,,,,-''(, ,' ;..(   ____`___,.,'" \
            "...................((^^,__(....((,(,__,'..``'--.`'`..,(\\ `,,,,_____ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_3"] = Image(
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "...................%....%................................................" \
            "........................................................................." \
            "........................................................................." \
            "....................`__`................................................." \
            "...................((..,__(....((,(,..,..................................",
            0, 0, 73, 15, 0x0007);



        m_imageDb["dragon_attack_1"] = Image(
            "........................................................................." \
            ".......................................................____________......" \
            "..................................(`-,,________,,,,---''  ____,,,_,-`...." \
            "...................................\\\\`,_______,,_,,---'''     ,'........." \
            "................................../ / `,      __,,-'`-,     /............" \
            ".................................//       _,-' _,,;;,_ `-,,'............." \
            "...............................,`'_,-',-' _,-'  //   ``--,_``,_.........." \
            "............................,;,''__,,- ,-' _,- (( =-    -, `-,_`-,_____.." \
            "...................'n__n`.\\' ,-''     '   _,,--,\\\\,--'````--,,_``-,`-,_`," \
            ".................../,__,\\/  `   ___  ,          `     \\      `--,_......." \
            ".................;\\v    v/; `      ``-,   `     /     /     `     `-,...." \
            "................./v  __  v\\    __,,-   \\     , (   < _,,,-----,,,_   `,.." \
            ".................\\^ :  : ^;``,,,-,__-,  \\      ,`_, `,,-'`--'`---''`,  \\." \
            "..................\\^`__`^/' ,'   _,-,,' /,,,,-''(, ,' ;..(   ____`___,.,'" \
            "...................(\\^^/__(....((,(,__,'..``'--.`'`..,(\\ `,,,,_____ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_attack_1"] = Image(
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "....................,__,................................................." \
            "...................v    v................................................" \
            "..................v  __  v..............................................." \
            "..................^ :  : ^..............................................." \
            "...................^`__`^...............................................'" \
            "...................(.^^.__(....((,(,..,..................................",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_attack_2"] = Image(
            "........................................................................." \
            ".......................................................____________......" \
            "..................................(`-,,________,,,,---''  ____,,,_,-`...." \
            "...................................\\\\`,_______,,_,,---'''     ,'........." \
            "................................../ / `,      __,,-'`-,     /............" \
            ".................................//       _,-' _,,;;,_ `-,,'............." \
            "...............................,`'_,-',-' _,-'  //   ``--,_``,_.........." \
            "............................,;,''__,,- ,-' _,- (( =-    -, `-,_`-,_____.." \
            "...................'n__n`.\\' ,-''     '   _,,--,\\\\,--'````--,,_``-,`-,_`," \
            ".................../,__,\\/  `   ___  ,          `     \\      `--,_......." \
            ".................;\\v    v/; `      ``-,   `     /     /     `     `-,...." \
            "................./v  __  v\\    __,,-   \\     , (   < _,,,-----,,,_   `,.." \
            ".................\\^ #### ^;``,,,-,__-,  \\      ,`_, `,,-'`--'`---''`,  \\." \
            "..................\\^####^/' ,'   _,-,,' /,,,,-''(, ,' ;..(   ____`___,.,'" \
            "...................(\\^^/__(....((,(,__,'..``'--.`'`..,(\\ `,,,,_____ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_attack_2"] = Image(
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "........................................................................." \
            "....................,__,................................................." \
            "...................v    v................................................" \
            "..................v  __  v..............................................." \
            "..................^ ####. ^..............................................." \
            "...................^####^...............................................'" \
            "...................(.^^/__(....((,(,..,..................................",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_attack_3"] = Image(
            "....................     ................................................" \
            ".............   #############   .......................____________......" \
            "...........  ##%%%%%%%%%%%%%%%##  (`-,,________,,,,---''  ____,,,_,-`...." \
            ".........  ##%%%%%%%%%%%%%%%%%%%##  \\`,_______,,_,,---'''     ,'........." \
            "........  #%%%%%%%%%%%%%%%%%%%%%%%#   `,      __,,-'`-,     /............" \
            ".......  #%%%%%%%%%%%%%%%%%%%%%%%%%#      _,-' _,,;;,_ `-,,'............." \
            "......  #%%%%%%%%%%%%%%%%%%%%%%%%%%%#  -' _,-'  //   ``--,_``,_.........." \
            "......  #%%%%%%%%%%%%&&&%%%%%%%%%%%%#   -' _,- (( =-    -, `-,_`-,_____.." \
            "......  #%%%%%%%%%%%&&&&&%%%%%%%%%%%#     _,,--,\\\\,--'````--,,_``-,`-,_`," \
            "......  ##%%%%%%%%%%%&&&%%%%%%%%%%%##           `     \\      `--,_......." \
            ".......  #%%%%%%%%%%%%%%%%%%%%%%%%%#  ,   `     /     /     `     `-,...." \
            "........  #%%%%%%%%%%%%%%%%%%%%%%%#    \\     , (   < _,,,-----,,,_   `,.." \
            ".........  ##%%%%%%%%%%%%%%%%%%%##  -,  \\      ,`_, `,,-'`--'`---''`,  \\." \
            "...........  ###%%%%%%%%%%%%%###  ,-,,' /,,,,-''(, ,' ;..(   ____`___,.,'" \
            "..............  ##%%%%%%%%%##  ((,(,__,'..``'--.`'`..,(\\ `,,,,_____ ___==",
            0, 0, 73, 15, 0x0007);
        m_imageDb["dragon_eyes_attack_3"] = Image(
            "........................................................................." \
            "................#############............................................" \
            ".............##%%%%%%%%%%%%%%%##........................................." \
            "...........##%%%%%%%%%%%%%%%%%%%##......................................." \
            "..........#%%%%%%%%%%%%%%%%%%%%%%%#......................................" \
            ".........#%%%%%%%%%%%%%%%%%%%%%%%%%#....................................." \
            "........#%%%%%%%%%%%%%%%%%%%%%%%%%%%#...................................." \
            "........#%%%%%%%%%%%%&&&%%%%%%%%%%%%#...................................." \
            "........#%%%%%%%%%%%&&&&&%%%%%%%%%%%#...................................." \
            "........##%%%%%%%%%%%&&&%%%%%%%%%%%##...................................." \
            ".........#%%%%%%%%%%%%%%%%%%%%%%%%%#....................................." \
            "..........#%%%%%%%%%%%%%%%%%%%%%%%#......................................" \
            "...........##%%%%%%%%%%%%%%%%%%%##......................................." \
            ".............###%%%%%%%%%%%%%###........................................." \
            "................##%%%%%%%%%##..((,(,_.,..................................",
            0, 0, 73, 15, 0x0007);

        m_imageDb["empty"] = Image(
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            ".................." \
            "..................",
            0, 0, 18, 15, 0x0007);

        m_imageDb["select_nothing"] = Image(
            ".",
            0, 0, 1, 1, 0x0007);
        m_imageDb["select_door"] = Image(
            "/-------------\\" \
            "|             |" \
            "\\-------------/",
            0, 0, 15, 3, 0x0007);
        m_imageDb["select_door_1"] = Image(
            "/--............" \
            "|.............." \
            "...............",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_2"] = Image(
            "/----------...." \
            "|.............." \
            "\\----..........",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_3"] = Image(
            "/-------------\\" \
            "|.............." \
            "\\----------....",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_4"] = Image(
            "/-------------\\" \
            "|.............|" \
            "\\-------------/",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_5"] = Image(
            "...-----------\\" \
            "..............|" \
            "\\-------------/",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_6"] = Image(
            "...........---\\" \
            "..............|" \
            "......--------/",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_7"] = Image(
            "..............." \
            "..............|" \
            "...........---/",
            0, 0, 15, 3, 0x0008);
        m_imageDb["select_door_8"] = Image(
            "..............." \
            "..............." \
            "...............",
            0, 0, 15, 3, 0x0008);

        m_imageDb["select_weapon"] = Image(
            "/--------------\\" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "\\--------------/",
            0, 0, 16, 10, 0x0007);
        m_imageDb["select_weapon_1"] = Image(
            "/----..........." \
            "|..............." \
            "|..............." \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_2"] = Image(
            "/-----------...." \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_3"] = Image(
            "/--------------\\" \
            "|..............|" \
            "|..............|" \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "|..............." \
            "\\----...........",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_4"] = Image(
            "/--------------\\" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............." \
            "|..............." \
            "\\----------.....",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_5"] = Image(
            "/--------------\\" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "\\--------------/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_6"] = Image(
            "....-----------\\" \
            "...............|" \
            "...............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "|..............|" \
            "\\--------------/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_7"] = Image(
            "...........----\\" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            "|..............|" \
            "|..............|" \
            "\\--------------/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_8"] = Image(
            "................" \
            "................" \
            "................" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            "...............|" \
            ".....----------/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_9"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "...............|" \
            "...............|" \
            "...........----/",
            0, 0, 16, 10, 0x0008);
        m_imageDb["select_weapon_10"] = Image(
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................" \
            "................",
            0, 0, 16, 10, 0x0008);

        m_imageDb["menu"] = Image(
            "/---------------\\" \
            "|               |" \
            "|               |" \
            "|               |" \
            "|               |" \
            "|               |" \
            "|               |" \
            "|               |" \
            "\\--------------/",
            0, 0, 17, 9, 0x0007);

        m_imageDb["menu_restart_dialog"] = Image(
            "/------------------------------\\" \
            "|                              |" \
            "|  You will lose all progress  |" \
            "|  Are you sure?               |" \
            "|                              |" \
            "|                              |" \
            "|                              |" \
            "\\------------------------------/",
            0, 0, 32, 8, 0x0007);

        m_imageDb["hero_border"] = Image(
            "/-----------------------\\" \
            "|                       |" \
            "|                       |" \
            "|                       |" \
            "|                       |" \
            "|                       |" \
            "|                       |" \
            "\\-----------------------/",
            0, 0, 25, 8, 0x0007);
        m_imageDb["hero_protect"] = Image(
            "/-+-+-+-+-+-+-+-+-+-+-+-\\" \
            "+                       +" \
            "|                       |" \
            "+                       +" \
            "|                       |" \
            "+                       +" \
            "|                       |" \
            "\\-+-+-+-+-+-+-+-+-+-+-+-/",
            0, 0, 25, 8, 0x000C);
        m_imageDb["select_hero_1"] = Image(
            "/----...................." \
            "|........................" \
            "|........................" \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            ".........................",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_2"] = Image(
            "/----------.............." \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "........................." \
            ".........................",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_3"] = Image(
            "/------------------......" \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "\\----....................",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_4"] = Image(
            "/-----------------------\\" \
            "|.......................|" \
            "|.......................|" \
            "|........................" \
            "|........................" \
            "|........................" \
            "|........................" \
            "\\------------............",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_5"] = Image(
            "/-----------------------\\" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|........................" \
            "|........................" \
            "\\-------------------.....",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_6"] = Image(
            "/-----------------------\\" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "\\-----------------------/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_7"] = Image(
            ".....-------------------\\" \
            "........................|" \
            "........................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "|.......................|" \
            "\\-----------------------/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_8"] = Image(
            "..........--------------\\" \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            "|.......................|" \
            "\\-----------------------/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_9"] = Image(
            "...................-----\\" \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            ".....-------------------/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_10"] = Image(
            "........................." \
            "........................." \
            "........................." \
            "........................|" \
            "........................|" \
            "........................|" \
            "........................|" \
            ".............-----------/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_11"] = Image(
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................|" \
            "........................|" \
            "....................----/",
            0, 0, 25, 8, 0x0008);
        m_imageDb["select_hero_12"] = Image(
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            "........................." \
            ".........................",
            0, 0, 25, 8, 0x0008);

        m_imageDb["hero_dead"] = Image(
            "........................." \
            ".         #####         ." \
            ".        #  #  #        ." \
            ".        ###^###        ." \
            ".         #####         ." \
            ".         #^#^#         ." \
            ".         -^-^-         ." \
            "..........#####..........",
            0, 0, 25, 8, 0x0004);
        m_imageDb["hero_level"] = Image(
            "........................." \
            ".                       ." \
            ".                       ." \
            ".    L E V E L  U P     ." \
            ".                       ." \
            ".                       ." \
            ".                       ." \
            ".........................",
            0, 0, 25, 8, 0x0006);

        m_imageDb["chest_opened"] = Image(
            "... _______________ ..." \
            ". /                 \\ ." \
            " /  _______________  \\ " \
            ". \\_  ___________  _/ ." \
            ". /                 \\ ." \
            " / _________________ \\ " \
            " |        |_|        | " \
            " |-------------------| " \
            " |                   | " \
            " |-------------------| " \
            " +-------------------+ ",
            0, 0, 23, 11, 0x0007);
        m_imageDb["chest_opened_gold"] = Image(
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......_#@$$*#$@$_......" \
            "..._*#$$*&#$@*$&@($_..." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            ".......................",
            0, 0, 23, 11, 0x0006);

        m_imageDb["chest_closed"] = Image(
            "......................." \
            "......................." \
            "... _______________ ..." \
            ".. _  ___________  _ .." \
            ". / --------------- \\ ." \
            " / _________________ \\ " \
            " |        |_|        | " \
            " |-------------------| " \
            " |                   | " \
            " |-------------------| " \
            " +-------------------+ ",
            0, 0, 23, 11, 0x0007);
        m_imageDb["chest_closed_gold"] = Image(
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            "......................." \
            ".......................",
            0, 0, 23, 11, 0x0006);
    }

    // ----------------------------------------------------------------------------------------------------------------
    m_frameDb.clear();
    {
        m_frameDb["background_1"] = Frame({ m_imageDb["background"], m_imageDb["background_1"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["background_2"] = Frame({ m_imageDb["background"], m_imageDb["background_2"], m_imageDb["background_panel"], m_imageDb["background_footer"], });

        m_frameDb["side_tile_0_1"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_1"] });
        m_frameDb["side_tile_0_2"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_2"] });
        m_frameDb["side_tile_0_3"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_3"] });
        m_frameDb["side_tile_0_4"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_4"] });
        m_frameDb["side_tile_0_5"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_5"] });
        m_frameDb["side_tile_0_6"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_6"] });
        m_frameDb["side_tile_0_7"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_7"] });
        m_frameDb["side_tile_0_8"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_8"] });
        m_frameDb["side_tile_0_9"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_9"] });
        m_frameDb["side_tile_0_10"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_10"] });
        m_frameDb["side_tile_0_11"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_11"] });
        m_frameDb["side_tile_0_12"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_12"] });
        m_frameDb["side_tile_0_13"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_13"] });
        m_frameDb["side_tile_0_14"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_14"] });
        m_frameDb["side_tile_0_15"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_15"] });
        m_frameDb["side_tile_0_16"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_16"] });
        m_frameDb["side_tile_0_17"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_17"] });
        m_frameDb["side_tile_0_18"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_18"] });
        m_frameDb["side_tile_0_19"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_19"] });
        m_frameDb["side_tile_0_20"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_20"] });
        m_frameDb["side_tile_0_20"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_cloud_20"] });
        m_frameDb["side_tile_1"] = Frame({ m_imageDb["side_tile_background_1"], m_imageDb["side_tile_mountains"], m_imageDb["side_tile_tree_top_1"], m_imageDb["side_tile_tree_top_2"] });
        m_frameDb["side_tile_2"] = Frame({ m_imageDb["side_tile_background_2"], m_imageDb["side_tile_tree_body_1"], m_imageDb["side_tile_tree_body_2"], m_imageDb["side_tile_tree_body_3"], m_imageDb["side_tile_tree_body_4"] });
        m_frameDb["side_tile_3_right"] = Frame({ m_imageDb["side_tile_background_3"], m_imageDb["side_tile_dungeon_right"] });
        m_frameDb["side_tile_3_left"] = Frame({ m_imageDb["side_tile_background_3"], m_imageDb["side_tile_dungeon_left"] });
        m_frameDb["side_bird_1"] = Frame({ m_imageDb["side_bird_1"], });
        m_frameDb["side_bird_2"] = Frame({ m_imageDb["side_bird_2"], });


        /*
        m_frameDb["side_tree_1"] = Frame({ m_imageDb["side_tree_leaves_1"], m_imageDb["side_tree_tree_1"], });
        m_frameDb["side_tree_2"] = Frame({ m_imageDb["side_tree_leaves_2"], m_imageDb["side_tree_tree_2"], });
        m_frameDb["side_cloud_1"] = Frame({ m_imageDb["side_cloud_1"], });
        m_frameDb["side_cloud_2"] = Frame({ m_imageDb["side_cloud_2"], });
        m_frameDb["side_cloud_3"] = Frame({ m_imageDb["side_cloud_3"], });
        m_frameDb["side_cloud_4"] = Frame({ m_imageDb["side_cloud_4"], });
        m_frameDb["side_cloud_5"] = Frame({ m_imageDb["side_cloud_5"], });
        m_frameDb["side_cloud_6"] = Frame({ m_imageDb["side_cloud_6"], });
        m_frameDb["side_cloud_7"] = Frame({ m_imageDb["side_cloud_7"], });
        m_frameDb["side_cloud_8"] = Frame({ m_imageDb["side_cloud_8"], });
        m_frameDb["side_cloud_9"] = Frame({ m_imageDb["side_cloud_9"], });
        m_frameDb["side_cloud_10"] = Frame({ m_imageDb["side_cloud_10"], });
        m_frameDb["side_cloud_11"] = Frame({ m_imageDb["side_cloud_11"], });
        m_frameDb["side_cloud_12"] = Frame({ m_imageDb["side_cloud_12"], });
        m_frameDb["side_cloud_13"] = Frame({ m_imageDb["side_cloud_13"], });
        m_frameDb["side_cloud_14"] = Frame({ m_imageDb["side_cloud_14"], });
        m_frameDb["side_cloud_15"] = Frame({ m_imageDb["side_cloud_15"], });
        m_frameDb["side_cloud_16"] = Frame({ m_imageDb["side_cloud_16"], });
        m_frameDb["side_cloud_17"] = Frame({ m_imageDb["side_cloud_17"], });
        m_frameDb["side_cloud_18"] = Frame({ m_imageDb["side_cloud_18"], });
        m_frameDb["side_cloud_19"] = Frame({ m_imageDb["side_cloud_19"], });
        m_frameDb["side_cloud_20"] = Frame({ m_imageDb["side_cloud_20"], });
        */

        m_frameDb["trap"] = Frame({ m_imageDb["background"], m_imageDb["trap"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["spikes_1"] = Frame({ m_imageDb["background"], m_imageDb["spikes_1"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["spikes_2"] = Frame({ m_imageDb["background"], m_imageDb["spikes_2"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_1"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_1"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_2"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_1"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_3"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_2"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_4"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_3"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_5"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_4"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_6"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_5"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_7"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_5"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_8"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_4"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_9"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_3"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["swinging_axe_10"] = Frame({ m_imageDb["background"], m_imageDb["swinging_axe_2"], m_imageDb["background_panel"], m_imageDb["background_footer"], });

        m_frameDb["stairs_1"] = Frame({ m_imageDb["background"], m_imageDb["stairs_1"], m_imageDb["background_panel"], m_imageDb["background_footer"], });
        m_frameDb["stairs_2"] = Frame({ m_imageDb["background"], m_imageDb["stairs_2"], m_imageDb["background_panel"], m_imageDb["background_footer"], });

        m_frameDb["fountain_1"] = Frame({ m_imageDb["fountain"], m_imageDb["fountain_water"], m_imageDb["fountain_water_1"] });
        m_frameDb["fountain_2"] = Frame({ m_imageDb["fountain"], m_imageDb["fountain_water"], m_imageDb["fountain_water_2"] });

        m_frameDb["fairy_1"] = Frame({ m_imageDb["fairy_1"], m_imageDb["fairy_wings_1"] });
        m_frameDb["fairy_2"] = Frame({ m_imageDb["fairy_2"], m_imageDb["fairy_wings_2"] });

        m_frameDb["fairy_dust_1"] = Frame({ m_imageDb["fairy_dust_1"] });
        m_frameDb["fairy_dust_2"] = Frame({ m_imageDb["fairy_dust_2"] });

        m_frameDb["common_door_opened"] = Frame({ m_imageDb["common_door_opened"], m_imageDb["common_door_details_opened"] });
        m_frameDb["common_door_closed"] = Frame({ m_imageDb["common_door_closed"], m_imageDb["common_door_details_closed"] });
        m_frameDb["rare_door_opened"] = Frame({ m_imageDb["rare_door_opened"], m_imageDb["rare_door_details_opened"] });
        m_frameDb["rare_door_closed"] = Frame({ m_imageDb["rare_door_closed"], m_imageDb["rare_door_details_closed"] });
        m_frameDb["epic_door_opened"] = Frame({ m_imageDb["epic_door_opened"], m_imageDb["epic_door_details_opened"] });
        m_frameDb["epic_door_closed"] = Frame({ m_imageDb["epic_door_closed"], m_imageDb["epic_door_details_closed"] });
        m_frameDb["legendary_door_opened_1"] = Frame({ m_imageDb["legendary_door_opened"], m_imageDb["legendary_door_details_opened"], m_imageDb["legendary_door_spike_opened"], m_imageDb["legendary_door_smoke_1"] });
        m_frameDb["legendary_door_opened_2"] = Frame({ m_imageDb["legendary_door_opened"], m_imageDb["legendary_door_details_opened"], m_imageDb["legendary_door_spike_opened"], m_imageDb["legendary_door_smoke_2"] });
        m_frameDb["legendary_door_opened_3"] = Frame({ m_imageDb["legendary_door_opened"], m_imageDb["legendary_door_details_opened"], m_imageDb["legendary_door_spike_opened"], m_imageDb["legendary_door_smoke_3"] });
        m_frameDb["legendary_door_closed_1"] = Frame({ m_imageDb["legendary_door_closed"], m_imageDb["legendary_door_details_closed"], m_imageDb["legendary_door_spike_closed"], m_imageDb["legendary_door_smoke_1"] });
        m_frameDb["legendary_door_closed_2"] = Frame({ m_imageDb["legendary_door_closed"], m_imageDb["legendary_door_details_closed"], m_imageDb["legendary_door_spike_closed"], m_imageDb["legendary_door_smoke_2"] });
        m_frameDb["legendary_door_closed_3"] = Frame({ m_imageDb["legendary_door_closed"], m_imageDb["legendary_door_details_closed"], m_imageDb["legendary_door_spike_closed"], m_imageDb["legendary_door_smoke_3"] });

        m_frameDb["select_nothing"] = Frame({ m_imageDb["select_nothing"], m_imageDb["select_nothing"] });

        m_frameDb["select_door_1"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_1"], });
        m_frameDb["select_door_2"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_2"], });
        m_frameDb["select_door_3"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_3"], });
        m_frameDb["select_door_4"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_4"], });
        m_frameDb["select_door_5"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_5"], });
        m_frameDb["select_door_6"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_6"], });
        m_frameDb["select_door_7"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_7"], });
        m_frameDb["select_door_8"] = Frame({ m_imageDb["select_door"], m_imageDb["select_door_8"], });

        m_frameDb["select_weapon_1"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_1"], });
        m_frameDb["select_weapon_2"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_2"], });
        m_frameDb["select_weapon_3"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_3"], });
        m_frameDb["select_weapon_4"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_4"], });
        m_frameDb["select_weapon_5"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_5"], });
        m_frameDb["select_weapon_6"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_6"], });
        m_frameDb["select_weapon_7"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_7"], });
        m_frameDb["select_weapon_8"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_8"], });
        m_frameDb["select_weapon_9"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_9"], });
        m_frameDb["select_weapon_10"] = Frame({ m_imageDb["select_weapon"], m_imageDb["select_weapon_10"], });

        m_frameDb["select_hero_1"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_1"], });
        m_frameDb["select_hero_2"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_2"], });
        m_frameDb["select_hero_3"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_3"], });
        m_frameDb["select_hero_4"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_4"], });
        m_frameDb["select_hero_5"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_5"], });
        m_frameDb["select_hero_6"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_6"], });
        m_frameDb["select_hero_7"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_7"], });
        m_frameDb["select_hero_8"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_8"], });
        m_frameDb["select_hero_9"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_9"], });
        m_frameDb["select_hero_10"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_10"], });
        m_frameDb["select_hero_11"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_11"], });
        m_frameDb["select_hero_12"] = Frame({ m_imageDb["hero_border"], m_imageDb["select_hero_12"], });

        m_frameDb["hero_dead"] = Frame({ m_imageDb["hero_dead"] });
        m_frameDb["hero_level"] = Frame({ m_imageDb["hero_level"] });
        m_frameDb["hero_border"] = Frame({ m_imageDb["hero_border"] });
        m_frameDb["hero_protect"] = Frame({ m_imageDb["hero_protect"] });

        m_frameDb["chest_closed"] = Frame({ m_imageDb["chest_closed"], m_imageDb["chest_closed_gold"] });
        m_frameDb["chest_opened"] = Frame({ m_imageDb["chest_opened"], m_imageDb["chest_opened_gold"] });

        m_frameDb["torch_1"] = Frame({ m_imageDb["torch_base"], m_imageDb["torch_fire_1"], });
        m_frameDb["torch_2"] = Frame({ m_imageDb["torch_base"], m_imageDb["torch_fire_2"], });
        m_frameDb["torch_3"] = Frame({ m_imageDb["torch_base"], m_imageDb["torch_fire_3"], });
        m_frameDb["torch_4"] = Frame({ m_imageDb["torch_base"], m_imageDb["torch_fire_4"], });

        m_frameDb["fist_attack_1"] = Frame({ m_imageDb["fist_attack_1"] });
        m_frameDb["fist_attack_2"] = Frame({ m_imageDb["fist_attack_1"] });
        m_frameDb["fist_attack_3"] = Frame({ m_imageDb["fist_attack_2"] });
        m_frameDb["fist_attack_4"] = Frame({ m_imageDb["fist_attack_2"] });
        m_frameDb["fist_attack_5"] = Frame({ m_imageDb["fist_attack_3"] });
        m_frameDb["fist_attack_6"] = Frame({ m_imageDb["fist_attack_3"] });
        m_frameDb["fist_attack_7"] = Frame({ m_imageDb["fist_attack_4"] });
        m_frameDb["fist_attack_8"] = Frame({ m_imageDb["fist_attack_4"] });
        m_frameDb["fist_attack_9"] = Frame({ m_imageDb["fist_attack_5"] });
        m_frameDb["fist_attack_10"] = Frame({ m_imageDb["fist_attack_5"] });

        m_frameDb["blast_attack_1"] = Frame({ m_imageDb["blast_attack_1"] });
        m_frameDb["blast_attack_2"] = Frame({ m_imageDb["blast_attack_1"] });
        m_frameDb["blast_attack_3"] = Frame({ m_imageDb["blast_attack_3"] });
        m_frameDb["blast_attack_4"] = Frame({ m_imageDb["blast_attack_3"] });
        m_frameDb["blast_attack_5"] = Frame({ m_imageDb["blast_attack_5"] });
        m_frameDb["blast_attack_6"] = Frame({ m_imageDb["blast_attack_5"] });
        m_frameDb["blast_attack_7"] = Frame({ m_imageDb["blast_attack_7"] });
        m_frameDb["blast_attack_8"] = Frame({ m_imageDb["blast_attack_7"] });
        m_frameDb["blast_attack_9"] = Frame({ m_imageDb["blast_attack_9"] });
        m_frameDb["blast_attack_10"] = Frame({ m_imageDb["blast_attack_10"] });

        m_frameDb["slash_attack_1"] = Frame({ m_imageDb["slash_attack_1"] });
        m_frameDb["slash_attack_2"] = Frame({ m_imageDb["slash_attack_1"] });
        m_frameDb["slash_attack_3"] = Frame({ m_imageDb["slash_attack_1"] });
        m_frameDb["slash_attack_4"] = Frame({ m_imageDb["slash_attack_4"] });
        m_frameDb["slash_attack_5"] = Frame({ m_imageDb["slash_attack_6"] });
        m_frameDb["slash_attack_6"] = Frame({ m_imageDb["slash_attack_8"] });
        m_frameDb["slash_attack_7"] = Frame({ m_imageDb["slash_attack_10"] });
        m_frameDb["slash_attack_8"] = Frame({ m_imageDb["slash_attack_10"] });
        m_frameDb["slash_attack_9"] = Frame({ m_imageDb["slash_attack_10"] });
        m_frameDb["slash_attack_10"] = Frame({ m_imageDb["slash_attack_10"] });

        m_frameDb["slime_1"] = Frame({ m_imageDb["slime_1"], m_imageDb["slime_eyes_1"] });
        m_frameDb["slime_2"] = Frame({ m_imageDb["slime_2"], m_imageDb["slime_eyes_2"] });
        m_frameDb["slime_3"] = Frame({ m_imageDb["slime_3"], m_imageDb["slime_eyes_3"] });
        m_frameDb["slime_4"] = Frame({ m_imageDb["slime_2"], m_imageDb["slime_eyes_2"] });
        m_frameDb["slime_attack_1"] = Frame({ m_imageDb["slime_attack_1"], m_imageDb["slime_attack_eyes_1"] });
        m_frameDb["slime_attack_2"] = Frame({ m_imageDb["slime_attack_1"], m_imageDb["slime_attack_eyes_1"] });
        m_frameDb["slime_attack_3"] = Frame({ m_imageDb["slime_attack_2"], m_imageDb["slime_attack_eyes_2"] });

        m_frameDb["blob_1"] = Frame({ m_imageDb["blob_1"], m_imageDb["blob_eyes_1"] });
        m_frameDb["blob_2"] = Frame({ m_imageDb["blob_2"], m_imageDb["blob_eyes_2"] });
        m_frameDb["blob_3"] = Frame({ m_imageDb["blob_3"], m_imageDb["blob_eyes_3"] });
        m_frameDb["blob_4"] = Frame({ m_imageDb["blob_2"], m_imageDb["blob_eyes_2"] });
        m_frameDb["blob_attack_1"] = Frame({ m_imageDb["blob_attack_1"], m_imageDb["blob_eyes_attack_1"] });
        m_frameDb["blob_attack_2"] = Frame({ m_imageDb["blob_attack_1"], m_imageDb["blob_eyes_attack_1"] });
        m_frameDb["blob_attack_3"] = Frame({ m_imageDb["blob_attack_2"], m_imageDb["blob_eyes_attack_2"] });
        m_frameDb["blob_attack_4"] = Frame({ m_imageDb["blob_attack_3"], m_imageDb["blob_eyes_attack_3"] });
        m_frameDb["blob_attack_5"] = Frame({ m_imageDb["blob_attack_4"], m_imageDb["blob_eyes_attack_4"] });
        m_frameDb["blob_attack_6"] = Frame({ m_imageDb["blob_attack_3"], m_imageDb["blob_eyes_attack_3"] });
        m_frameDb["blob_attack_7"] = Frame({ m_imageDb["blob_attack_3"], m_imageDb["blob_eyes_attack_3"] });
        m_frameDb["blob_attack_8"] = Frame({ m_imageDb["blob_attack_4"], m_imageDb["blob_eyes_attack_4"] });
        m_frameDb["blob_attack_9"] = Frame({ m_imageDb["blob_attack_3"], m_imageDb["blob_eyes_attack_3"] });
        m_frameDb["blob_attack_10"] = Frame({ m_imageDb["blob_attack_2"], m_imageDb["blob_eyes_attack_2"] });
        m_frameDb["blob_attack_11"] = Frame({ m_imageDb["blob_attack_1"], m_imageDb["blob_eyes_attack_1"] });
        m_frameDb["blob_attack_12"] = Frame({ m_imageDb["blob_attack_1"], m_imageDb["blob_eyes_attack_1"] });
        m_frameDb["alt_blob_attack_1"] = Frame({ m_imageDb["alt_blob_attack_1"], m_imageDb["alt_blob_eyes_attack_1"] });
        m_frameDb["alt_blob_attack_2"] = Frame({ m_imageDb["alt_blob_attack_2"], m_imageDb["alt_blob_eyes_attack_2"] });
        m_frameDb["alt_blob_attack_3"] = Frame({ m_imageDb["alt_blob_attack_2"], m_imageDb["alt_blob_eyes_attack_2"] });
        m_frameDb["alt_blob_attack_4"] = Frame({ m_imageDb["alt_blob_attack_3"], m_imageDb["alt_blob_eyes_attack_3"] });
        m_frameDb["alt_blob_attack_5"] = Frame({ m_imageDb["alt_blob_attack_1"], m_imageDb["alt_blob_eyes_attack_1"] });

        m_frameDb["spider_1"] = Frame({ m_imageDb["spider_1"], m_imageDb["spider_eyes_1"] });
        m_frameDb["spider_2"] = Frame({ m_imageDb["spider_2"], m_imageDb["spider_eyes_2"] });
        m_frameDb["spider_3"] = Frame({ m_imageDb["spider_3"], m_imageDb["spider_eyes_3"] });
        m_frameDb["spider_4"] = Frame({ m_imageDb["spider_4"], m_imageDb["spider_eyes_4"] });
        m_frameDb["spider_5"] = Frame({ m_imageDb["spider_3"], m_imageDb["spider_eyes_3"] });
        m_frameDb["spider_6"] = Frame({ m_imageDb["spider_2"], m_imageDb["spider_eyes_2"] });
        m_frameDb["spider_attack_1"] = Frame({ m_imageDb["spider_attack_1"], m_imageDb["spider_eyes_attack_1"] });
        m_frameDb["spider_attack_2"] = Frame({ m_imageDb["spider_attack_2"], m_imageDb["spider_eyes_attack_2"] });
        m_frameDb["spider_attack_3"] = Frame({ m_imageDb["spider_attack_3"], m_imageDb["spider_eyes_attack_3"] });
        m_frameDb["spider_attack_4"] = Frame({ m_imageDb["spider_attack_3"], m_imageDb["spider_eyes_attack_3"] });
        m_frameDb["alt_spider_attack_1"] = Frame({ m_imageDb["alt_spider_attack_1"], m_imageDb["alt_spider_eyes_attack_1"] });
        m_frameDb["alt_spider_attack_2"] = Frame({ m_imageDb["alt_spider_attack_2"], m_imageDb["alt_spider_eyes_attack_2"] });
        m_frameDb["alt_spider_attack_3"] = Frame({ m_imageDb["alt_spider_attack_2"], m_imageDb["alt_spider_eyes_attack_3"] });
        m_frameDb["alt_spider_attack_4"] = Frame({ m_imageDb["alt_spider_attack_3"], m_imageDb["alt_spider_eyes_attack_3"] });
        m_frameDb["alt_spider_attack_5"] = Frame({ m_imageDb["alt_spider_attack_1"], m_imageDb["alt_spider_eyes_attack_1"] });

        m_frameDb["bat_1"] = Frame({ m_imageDb["bat_1"], m_imageDb["bat_eyes_1"] });
        m_frameDb["bat_2"] = Frame({ m_imageDb["bat_2"], m_imageDb["bat_eyes_2"] });
        m_frameDb["bat_3"] = Frame({ m_imageDb["bat_3"], m_imageDb["bat_eyes_3"] });
        m_frameDb["bat_4"] = Frame({ m_imageDb["bat_4"], m_imageDb["bat_eyes_4"] });
        m_frameDb["bat_attack_1"] = Frame({ m_imageDb["bat_attack_1"], m_imageDb["bat_attack_eyes_1"] });
        m_frameDb["bat_attack_2"] = Frame({ m_imageDb["bat_attack_2"], m_imageDb["bat_attack_eyes_2"] });
        m_frameDb["bat_attack_3"] = Frame({ m_imageDb["bat_attack_3"], m_imageDb["bat_attack_eyes_3"] });
        m_frameDb["bat_attack_4"] = Frame({ m_imageDb["bat_attack_4"], m_imageDb["bat_attack_eyes_4"] });
        m_frameDb["alt_bat_attack_1"] = Frame({ m_imageDb["alt_bat_attack_1"], m_imageDb["alt_bat_attack_eyes_1"] });
        m_frameDb["alt_bat_attack_2"] = Frame({ m_imageDb["alt_bat_attack_2"], m_imageDb["alt_bat_attack_eyes_2"] });
        m_frameDb["alt_bat_attack_3"] = Frame({ m_imageDb["alt_bat_attack_3"], m_imageDb["alt_bat_attack_eyes_3"] });

        m_frameDb["skeleton_1"] = Frame({ m_imageDb["skeleton_1"], m_imageDb["skeleton_eyes_1"] });
        m_frameDb["skeleton_2"] = Frame({ m_imageDb["skeleton_2"], m_imageDb["skeleton_eyes_2"] });
        m_frameDb["skeleton_attack_1"] = Frame({ m_imageDb["skeleton_attack_1"], m_imageDb["skeleton_eyes_attack_1"] });
        m_frameDb["skeleton_attack_2"] = Frame({ m_imageDb["skeleton_attack_2"], m_imageDb["skeleton_eyes_attack_2"] });
        m_frameDb["skeleton_attack_3"] = Frame({ m_imageDb["skeleton_attack_1"], m_imageDb["skeleton_eyes_attack_1"] });
        m_frameDb["skeleton_attack_4"] = Frame({ m_imageDb["skeleton_attack_2"], m_imageDb["skeleton_eyes_attack_2"] });
        m_frameDb["alt_skeleton_attack_1"] = Frame({ m_imageDb["alt_skeleton_attack_1"], m_imageDb["alt_skeleton_eyes_attack_1"] });
        m_frameDb["alt_skeleton_attack_2"] = Frame({ m_imageDb["alt_skeleton_attack_2"], m_imageDb["alt_skeleton_eyes_attack_2"] });
        m_frameDb["alt_skeleton_attack_3"] = Frame({ m_imageDb["alt_skeleton_attack_3"], m_imageDb["alt_skeleton_eyes_attack_3"] });
        m_frameDb["alt_skeleton_attack_4"] = Frame({ m_imageDb["alt_skeleton_attack_4"], m_imageDb["alt_skeleton_eyes_attack_4"] });
        m_frameDb["spawn_skeleton_1"] = Frame({ m_imageDb["spawn_skeleton_1"], m_imageDb["spawn_skeleton_eyes_1"] });
        m_frameDb["spawn_skeleton_2"] = Frame({ m_imageDb["spawn_skeleton_2"], m_imageDb["spawn_skeleton_eyes_2"] });
        m_frameDb["spawn_skeleton_3"] = Frame({ m_imageDb["spawn_skeleton_3"], m_imageDb["spawn_skeleton_eyes_3"] });
        m_frameDb["spawn_skeleton_4"] = Frame({ m_imageDb["spawn_skeleton_4"], m_imageDb["spawn_skeleton_eyes_4"] });

        m_frameDb["dragon_1"] = Frame({ m_imageDb["dragon_1"], m_imageDb["dragon_eyes_1"] });
        m_frameDb["dragon_2"] = Frame({ m_imageDb["dragon_2"], m_imageDb["dragon_eyes_2"] });
        m_frameDb["dragon_3"] = Frame({ m_imageDb["dragon_3"], m_imageDb["dragon_eyes_3"] });
        m_frameDb["dragon_attack_1"] = Frame({ m_imageDb["dragon_attack_1"], m_imageDb["dragon_eyes_attack_1"] });
        m_frameDb["dragon_attack_2"] = Frame({ m_imageDb["dragon_attack_2"], m_imageDb["dragon_eyes_attack_2"] });
        m_frameDb["dragon_attack_3"] = Frame({ m_imageDb["dragon_attack_3"], m_imageDb["dragon_eyes_attack_3"] });

        m_frameDb["armor_robe"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"] });
        m_frameDb["armor_leather"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"] });
        m_frameDb["armor_plate"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"] });

        m_frameDb["armor_rare_robe_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["rare_1"]});
        m_frameDb["armor_rare_robe_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["rare_2"] });
        m_frameDb["armor_rare_robe_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["rare_3"] });
        m_frameDb["armor_rare_robe_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["rare_4"] });
        m_frameDb["armor_rare_leather_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["rare_1"] });
        m_frameDb["armor_rare_leather_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["rare_2"] });
        m_frameDb["armor_rare_leather_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["rare_3"] });
        m_frameDb["armor_rare_leather_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["rare_4"] });
        m_frameDb["armor_rare_plate_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["rare_1"] });
        m_frameDb["armor_rare_plate_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["rare_2"] });
        m_frameDb["armor_rare_plate_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["rare_3"] });
        m_frameDb["armor_rare_plate_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["rare_4"] });

        m_frameDb["armor_epic_robe_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["epic_1"] });
        m_frameDb["armor_epic_robe_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["epic_2"] });
        m_frameDb["armor_epic_robe_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["epic_3"] });
        m_frameDb["armor_epic_robe_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["epic_4"] });
        m_frameDb["armor_epic_leather_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["epic_1"] });
        m_frameDb["armor_epic_leather_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["epic_2"] });
        m_frameDb["armor_epic_leather_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["epic_3"] });
        m_frameDb["armor_epic_leather_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["epic_4"] });
        m_frameDb["armor_epic_plate_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["epic_1"] });
        m_frameDb["armor_epic_plate_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["epic_2"] });
        m_frameDb["armor_epic_plate_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["epic_3"] });
        m_frameDb["armor_epic_plate_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["epic_4"] });

        m_frameDb["armor_legendary_robe_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["legendary_1"] });
        m_frameDb["armor_legendary_robe_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["legendary_2"] });
        m_frameDb["armor_legendary_robe_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["legendary_3"] });
        m_frameDb["armor_legendary_robe_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["robe"], m_imageDb["robe_lining"], m_imageDb["legendary_4"] });
        m_frameDb["armor_legendary_leather_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["legendary_1"] });
        m_frameDb["armor_legendary_leather_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["legendary_2"] });
        m_frameDb["armor_legendary_leather_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["legendary_3"] });
        m_frameDb["armor_legendary_leather_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["leather"], m_imageDb["leather_lining"], m_imageDb["legendary_4"] });
        m_frameDb["armor_legendary_plate_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["legendary_1"] });
        m_frameDb["armor_legendary_plate_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["legendary_2"] });
        m_frameDb["armor_legendary_plate_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["legendary_3"] });
        m_frameDb["armor_legendary_plate_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["plate"], m_imageDb["plate_lining"], m_imageDb["legendary_4"] });


        m_frameDb["weapon_gloves"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"] });
        m_frameDb["weapon_wand"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"] });
        m_frameDb["weapon_staff"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"] });
        m_frameDb["weapon_dagger"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"] });
        m_frameDb["weapon_sword"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"] });
        m_frameDb["weapon_great_sword"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"] });

        m_frameDb["weapon_rare_gloves_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_gloves_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_gloves_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_gloves_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["rare_4"] });
        m_frameDb["weapon_rare_wand_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_wand_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_wand_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_wand_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["rare_4"] });
        m_frameDb["weapon_rare_staff_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_staff_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_staff_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_staff_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["rare_4"] });
        m_frameDb["weapon_rare_dagger_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_dagger_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_dagger_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_dagger_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["rare_4"] });
        m_frameDb["weapon_rare_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["rare_4"] });
        m_frameDb["weapon_rare_great_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["rare_1"] });
        m_frameDb["weapon_rare_great_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["rare_2"] });
        m_frameDb["weapon_rare_great_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["rare_3"] });
        m_frameDb["weapon_rare_great_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["rare_4"] });

        m_frameDb["weapon_epic_gloves_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_gloves_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_gloves_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_gloves_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["epic_4"] });
        m_frameDb["weapon_epic_wand_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_wand_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_wand_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_wand_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["epic_4"] });
        m_frameDb["weapon_epic_staff_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_staff_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_staff_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_staff_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["epic_4"] });
        m_frameDb["weapon_epic_dagger_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_dagger_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_dagger_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_dagger_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["epic_4"] });
        m_frameDb["weapon_epic_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["epic_4"] });
        m_frameDb["weapon_epic_great_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["epic_1"] });
        m_frameDb["weapon_epic_great_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["epic_2"] });
        m_frameDb["weapon_epic_great_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["epic_3"] });
        m_frameDb["weapon_epic_great_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["epic_4"] });

        m_frameDb["weapon_legendary_gloves_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_gloves_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_gloves_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_gloves_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["gloves"], m_imageDb["knuckles"], m_imageDb["legendary_4"] });
        m_frameDb["weapon_legendary_wand_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_wand_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_wand_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_wand_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["wand_handle"], m_imageDb["wand"], m_imageDb["legendary_4"] });
        m_frameDb["weapon_legendary_staff_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_staff_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_staff_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_staff_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["staff_handle"], m_imageDb["staff"], m_imageDb["legendary_4"] });
        m_frameDb["weapon_legendary_dagger_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_dagger_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_dagger_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_dagger_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["dagger_hilt"], m_imageDb["dagger_blade"], m_imageDb["legendary_4"] });
        m_frameDb["weapon_legendary_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["sword_hilt"], m_imageDb["sword_blade"], m_imageDb["legendary_4"] });
        m_frameDb["weapon_legendary_great_sword_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["legendary_1"] });
        m_frameDb["weapon_legendary_great_sword_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["legendary_2"] });
        m_frameDb["weapon_legendary_great_sword_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["legendary_3"] });
        m_frameDb["weapon_legendary_great_sword_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["great_sword_hilt"], m_imageDb["great_sword_blade"], m_imageDb["legendary_4"] });

        m_frameDb["lesser_healing_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["lesser_potion_bottle"], m_imageDb["lesser_healing_potion"] });
        m_frameDb["lesser_magic_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["lesser_potion_bottle"], m_imageDb["lesser_magic_potion"] });
        m_frameDb["lesser_resurrect_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["lesser_potion_bottle"], m_imageDb["lesser_resurrect_potion"] });
        m_frameDb["healing_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["potion_bottle"], m_imageDb["healing_potion"] });
        m_frameDb["magic_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["potion_bottle"], m_imageDb["magic_potion"] });
        m_frameDb["resurrect_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["potion_bottle"], m_imageDb["resurrect_potion"] });
        m_frameDb["greater_healing_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["greater_potion_bottle"], m_imageDb["greater_healing_potion"] });
        m_frameDb["greater_magic_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["greater_potion_bottle"], m_imageDb["greater_magic_potion"] });
        m_frameDb["greater_resurrect_potion"] = Frame({ m_imageDb["weapon_border"], m_imageDb["greater_potion_bottle"], m_imageDb["greater_resurrect_potion"] });

        m_frameDb["new_hero"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["new_hero"] });
        m_frameDb["level_up"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["level_up"] });
        m_frameDb["level_up_5_1"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["level_up_5"], m_imageDb["epic_1"] });
        m_frameDb["level_up_5_2"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["level_up_5"], m_imageDb["epic_2"] });
        m_frameDb["level_up_5_3"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["level_up_5"], m_imageDb["epic_3"] });
        m_frameDb["level_up_5_4"] = Frame({ m_imageDb["weapon_border"], m_imageDb["hero_card"], m_imageDb["level_up_5"], m_imageDb["epic_4"] });
        m_frameDb["exit"] = Frame({ m_imageDb["weapon_border"], m_imageDb["exit"] });
    }

    // ----------------------------------------------------------------------------------------------------------------
    m_animationDb.clear();
    {
        m_animationDb["background_1"].SetData(
            { m_frameDb["background_1"] },
            200, false, false, false);
        m_animationDb["background_2"].SetData(
            { m_frameDb["background_2"] },
            200, false, false, false);

        m_animationDb["side_bird"].SetData(
            { m_frameDb["side_bird_1"], m_frameDb["side_bird_2"] },
            100, 200, true, true, true);
        m_animationDb["side_tile_0"].SetData(
            { m_frameDb["side_tile_0_1"], m_frameDb["side_tile_0_2"], m_frameDb["side_tile_0_3"], m_frameDb["side_tile_0_4"], m_frameDb["side_tile_0_5"], m_frameDb["side_tile_0_6"], m_frameDb["side_tile_0_7"], m_frameDb["side_tile_0_8"], m_frameDb["side_tile_0_9"], m_frameDb["side_tile_0_10"], m_frameDb["side_tile_0_11"], m_frameDb["side_tile_0_12"], m_frameDb["side_tile_0_13"], m_frameDb["side_tile_0_14"], m_frameDb["side_tile_0_15"], m_frameDb["side_tile_0_16"], m_frameDb["side_tile_0_17"], m_frameDb["side_tile_0_18"], m_frameDb["side_tile_0_19"], m_frameDb["side_tile_0_20"], },
            1200, true, true, false);
        m_animationDb["side_tile_1"].SetData(
            { m_frameDb["side_tile_1"] },
            200, false, false, false);
        m_animationDb["side_tile_2"].SetData(
            { m_frameDb["side_tile_2"] },
            200, false, false, false);
        m_animationDb["side_tile_3_right"].SetData(
            { m_frameDb["side_tile_3_right"] },
            200, false, false, false);
        m_animationDb["side_tile_3_left"].SetData(
            { m_frameDb["side_tile_3_left"] },
            200, false, false, false);

        m_animationDb["trap"].SetData(
            { m_frameDb["trap"] },
            200, false, false, false);
        m_animationDb["spikes_1"].SetData(
            { m_frameDb["spikes_1"] },
            200, false, false, false);
        m_animationDb["spikes_2"].SetData(
            { m_frameDb["spikes_2"] },
            200, false, false, false);
        m_animationDb["swinging_axe_1"].SetData(
            { m_frameDb["swinging_axe_1"], m_frameDb["swinging_axe_2"], m_frameDb["swinging_axe_3"], m_frameDb["swinging_axe_4"], m_frameDb["swinging_axe_5"], m_frameDb["swinging_axe_6"], m_frameDb["swinging_axe_7"], m_frameDb["swinging_axe_8"], m_frameDb["swinging_axe_9"], m_frameDb["swinging_axe_10"], },
            100, true, true, false);
        m_animationDb["swinging_axe_2"].SetData(
            { m_frameDb["swinging_axe_1"], m_frameDb["swinging_axe_2"], m_frameDb["swinging_axe_3"], m_frameDb["swinging_axe_4"], m_frameDb["swinging_axe_5"], m_frameDb["swinging_axe_6"], m_frameDb["swinging_axe_7"], m_frameDb["swinging_axe_8"], m_frameDb["swinging_axe_9"], m_frameDb["swinging_axe_10"], },
            200, true, true, false);

        m_animationDb["stairs"].SetData(
            { m_frameDb["stairs_1"], m_frameDb["stairs_2"] },
            600, true, true, false);

        m_animationDb["fountain"].SetData(
            { m_frameDb["fountain_1"], m_frameDb["fountain_2"], },
            200, 400, true, true, true);

        m_animationDb["fairy"].SetData(
            { m_frameDb["fairy_1"], m_frameDb["fairy_2"] },
            100, 200, true, true, true);
        m_animationDb["fairy_dust"].SetData(
            { m_frameDb["fairy_dust_1"], m_frameDb["fairy_dust_2"]  },
            50, 100, true, true, true);

        m_animationDb["common_door_opened"].SetData(
            { m_frameDb["common_door_opened"] },
            200, false, false, false);
        m_animationDb["common_door_closed"].SetData(
            { m_frameDb["common_door_closed"] },
            200, false, false, false);
        m_animationDb["rare_door_opened"].SetData(
            { m_frameDb["rare_door_opened"] },
            200, false, false, false);
        m_animationDb["rare_door_closed"].SetData(
            { m_frameDb["rare_door_closed"] },
            200, false, false, false);
        m_animationDb["epic_door_opened"].SetData(
            { m_frameDb["epic_door_opened"] },
            200, false, false, false);
        m_animationDb["epic_door_closed"].SetData(
            { m_frameDb["epic_door_closed"] },
            200, false, false, false);
        m_animationDb["legendary_door_opened"].SetData(
            { m_frameDb["legendary_door_opened_1"], m_frameDb["legendary_door_opened_2"], m_frameDb["legendary_door_opened_3"], },
            100, 300, false, true, true);
        m_animationDb["legendary_door_closed"].SetData(
            { m_frameDb["legendary_door_closed_1"], m_frameDb["legendary_door_closed_2"], m_frameDb["legendary_door_closed_3"], },
            100, 300, false, true, true);

        m_animationDb["select_nothing"].SetData(
            { m_frameDb["select_nothing"] },
            200, false, false, false);        
        m_animationDb["select_door"].SetData(
            { m_frameDb["select_door_1"], m_frameDb["select_door_2"], m_frameDb["select_door_3"], m_frameDb["select_door_4"], m_frameDb["select_door_5"], m_frameDb["select_door_6"], m_frameDb["select_door_7"], m_frameDb["select_door_8"] },
            80, true, true, false);
        m_animationDb["select_weapon"].SetData(
            { m_frameDb["select_weapon_1"], m_frameDb["select_weapon_2"], m_frameDb["select_weapon_3"], m_frameDb["select_weapon_4"], m_frameDb["select_weapon_5"], m_frameDb["select_weapon_6"], m_frameDb["select_weapon_7"], m_frameDb["select_weapon_8"], m_frameDb["select_weapon_9"], m_frameDb["select_weapon_10"] },
            80, true, true, false);
        m_animationDb["select_hero"].SetData(
            { m_frameDb["select_hero_1"], m_frameDb["select_hero_2"], m_frameDb["select_hero_3"], m_frameDb["select_hero_4"], m_frameDb["select_hero_5"], m_frameDb["select_hero_6"], m_frameDb["select_hero_7"], m_frameDb["select_hero_8"], m_frameDb["select_hero_9"], m_frameDb["select_hero_10"], m_frameDb["select_hero_11"], m_frameDb["select_hero_12"] },
            80, true, true, false);

        m_animationDb["hero_dead"].SetData(
            { m_frameDb["hero_dead"] },
            200, false, false, false);
        m_animationDb["hero_level"].SetData(
            { m_frameDb["hero_level"] },
            200, false, false, false);
        m_animationDb["hero_border"].SetData(
            { m_frameDb["hero_border"] },
            200, false, false, false);
        m_animationDb["hero_protect"].SetData(
            { m_frameDb["hero_protect"] },
            200, false, false, false);

        m_animationDb["chest_closed"].SetData(
            { m_frameDb["chest_closed"] },
            200, false, false, false);
        m_animationDb["chest_opened"].SetData(
            { m_frameDb["chest_opened"] },
            200, false, false, false);

        m_animationDb["torch"].SetData(
            { m_frameDb["torch_1"], m_frameDb["torch_2"], m_frameDb["torch_3"], m_frameDb["torch_4"], },
            200, 400, true, true, true);

        m_animationDb["fist_attack"].SetData(
            { m_frameDb["fist_attack_1"], m_frameDb["fist_attack_2"], m_frameDb["fist_attack_3"], m_frameDb["fist_attack_4"], m_frameDb["fist_attack_5"], m_frameDb["fist_attack_6"], m_frameDb["fist_attack_7"], m_frameDb["fist_attack_8"], m_frameDb["fist_attack_9"], m_frameDb["fist_attack_10"] },
            150, false, true, false);
        m_animationDb["blast_attack"].SetData(
            { m_frameDb["blast_attack_1"], m_frameDb["blast_attack_2"], m_frameDb["blast_attack_3"], m_frameDb["blast_attack_4"], m_frameDb["blast_attack_5"], m_frameDb["blast_attack_6"], m_frameDb["blast_attack_7"], m_frameDb["blast_attack_8"], m_frameDb["blast_attack_9"], m_frameDb["blast_attack_10"] },
            120, false, true, false);
        m_animationDb["slash_attack"].SetData(
            { m_frameDb["slash_attack_1"], m_frameDb["slash_attack_2"], m_frameDb["slash_attack_3"], m_frameDb["slash_attack_4"], m_frameDb["slash_attack_5"], m_frameDb["slash_attack_6"], m_frameDb["slash_attack_7"], m_frameDb["slash_attack_8"], m_frameDb["slash_attack_9"], m_frameDb["slash_attack_10"] },
            120, false, true, false);

        m_animationDb["slime"].SetData(
            { m_frameDb["slime_1"], m_frameDb["slime_2"], m_frameDb["slime_3"], m_frameDb["slime_4"] },
            120, 800, true, true, true);
        m_animationDb["slime_attack"].SetData(
            { m_frameDb["slime_attack_1"], m_frameDb["slime_attack_2"], m_frameDb["slime_attack_3"] },
            100, false, true, false);

        m_animationDb["blob"].SetData(
            { m_frameDb["blob_1"], m_frameDb["blob_2"], m_frameDb["blob_3"], m_frameDb["blob_4"] },
            80, 120, true, true, true);
        m_animationDb["blob_attack"].SetData(
            { m_frameDb["blob_attack_1"], m_frameDb["blob_attack_2"], m_frameDb["blob_attack_3"], m_frameDb["blob_attack_4"], m_frameDb["blob_attack_5"], m_frameDb["blob_attack_6"], m_frameDb["blob_attack_7"], m_frameDb["blob_attack_8"], m_frameDb["blob_attack_9"], m_frameDb["blob_attack_10"], m_frameDb["blob_attack_11"], m_frameDb["blob_attack_12"] },
            120, false, true, false);
        m_animationDb["alt_blob_attack"].SetData(
            { m_frameDb["alt_blob_attack_1"], m_frameDb["alt_blob_attack_2"], m_frameDb["alt_blob_attack_3"], m_frameDb["alt_blob_attack_4"], m_frameDb["alt_blob_attack_5"], },
            120, false, true, false);

        m_animationDb["skeleton"].SetData(
            { m_frameDb["skeleton_1"], m_frameDb["skeleton_2"], },
            120, 600, true, true, true);
        m_animationDb["skeleton_attack"].SetData(
            { m_frameDb["skeleton_attack_1"], m_frameDb["skeleton_attack_2"], m_frameDb["skeleton_attack_3"], m_frameDb["skeleton_attack_4"], },
            200, false, true, false);
        m_animationDb["alt_skeleton_attack"].SetData(
            { m_frameDb["alt_skeleton_attack_1"], m_frameDb["alt_skeleton_attack_2"], m_frameDb["alt_skeleton_attack_3"], m_frameDb["alt_skeleton_attack_4"], },
            200, false, true, false);
        m_animationDb["spawn_skeleton"].SetData(
            { m_frameDb["spawn_skeleton_1"], m_frameDb["spawn_skeleton_2"], m_frameDb["spawn_skeleton_3"], m_frameDb["spawn_skeleton_4"], },
            200, false, true, false);

        m_animationDb["spider"].SetData(
            { m_frameDb["spider_1"], m_frameDb["spider_2"], m_frameDb["spider_3"], m_frameDb["spider_4"], m_frameDb["spider_5"], m_frameDb["spider_6"] },
            120, 600, true, true, true);
        m_animationDb["spider_attack"].SetData(
            { m_frameDb["spider_attack_1"], m_frameDb["spider_attack_2"], m_frameDb["spider_attack_3"], m_frameDb["spider_attack_4"] },
            200, false, true, false);
        m_animationDb["alt_spider_attack"].SetData(
            { m_frameDb["alt_spider_attack_1"], m_frameDb["alt_spider_attack_2"], m_frameDb["alt_spider_attack_3"], m_frameDb["alt_spider_attack_4"], m_frameDb["alt_spider_attack_5"] },
            200, false, true, false);

        m_animationDb["bat"].SetData(
            { m_frameDb["bat_1"], m_frameDb["bat_2"], m_frameDb["bat_3"], m_frameDb["bat_4"] },
            120, 220, true, true, false);
        m_animationDb["bat_attack"].SetData(
            { m_frameDb["bat_attack_1"], m_frameDb["bat_attack_2"], m_frameDb["bat_attack_3"], m_frameDb["bat_attack_4"] },
            150, false, true, false);
        m_animationDb["alt_bat_attack"].SetData(
            { m_frameDb["alt_bat_attack_1"], m_frameDb["alt_bat_attack_2"], m_frameDb["alt_bat_attack_3"] },
            150, false, true, false);

        m_animationDb["dragon"].SetData(
            { m_frameDb["dragon_1"], m_frameDb["dragon_2"], m_frameDb["dragon_3"] },
            300, 600, true, true, true);
        m_animationDb["dragon_attack"].SetData(
            { m_frameDb["dragon_attack_1"], m_frameDb["dragon_attack_2"], m_frameDb["dragon_attack_3"] },
            200, false, true, false);

        m_animationDb["weapon_gloves"].SetData(
            { m_frameDb["weapon_gloves"] },
            200, false, true, false);
        m_animationDb["weapon_wand"].SetData(
            { m_frameDb["weapon_wand"] },
            200, false, true, false);
        m_animationDb["weapon_staff"].SetData(
            { m_frameDb["weapon_staff"] },
            200, false, true, false);
        m_animationDb["weapon_dagger"].SetData(
            { m_frameDb["weapon_dagger"] },
            200, false, true, false);
        m_animationDb["weapon_sword"].SetData(
            { m_frameDb["weapon_sword"] },
            200, false, true, false);
        m_animationDb["weapon_great_sword"].SetData(
            { m_frameDb["weapon_great_sword"] },
            200, false, true, false);
        m_animationDb["armor_robe"].SetData(
            { m_frameDb["armor_robe"] },
            200, false, true, false);
        m_animationDb["armor_leather"].SetData(
            { m_frameDb["armor_leather"] },
            200, false, true, false);
        m_animationDb["armor_plate"].SetData(
            { m_frameDb["armor_plate"] },
            200, false, true, false);

        m_animationDb["weapon_rare_gloves"].SetData(
            { m_frameDb["weapon_rare_gloves_1"], m_frameDb["weapon_rare_gloves_2"], m_frameDb["weapon_rare_gloves_3"], m_frameDb["weapon_rare_gloves_4"] },
            200, false, true, true);
        m_animationDb["weapon_rare_wand"].SetData(
            { m_frameDb["weapon_rare_wand_1"], m_frameDb["weapon_rare_wand_2"], m_frameDb["weapon_rare_wand_3"], m_frameDb["weapon_rare_wand_4"] },
            200, false, true, true);
        m_animationDb["weapon_rare_staff"].SetData(
            { m_frameDb["weapon_rare_staff_1"], m_frameDb["weapon_rare_staff_2"], m_frameDb["weapon_rare_staff_3"], m_frameDb["weapon_rare_staff_4"] },
            200, false, true, true);
        m_animationDb["weapon_rare_dagger"].SetData(
            { m_frameDb["weapon_rare_dagger_1"], m_frameDb["weapon_rare_dagger_2"], m_frameDb["weapon_rare_dagger_3"], m_frameDb["weapon_rare_dagger_4"] },
            200, false, true, true);
        m_animationDb["weapon_rare_sword"].SetData(
            { m_frameDb["weapon_rare_sword_1"], m_frameDb["weapon_rare_sword_2"], m_frameDb["weapon_rare_sword_3"], m_frameDb["weapon_rare_sword_4"] },
            200, false, true, true);
        m_animationDb["weapon_rare_great_sword"].SetData(
            { m_frameDb["weapon_rare_great_sword_1"], m_frameDb["weapon_rare_great_sword_2"], m_frameDb["weapon_rare_great_sword_3"], m_frameDb["weapon_rare_great_sword_4"] },
            200, false, true, true);
        m_animationDb["armor_rare_robe"].SetData(
            { m_frameDb["armor_rare_robe_1"], m_frameDb["armor_rare_robe_2"], m_frameDb["armor_rare_robe_3"], m_frameDb["armor_rare_robe_4"], },
            200, false, true, true);
        m_animationDb["armor_rare_leather"].SetData(
            { m_frameDb["armor_rare_leather_1"], m_frameDb["armor_rare_leather_2"], m_frameDb["armor_rare_leather_3"], m_frameDb["armor_rare_leather_4"], },
            200, false, true, true);
        m_animationDb["armor_rare_plate"].SetData(
            { m_frameDb["armor_rare_plate_1"], m_frameDb["armor_rare_plate_2"], m_frameDb["armor_rare_plate_3"], m_frameDb["armor_rare_plate_4"], },
            200, false, true, true);

        m_animationDb["weapon_epic_gloves"].SetData(
            { m_frameDb["weapon_epic_gloves_1"], m_frameDb["weapon_epic_gloves_2"], m_frameDb["weapon_epic_gloves_3"], m_frameDb["weapon_epic_gloves_4"] },
            200, false, true, true);
        m_animationDb["weapon_epic_wand"].SetData(
            { m_frameDb["weapon_epic_wand_1"], m_frameDb["weapon_epic_wand_2"], m_frameDb["weapon_epic_wand_3"], m_frameDb["weapon_epic_wand_4"] },
            200, false, true, true);
        m_animationDb["weapon_epic_staff"].SetData(
            { m_frameDb["weapon_epic_staff_1"], m_frameDb["weapon_epic_staff_2"], m_frameDb["weapon_epic_staff_3"], m_frameDb["weapon_epic_staff_4"] },
            200, false, true, true);
        m_animationDb["weapon_epic_dagger"].SetData(
            { m_frameDb["weapon_epic_dagger_1"], m_frameDb["weapon_epic_dagger_2"], m_frameDb["weapon_epic_dagger_3"], m_frameDb["weapon_epic_dagger_4"] },
            200, false, true, true);
        m_animationDb["weapon_epic_sword"].SetData(
            { m_frameDb["weapon_epic_sword_1"], m_frameDb["weapon_epic_sword_2"], m_frameDb["weapon_epic_sword_3"], m_frameDb["weapon_epic_sword_4"] },
            200, false, true, true);
        m_animationDb["weapon_epic_great_sword"].SetData(
            { m_frameDb["weapon_epic_great_sword_1"], m_frameDb["weapon_epic_great_sword_2"], m_frameDb["weapon_epic_great_sword_3"], m_frameDb["weapon_epic_great_sword_4"] },
            200, false, true, true);
        m_animationDb["armor_epic_robe"].SetData(
            { m_frameDb["armor_epic_robe_1"], m_frameDb["armor_epic_robe_2"], m_frameDb["armor_epic_robe_3"], m_frameDb["armor_epic_robe_4"], },
            200, false, true, true);
        m_animationDb["armor_epic_leather"].SetData(
            { m_frameDb["armor_epic_leather_1"], m_frameDb["armor_epic_leather_2"], m_frameDb["armor_epic_leather_3"], m_frameDb["armor_epic_leather_4"], },
            200, false, true, true);
        m_animationDb["armor_epic_plate"].SetData(
            { m_frameDb["armor_epic_plate_1"], m_frameDb["armor_epic_plate_2"], m_frameDb["armor_epic_plate_3"], m_frameDb["armor_epic_plate_4"], },
            200, false, true, true);


        m_animationDb["weapon_legendary_gloves"].SetData(
            { m_frameDb["weapon_legendary_gloves_1"], m_frameDb["weapon_legendary_gloves_2"], m_frameDb["weapon_legendary_gloves_3"], m_frameDb["weapon_legendary_gloves_4"] },
            200, false, true, true);
        m_animationDb["weapon_legendary_wand"].SetData(
            { m_frameDb["weapon_legendary_wand_1"], m_frameDb["weapon_legendary_wand_2"], m_frameDb["weapon_legendary_wand_3"], m_frameDb["weapon_legendary_wand_4"] },
            200, false, true, true);
        m_animationDb["weapon_legendary_staff"].SetData(
            { m_frameDb["weapon_legendary_staff_1"], m_frameDb["weapon_legendary_staff_2"], m_frameDb["weapon_legendary_staff_3"], m_frameDb["weapon_legendary_staff_4"] },
            200, false, true, true);
        m_animationDb["weapon_legendary_dagger"].SetData(
            { m_frameDb["weapon_legendary_dagger_1"], m_frameDb["weapon_legendary_dagger_2"], m_frameDb["weapon_legendary_dagger_3"], m_frameDb["weapon_legendary_dagger_4"] },
            200, false, true, true);
        m_animationDb["weapon_legendary_sword"].SetData(
            { m_frameDb["weapon_legendary_sword_1"], m_frameDb["weapon_legendary_sword_2"], m_frameDb["weapon_legendary_sword_3"], m_frameDb["weapon_legendary_sword_4"] },
            200, false, true, true);
        m_animationDb["weapon_legendary_great_sword"].SetData(
            { m_frameDb["weapon_legendary_great_sword_1"], m_frameDb["weapon_legendary_great_sword_2"], m_frameDb["weapon_legendary_great_sword_3"], m_frameDb["weapon_legendary_great_sword_4"] },
            200, false, true, true);
        m_animationDb["armor_legendary_robe"].SetData(
            { m_frameDb["armor_legendary_robe_1"], m_frameDb["armor_legendary_robe_2"], m_frameDb["armor_legendary_robe_3"], m_frameDb["armor_legendary_robe_4"], },
            200, false, true, true);
        m_animationDb["armor_legendary_leather"].SetData(
            { m_frameDb["armor_legendary_leather_1"], m_frameDb["armor_legendary_leather_2"], m_frameDb["armor_legendary_leather_3"], m_frameDb["armor_legendary_leather_4"], },
            200, false, true, true);
        m_animationDb["armor_legendary_plate"].SetData(
            { m_frameDb["armor_legendary_plate_1"], m_frameDb["armor_legendary_plate_2"], m_frameDb["armor_legendary_plate_3"], m_frameDb["armor_legendary_plate_4"], },
            200, false, true, true);

        m_animationDb["lesser_healing_potion"].SetData(
            { m_frameDb["lesser_healing_potion"] },
            200, false, true, false);
        m_animationDb["lesser_magic_potion"].SetData(
            { m_frameDb["lesser_magic_potion"] },
            200, false, true, false);
        m_animationDb["lesser_resurrect_potion"].SetData(
            { m_frameDb["lesser_resurrect_potion"] },
            200, false, true, false);
        m_animationDb["healing_potion"].SetData(
            { m_frameDb["healing_potion"] },
            200, false, true, false);
        m_animationDb["magic_potion"].SetData(
            { m_frameDb["magic_potion"] },
            200, false, true, false);
        m_animationDb["resurrect_potion"].SetData(
            { m_frameDb["resurrect_potion"] },
            200, false, true, false);
        m_animationDb["greater_healing_potion"].SetData(
            { m_frameDb["greater_healing_potion"] },
            200, false, true, false);
        m_animationDb["greater_magic_potion"].SetData(
            { m_frameDb["greater_magic_potion"] },
            200, false, true, false);
        m_animationDb["greater_resurrect_potion"].SetData(
            { m_frameDb["greater_resurrect_potion"] },
            200, false, true, false);

        m_animationDb["new_hero"].SetData(
            { m_frameDb["new_hero"] },
            200, false, true, false);
        m_animationDb["level_up"].SetData(
            { m_frameDb["level_up"] },
            200, false, true, false);
        m_animationDb["level_up_5"].SetData(
            { m_frameDb["level_up_5_1"], m_frameDb["level_up_5_2"], m_frameDb["level_up_5_3"], m_frameDb["level_up_5_4"], },
            200, false, true, true);

        m_animationDb["exit"].SetData(
            { m_frameDb["exit"] },
            200, false, true, false);
    }

    // ----------------------------------------------------------------------------------------------------------------
    m_dieRangeDb.clear();
    {
        //                                        mult  die   const speed mp    ac    cost
        m_dieRangeDb["weapon_unarmed"] = DieRange(0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, Die(0, 0, 0), DamageType::NORMAL);
        m_dieRangeDb["weapon_gloves"]  = DieRange(1, 2, 4, 4, 1, 3, 0, 0, 0, 0, 0, 0, Die(2, 4, 4));
        m_dieRangeDb["weapon_wand"]    = DieRange(2, 4, 4, 4, 1, 5, 2, 5, 4, 5, 0, 0, Die(2, 4, 4));
        m_dieRangeDb["weapon_staff"]   = DieRange(3, 5, 8, 8, 1, 10, 8, 10, 8, 15, 0, 0, Die(2, 8, 8));
        m_dieRangeDb["weapon_dagger"]  = DieRange(1, 3, 4, 4, 1, 10, 1, 4, 0, 0, 0, 0, Die(2, 4, 4));
        m_dieRangeDb["weapon_sword"]   = DieRange(1, 4, 6, 6, 1, 6, 3, 6, 0, 0, 0, 0, Die(2, 6, 6));
        m_dieRangeDb["weapon_great_sword"]
                                       = DieRange(1, 4, 10, 10, 1, 10, 8, 15, 0, 0, 0, 0, Die(2, 10, 10));

        m_dieRangeDb["armor_robe"]     = DieRange(1, 1, 10, 10, 8, 8, 0, 2, 0, 0, 1, 2, Die(4, 4, 4));
        m_dieRangeDb["armor_leather"]  = DieRange(0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 2, 3, Die(4, 8, 4));
        m_dieRangeDb["armor_plate"]    = DieRange(0, 0, 0, 0, 0, 0, 2, 4, 0, 0, 3, 4, Die(4, 10, 4));

        m_dieRangeDb["potion_lesser"]  = DieRange(2, 2, 8, 8, 1, 4, 1, 1, 0, 0, 0, 0, Die(1, 8, 8));
        m_dieRangeDb["potion"]         = DieRange(4, 4, 8, 8, 4, 8, 1, 1, 0, 0, 0, 0, Die(2, 8, 8));
        m_dieRangeDb["potion_greater"] = DieRange(8, 8, 8, 8, 8, 12, 1, 1, 0, 0, 0, 0, Die(3, 8, 8));

        m_dieRangeDb["resurrect_lesser"]  = DieRange(2, 2, 8, 8, 1, 4, 1, 1, 0, 0, 0, 0, Die(10, 8, 8));
        m_dieRangeDb["resurrect"]         = DieRange(4, 4, 8, 8, 4, 8, 1, 1, 0, 0, 0, 0, Die(12, 8, 8));
        m_dieRangeDb["resurrect_greater"] = DieRange(8, 8, 8, 8, 8, 12, 1, 1, 0, 0, 0, 0, Die(14, 8, 8));

        m_dieRangeDb["new_hero"]       = DieRange(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Die(10, 10, 10));
        m_dieRangeDb["level_up"]       = DieRange(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Die(10, 10, 10));
        m_dieRangeDb["level_up_5"]     = DieRange(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Die(45, 10, 10));
    }

    // ----------------------------------------------------------------------------------------------------------------
    m_weaponDb.clear();
    {
        m_weaponDb["Empty"] = Weapon("Empty", DamageType::NORMAL, m_dieRangeDb["weapon_unarmed"], Target::ENEMY, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["weapon_gloves"], WeaponType::INVALID);
        m_weaponDb["Unarmed"] = Weapon("Unarmed", DamageType::NORMAL, m_dieRangeDb["weapon_unarmed"], Target::ENEMY, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["weapon_gloves"], WeaponType::INVALID);

        WEAPON_DB_COMMON("Gloves", Target::ENEMY, m_dieRangeDb["weapon_gloves"], m_animationDb["fist_attack"], m_animationDb["weapon_gloves"], WeaponType::GLOVES);
        WEAPON_DB_COMMON("Wand", Target::ENEMY, m_dieRangeDb["weapon_wand"], m_animationDb["blast_attack"], m_animationDb["weapon_wand"], WeaponType::WAND);
        WEAPON_DB_COMMON_HEALING("Wand", Target::PLAYERHP_REUSE, m_dieRangeDb["weapon_wand"], m_animationDb["blast_attack"], m_animationDb["weapon_wand"], WeaponType::WAND);
        WEAPON_DB_COMMON_PROTECT("Wand", Target::PLAYER_PROTECT, m_dieRangeDb["weapon_wand"], m_animationDb["blast_attack"], m_animationDb["weapon_wand"], WeaponType::WAND);
        WEAPON_DB_COMMON("Staff", Target::ALLENEMIES, m_dieRangeDb["weapon_staff"], m_animationDb["blast_attack"], m_animationDb["weapon_staff"], WeaponType::STAFF);
        WEAPON_DB_COMMON_HEALING("Staff", Target::ALLPLAYERSHP, m_dieRangeDb["weapon_staff"], m_animationDb["blast_attack"], m_animationDb["weapon_staff"], WeaponType::STAFF);
        WEAPON_DB_COMMON_PROTECT("Staff", Target::PLAYER_PROTECTALL, m_dieRangeDb["weapon_staff"], m_animationDb["blast_attack"], m_animationDb["weapon_staff"], WeaponType::STAFF);
        WEAPON_DB_COMMON("Dagger", Target::ENEMY, m_dieRangeDb["weapon_dagger"], m_animationDb["slash_attack"], m_animationDb["weapon_dagger"], WeaponType::DAGGER);
        WEAPON_DB_COMMON("Sword", Target::ENEMY, m_dieRangeDb["weapon_sword"], m_animationDb["slash_attack"], m_animationDb["weapon_sword"], WeaponType::SWORD);
        WEAPON_DB_COMMON("Great Sword", Target::ENEMY, m_dieRangeDb["weapon_great_sword"], m_animationDb["slash_attack"], m_animationDb["weapon_great_sword"], WeaponType::GREATSWORD);
        WEAPON_DB_COMMON("Robe", Target::PLAYERAC_SPELL, m_dieRangeDb["armor_robe"], m_animationDb["fist_attack"], m_animationDb["armor_robe"], WeaponType::ROBE);
        WEAPON_DB_COMMON("Leather", Target::PLAYERAC_SPEED, m_dieRangeDb["armor_leather"], m_animationDb["fist_attack"], m_animationDb["armor_leather"], WeaponType::LEATHER);
        WEAPON_DB_COMMON("Plate", Target::PLAYERAC_SLOW, m_dieRangeDb["armor_plate"], m_animationDb["fist_attack"], m_animationDb["armor_plate"], WeaponType::PLATE);

        WEAPON_DB_RARE("Gloves", Target::ENEMY, m_dieRangeDb["weapon_gloves"].As(Rarity::RARE), m_animationDb["fist_attack"], m_animationDb["weapon_rare_gloves"], WeaponType::GLOVES);
        WEAPON_DB_RARE("Wand", Target::ENEMY, m_dieRangeDb["weapon_wand"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_wand"], WeaponType::WAND);
        WEAPON_DB_RARE_HEALING("Wand", Target::PLAYERHP_REUSE, m_dieRangeDb["weapon_wand"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_wand"], WeaponType::WAND);
        WEAPON_DB_RARE_PROTECT("Wand", Target::PLAYER_PROTECT, m_dieRangeDb["weapon_wand"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_wand"], WeaponType::WAND);
        WEAPON_DB_RARE("Staff", Target::ALLENEMIES, m_dieRangeDb["weapon_staff"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_staff"], WeaponType::STAFF);
        WEAPON_DB_RARE_HEALING("Staff", Target::ALLPLAYERSHP, m_dieRangeDb["weapon_staff"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_staff"], WeaponType::STAFF);
        WEAPON_DB_RARE_PROTECT("Staff", Target::PLAYER_PROTECTALL, m_dieRangeDb["weapon_staff"].As(Rarity::RARE), m_animationDb["blast_attack"], m_animationDb["weapon_rare_staff"], WeaponType::STAFF);
        WEAPON_DB_RARE("Dagger", Target::ENEMY, m_dieRangeDb["weapon_dagger"].As(Rarity::RARE), m_animationDb["slash_attack"], m_animationDb["weapon_rare_dagger"], WeaponType::DAGGER);
        WEAPON_DB_RARE("Sword", Target::ENEMY, m_dieRangeDb["weapon_sword"].As(Rarity::RARE), m_animationDb["slash_attack"], m_animationDb["weapon_rare_sword"], WeaponType::SWORD);
        WEAPON_DB_RARE("Great Sword", Target::ENEMY, m_dieRangeDb["weapon_great_sword"].As(Rarity::RARE), m_animationDb["slash_attack"], m_animationDb["weapon_rare_great_sword"], WeaponType::GREATSWORD);
        WEAPON_DB_RARE("Robe", Target::PLAYERAC_SPELL, m_dieRangeDb["armor_robe"].As(Rarity::RARE), m_animationDb["fist_attack"], m_animationDb["armor_rare_robe"], WeaponType::ROBE);
        WEAPON_DB_RARE("Leather", Target::PLAYERAC_SPEED, m_dieRangeDb["armor_leather"].As(Rarity::RARE), m_animationDb["fist_attack"], m_animationDb["armor_rare_leather"], WeaponType::LEATHER);
        WEAPON_DB_RARE("Plate", Target::PLAYERAC_SLOW, m_dieRangeDb["armor_plate"].As(Rarity::RARE), m_animationDb["fist_attack"], m_animationDb["armor_rare_plate"], WeaponType::PLATE);

        WEAPON_DB_EPIC("Gloves", Target::ENEMY, m_dieRangeDb["weapon_gloves"].As(Rarity::EPIC), m_animationDb["fist_attack"], m_animationDb["weapon_epic_gloves"], WeaponType::GLOVES);
        WEAPON_DB_EPIC("Wand", Target::ENEMY, m_dieRangeDb["weapon_wand"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_wand"], WeaponType::WAND);
        WEAPON_DB_EPIC_HEALING("Wand", Target::PLAYERHP_REUSE, m_dieRangeDb["weapon_wand"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_wand"], WeaponType::WAND);
        WEAPON_DB_EPIC_PROTECT("Wand", Target::PLAYER_PROTECT, m_dieRangeDb["weapon_wand"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_wand"], WeaponType::WAND);
        WEAPON_DB_EPIC("Staff", Target::ALLENEMIES, m_dieRangeDb["weapon_staff"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_staff"], WeaponType::STAFF);
        WEAPON_DB_EPIC_HEALING("Staff", Target::ALLPLAYERSHP, m_dieRangeDb["weapon_staff"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_staff"], WeaponType::STAFF);
        WEAPON_DB_EPIC_PROTECT("Staff", Target::PLAYER_PROTECTALL, m_dieRangeDb["weapon_staff"].As(Rarity::EPIC), m_animationDb["blast_attack"], m_animationDb["weapon_epic_staff"], WeaponType::STAFF);
        WEAPON_DB_EPIC("Dagger", Target::ENEMY, m_dieRangeDb["weapon_dagger"].As(Rarity::EPIC), m_animationDb["slash_attack"], m_animationDb["weapon_epic_dagger"], WeaponType::DAGGER);
        WEAPON_DB_EPIC("Sword", Target::ENEMY, m_dieRangeDb["weapon_sword"].As(Rarity::EPIC), m_animationDb["slash_attack"], m_animationDb["weapon_epic_sword"], WeaponType::SWORD);
        WEAPON_DB_EPIC("Great Sword", Target::ENEMY, m_dieRangeDb["weapon_great_sword"].As(Rarity::EPIC), m_animationDb["slash_attack"], m_animationDb["weapon_epic_great_sword"], WeaponType::GREATSWORD);
        WEAPON_DB_EPIC("Robe", Target::PLAYERAC_SPELL, m_dieRangeDb["armor_robe"].As(Rarity::EPIC), m_animationDb["fist_attack"], m_animationDb["armor_epic_robe"], WeaponType::ROBE);
        WEAPON_DB_EPIC("Leather", Target::PLAYERAC_SPEED, m_dieRangeDb["armor_leather"].As(Rarity::EPIC), m_animationDb["fist_attack"], m_animationDb["armor_epic_leather"], WeaponType::LEATHER);
        WEAPON_DB_EPIC("Plate", Target::PLAYERAC_SLOW, m_dieRangeDb["armor_plate"].As(Rarity::EPIC), m_animationDb["fist_attack"], m_animationDb["armor_epic_plate"], WeaponType::PLATE);

        WEAPON_DB_LEGENDARY("Gloves", Target::ENEMY, m_dieRangeDb["weapon_gloves"].As(Rarity::LEGENDARY), m_animationDb["fist_attack"], m_animationDb["weapon_legendary_gloves"], WeaponType::GLOVES);
        WEAPON_DB_LEGENDARY("Wand", Target::ENEMY, m_dieRangeDb["weapon_wand"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_wand"], WeaponType::WAND);
        WEAPON_DB_LEGENDARY_HEALING("Wand", Target::PLAYERHP_REUSE, m_dieRangeDb["weapon_wand"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_wand"], WeaponType::WAND);
        WEAPON_DB_LEGENDARY_PROTECT("Wand", Target::PLAYER_PROTECT, m_dieRangeDb["weapon_wand"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_wand"], WeaponType::WAND);
        WEAPON_DB_LEGENDARY("Staff", Target::ALLENEMIES, m_dieRangeDb["weapon_staff"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_staff"], WeaponType::STAFF);
        WEAPON_DB_LEGENDARY_HEALING("Staff", Target::ALLPLAYERSHP, m_dieRangeDb["weapon_staff"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_staff"], WeaponType::STAFF);
        WEAPON_DB_LEGENDARY_PROTECT("Staff", Target::PLAYER_PROTECTALL, m_dieRangeDb["weapon_staff"].As(Rarity::LEGENDARY), m_animationDb["blast_attack"], m_animationDb["weapon_legendary_staff"], WeaponType::STAFF);
        WEAPON_DB_LEGENDARY("Dagger", Target::ENEMY, m_dieRangeDb["weapon_dagger"].As(Rarity::LEGENDARY), m_animationDb["slash_attack"], m_animationDb["weapon_legendary_dagger"], WeaponType::DAGGER);
        WEAPON_DB_LEGENDARY("Sword", Target::ENEMY, m_dieRangeDb["weapon_sword"].As(Rarity::LEGENDARY), m_animationDb["slash_attack"], m_animationDb["weapon_legendary_sword"], WeaponType::SWORD);
        WEAPON_DB_LEGENDARY("Great Sword", Target::ENEMY, m_dieRangeDb["weapon_great_sword"].As(Rarity::LEGENDARY), m_animationDb["slash_attack"], m_animationDb["weapon_legendary_great_sword"], WeaponType::GREATSWORD);
        WEAPON_DB_LEGENDARY("Robe", Target::PLAYERAC_SPELL, m_dieRangeDb["armor_robe"].As(Rarity::LEGENDARY), m_animationDb["fist_attack"], m_animationDb["armor_legendary_robe"], WeaponType::ROBE);
        WEAPON_DB_LEGENDARY("Leather", Target::PLAYERAC_SPEED, m_dieRangeDb["armor_leather"].As(Rarity::LEGENDARY), m_animationDb["fist_attack"], m_animationDb["armor_legendary_leather"], WeaponType::LEATHER);
        WEAPON_DB_LEGENDARY("Plate", Target::PLAYERAC_SLOW, m_dieRangeDb["armor_plate"].As(Rarity::LEGENDARY), m_animationDb["fist_attack"], m_animationDb["armor_legendary_plate"], WeaponType::PLATE);

        m_weaponDb["Lesser Healing Potion"] = Weapon("Lesser Healing Potion", DamageType::NORMAL, m_dieRangeDb["potion_lesser"], Target::PLAYERHP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["lesser_healing_potion"], WeaponType::POTION);
        m_weaponDb["Lesser Magic Potion"] = Weapon("Lesser Magic Potion", DamageType::NORMAL, m_dieRangeDb["potion_lesser"], Target::PLAYERMP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["lesser_magic_potion"], WeaponType::POTION);
        m_weaponDb["Lesser Resurrect Potion"] = Weapon("Lesser Resurrect Potion", DamageType::NORMAL, m_dieRangeDb["resurrect_lesser"], Target::PLAYERHP_REVIVE_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["lesser_resurrect_potion"], WeaponType::POTION);
        m_weaponDb["Healing Potion"] = Weapon("Healing Potion", DamageType::NORMAL, m_dieRangeDb["potion"], Target::PLAYERHP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["healing_potion"], WeaponType::POTION);
        m_weaponDb["Magic Potion"] = Weapon("Magic Potion", DamageType::NORMAL, m_dieRangeDb["potion"], Target::PLAYERMP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["magic_potion"], WeaponType::POTION);
        m_weaponDb["Resurrect Potion"] = Weapon("Resurrect Potion", DamageType::NORMAL, m_dieRangeDb["resurrect"], Target::PLAYERHP_REVIVE_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["resurrect_potion"], WeaponType::POTION);
        m_weaponDb["Greater Healing Potion"] = Weapon("Greater Healing Potion", DamageType::NORMAL, m_dieRangeDb["potion_greater"], Target::PLAYERHP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["greater_healing_potion"], WeaponType::POTION);
        m_weaponDb["Greater Magic Potion"] = Weapon("Greater Magic Potion", DamageType::NORMAL, m_dieRangeDb["potion_greater"], Target::PLAYERMP_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["greater_magic_potion"], WeaponType::POTION);
        m_weaponDb["Greater Resurrect Potion"] = Weapon("Greater Resurrect Potion", DamageType::NORMAL, m_dieRangeDb["resurrect_greater"], Target::PLAYERHP_REVIVE_CONSUME, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["greater_resurrect_potion"], WeaponType::POTION);
        
        m_weaponDb["New Hero"] = Weapon("New Hero", DamageType::NORMAL, m_dieRangeDb["new_hero"], Target::NEWHERO, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["new_hero"], WeaponType::NEWHERO);
        m_weaponDb["Level Up"] = Weapon("Level Up", DamageType::NORMAL, m_dieRangeDb["level_up"], Target::PLAYERLEVEL, Rarity::COMMON, m_animationDb["fist_attack"], m_animationDb["level_up"], WeaponType::LEVELUP);
        m_weaponDb["Level Up 5"] = Weapon("Level Up 5", DamageType::NORMAL, m_dieRangeDb["level_up_5"], Target::PLAYERLEVEL5, Rarity::EPIC, m_animationDb["fist_attack"], m_animationDb["level_up_5"], WeaponType::LEVELUP5);
    }

    // ----------------------------------------------------------------------------------------------------------------
    m_commonTable.clear();
    m_commonTable =
    {
        WEAPON_TABLE("Gloves", Rarity::COMMON)
        WEAPON_TABLE("Wand", Rarity::COMMON)
        WEAPON_TABLE("Staff", Rarity::COMMON)
        WEAPON_TABLE("Dagger", Rarity::COMMON)
        WEAPON_TABLE("Sword", Rarity::COMMON)
        WEAPON_TABLE("Great Sword", Rarity::COMMON)
        WEAPON_TABLE("Robe", Rarity::COMMON)
        WEAPON_TABLE("Leather", Rarity::COMMON)
        WEAPON_TABLE("Plate", Rarity::COMMON)
        WEAPON_TABLE("Gloves", Rarity::COMMON)
        WEAPON_TABLE_HEALING("Wand", Rarity::COMMON)
        WEAPON_TABLE_HEALING("Staff", Rarity::COMMON)
        WEAPON_TABLE_PROTECT("Wand", Rarity::COMMON)
        WEAPON_TABLE_PROTECT("Staff", Rarity::COMMON)

        m_weaponDb["Lesser Healing Potion"],
        m_weaponDb["Lesser Magic Potion"],
        m_weaponDb["Healing Potion"],
        m_weaponDb["Magic Potion"],

        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["New Hero"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
        m_weaponDb["Level Up"],
    };

    // ----------------------------------------------------------------------------------------------------------------
    m_rareTable.clear();
    m_rareTable =
    {
        WEAPON_TABLE("Gloves", Rarity::RARE)
        WEAPON_TABLE("Wand", Rarity::RARE)
        WEAPON_TABLE("Staff", Rarity::RARE)
        WEAPON_TABLE("Dagger", Rarity::RARE)
        WEAPON_TABLE("Sword", Rarity::RARE)
        WEAPON_TABLE("Great Sword", Rarity::RARE)
        WEAPON_TABLE("Robe", Rarity::RARE)
        WEAPON_TABLE("Leather", Rarity::RARE)
        WEAPON_TABLE("Plate", Rarity::RARE)
        WEAPON_TABLE("Gloves", Rarity::RARE)
        WEAPON_TABLE_HEALING("Wand", Rarity::RARE)
        WEAPON_TABLE_HEALING("Staff", Rarity::RARE)
        WEAPON_TABLE_PROTECT("Wand", Rarity::RARE)
        WEAPON_TABLE_PROTECT("Staff", Rarity::RARE)

        m_weaponDb["Greater Healing Potion"],
        m_weaponDb["Greater Magic Potion"],
        m_weaponDb["Lesser Resurrect Potion"],
        m_weaponDb["Resurrect Potion"],
    };

    // ----------------------------------------------------------------------------------------------------------------
    m_epicTable.clear();
    m_epicTable =
    {
        WEAPON_TABLE("Gloves", Rarity::EPIC)
        WEAPON_TABLE("Wand", Rarity::EPIC)
        WEAPON_TABLE("Staff", Rarity::EPIC)
        WEAPON_TABLE("Dagger", Rarity::EPIC)
        WEAPON_TABLE("Sword", Rarity::EPIC)
        WEAPON_TABLE("Great Sword", Rarity::EPIC)
        WEAPON_TABLE("Robe", Rarity::EPIC)
        WEAPON_TABLE("Leather", Rarity::EPIC)
        WEAPON_TABLE("Plate", Rarity::EPIC)
        WEAPON_TABLE("Gloves", Rarity::EPIC)
        WEAPON_TABLE_HEALING("Wand", Rarity::EPIC)
        WEAPON_TABLE_HEALING("Staff", Rarity::EPIC)
        WEAPON_TABLE_PROTECT("Wand", Rarity::EPIC)
        WEAPON_TABLE_PROTECT("Staff", Rarity::EPIC)

        m_weaponDb["Greater Healing Potion"],
        m_weaponDb["Greater Magic Potion"],

        m_weaponDb["Level Up 5"],
        m_weaponDb["Level Up 5"],

        m_weaponDb["Greater Resurrect Potion"],
    };

    // ----------------------------------------------------------------------------------------------------------------
    m_legendaryTable.clear();
    m_legendaryTable =
    {
        WEAPON_TABLE("Gloves", Rarity::LEGENDARY)
        WEAPON_TABLE("Wand", Rarity::LEGENDARY)
        WEAPON_TABLE("Staff", Rarity::LEGENDARY)
        WEAPON_TABLE("Dagger", Rarity::LEGENDARY)
        WEAPON_TABLE("Sword", Rarity::LEGENDARY)
        WEAPON_TABLE("Great Sword", Rarity::LEGENDARY)
        WEAPON_TABLE("Robe", Rarity::LEGENDARY)
        WEAPON_TABLE("Leather", Rarity::LEGENDARY)
        WEAPON_TABLE("Plate", Rarity::LEGENDARY)
        WEAPON_TABLE("Gloves", Rarity::LEGENDARY)
        WEAPON_TABLE_HEALING("Wand", Rarity::LEGENDARY)
        WEAPON_TABLE_HEALING("Staff", Rarity::LEGENDARY)
        WEAPON_TABLE_PROTECT("Wand", Rarity::LEGENDARY)
        WEAPON_TABLE_PROTECT("Staff", Rarity::LEGENDARY)
    };

    // ----------------------------------------------------------------------------------------------------------------
    m_potionTable.clear();
    m_potionTable =
    {
        m_weaponDb["Lesser Healing Potion"],
        m_weaponDb["Lesser Healing Potion"],
        m_weaponDb["Lesser Healing Potion"],
        m_weaponDb["Lesser Magic Potion"],
        m_weaponDb["Lesser Magic Potion"],
        m_weaponDb["Lesser Magic Potion"],
        m_weaponDb["Healing Potion"],
        m_weaponDb["Healing Potion"],
        m_weaponDb["Magic Potion"],
        m_weaponDb["Magic Potion"],
        m_weaponDb["Lesser Resurrect Potion"],
    };
    // ----------------------------------------------------------------------------------------------------------------

    m_monsterDb.clear();
    {
        m_monsterDb["Blob"] = Monster("Blob", Die(1, 6, 2), Die(0, 0, 0), DamageType::NORMAL, 5, 1, m_animationDb["blob_attack"], m_animationDb["alt_blob_attack"], m_animationDb["blob"], m_animationDb["blob"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"]);
        m_monsterDb["Bat"] = Monster("Bat", Die(1, 4, 2), Die(0, 0, 0), DamageType::NORMAL, 5, 1, m_animationDb["alt_bat_attack"], m_animationDb["alt_bat_attack"], m_animationDb["bat"], m_animationDb["bat"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"]);
        m_monsterDb["Skeleton"] = Monster("Skeleton", Die(1, 6, 2), Die(0, 0, 0), DamageType::NORMAL, 5, 1, m_animationDb["skeleton_attack"], m_animationDb["alt_skeleton_attack"], m_animationDb["skeleton"], m_animationDb["spawn_skeleton"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"]);
        m_monsterDb["Spider"] = Monster("Spider", Die(1, 6, 2), Die(0, 0, 0), DamageType::NORMAL, 5, 1, m_animationDb["alt_spider_attack"], m_animationDb["spider_attack"], m_animationDb["spider"], m_animationDb["spider"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"]);
        m_monsterDb["Dragon"] = Monster("Dragon", Die(2, 20, 2), Die(0, 0, 0), DamageType::NORMAL, 5, 1, m_animationDb["dragon_attack"], m_animationDb["dragon_attack"], m_animationDb["dragon"], m_animationDb["dragon"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"], m_weaponDb["Empty"]);
    }

    //-----------------------------------------------------------------------------------------------------------------
    m_passiveDb.clear();
    {
        m_passiveDb[PassiveType::GLOVES_BASH]           = Passive("Rabbit Punch",      Rarity::COMMON, m_animationDb["weapon_gloves"].As(0x0007, 0x0006, 0x0001),      0, 0, 0, "Gloves have slight chance to stun opponent.");
        m_passiveDb[PassiveType::GLOVES_PICKPOCKET]     = Passive("5 Finger Discount", Rarity::COMMON, m_animationDb["weapon_gloves"].As(0x0007, 0x0008, 0x0002),      1, 0, 0, "Gloves have chance to steal gold on hit when equipped with leather.");
        m_passiveDb[PassiveType::GLOVES_BOXER]          = Passive("Boxing",            Rarity::COMMON, m_animationDb["weapon_gloves"].As(0x0007, 0x000E, 0x0003),      2, 0, 0, "Gloves swing once per level if equipped with two gloves.");
        m_passiveDb[PassiveType::DAGGER_DUALWIELD]      = Passive("Dual Wield",        Rarity::COMMON, m_animationDb["weapon_dagger"].As(0x0007, 0x000F, 0x0004),      3, 0, 0, "Attack with both weapons so as long only Daggers or Swords are equipped in both hands.");
        m_passiveDb[PassiveType::DAGGER_BLEED]          = Passive("Bled Dry",          Rarity::COMMON, m_animationDb["weapon_dagger"].As(0x0007, 0x0006, 0x0005),      4, 0, 0, "Daggers have chance to add bleed condition to enemy.");
        m_passiveDb[PassiveType::DAGGER_MULTIATTACK]    = Passive("Nimble Fingers",    Rarity::COMMON, m_animationDb["weapon_dagger"].As(0x0007, 0x0008, 0x0006),      5, 0, 0, "Daggers attack twice.");
        m_passiveDb[PassiveType::SWORD_PARRY]           = Passive("Parry Master",      Rarity::COMMON, m_animationDb["weapon_sword"].As(0x0007, 0x000E, 0x0007),       0, 1, 0, "Swords have chance to parry incoming attacks.");
        m_passiveDb[PassiveType::SWORD_RIPOSTE]         = Passive("Riposte",           Rarity::COMMON, m_animationDb["weapon_sword"].As(0x0007, 0x000F, 0x0009),       1, 1, 0, "Use equipped swords when attacked.");
        m_passiveDb[PassiveType::SWORD_BATTLEWIZARD]    = Passive("Battle Wizard",     Rarity::COMMON, m_animationDb["weapon_sword"].As(0x0007, 0x0006, 0x000B),       2, 1, 0, "Equipping sword with staff deals additional damage.");
        m_passiveDb[PassiveType::GREATSWORD_HEAVYSWING] = Passive("Heavy Swing",       Rarity::COMMON, m_animationDb["weapon_great_sword"].As(0x0007, 0x0008, 0x000C), 3, 1, 0, "Great sword speed decreased, great axes deal extra multi per level.");
        m_passiveDb[PassiveType::GREATSWORD_ALONE]      = Passive("Speciality",        Rarity::COMMON, m_animationDb["weapon_great_sword"].As(0x0007, 0x000E, 0x000D), 4, 1, 0, "Equipping only a great sword deals 2 times multi damage.");
        m_passiveDb[PassiveType::GREATSWORD_HURT]       = Passive("Masochist",         Rarity::COMMON, m_animationDb["weapon_great_sword"].As(0x0007, 0x000F, 0x000E), 5, 1, 0, "Every attack received this turn to hero equipping great swords doubles multi.");
        m_passiveDb[PassiveType::WAND_QUICKSPELL]       = Passive("Quick Spell",       Rarity::COMMON, m_animationDb["weapon_wand"].As(0x0007, 0x0006, 0x000F),        0, 0, 1, "Wands and Staves gain -2 speed.");
        m_passiveDb[PassiveType::WAND_ELEMENTALMASTER]  = Passive("Elemental Master",  Rarity::COMMON, m_animationDb["weapon_wand"].As(0x0007, 0x0008, 0x0001),        1, 0, 1, "Wands add 2x level to multi when eqipped with Robe.");
        m_passiveDb[PassiveType::WAND_FINESSE]          = Passive("Wand Finesse",      Rarity::COMMON, m_animationDb["weapon_wand"].As(0x0007, 0x000E, 0x0002),        2, 0, 1, "Wands have chance to attack twice.");
        m_passiveDb[PassiveType::STAFF_ELEMENTALMASTER] = Passive("Elemental Master",  Rarity::COMMON, m_animationDb["weapon_staff"].As(0x0007, 0x000F, 0x0003),       3, 0, 1, "Staffs deal twice as much multi but are slower.");
        m_passiveDb[PassiveType::STAFF_IGNOREELEMENT]   = Passive("Potent Spells",     Rarity::COMMON, m_animationDb["weapon_staff"].As(0x0007, 0x0006, 0x0004),       4, 0, 1, "Staffs ignore elemental weaknesses.");
        m_passiveDb[PassiveType::STAFF_MAGICSHIELD]     = Passive("Magical Bubble",    Rarity::COMMON, m_animationDb["weapon_staff"].As(0x0007, 0x0008, 0x0005),       5, 0, 1, "Equipping robes with staff adds staff damage to mitigation as protective bubble.");
        m_passiveDb[PassiveType::PLATE_TAUNT]           = Passive("Taunt",             Rarity::COMMON, m_animationDb["armor_plate"].As(0x0007, 0x000E, 0x0006),        0, 0, 2, "Add higher chance for monsters to aim for plate wearers.");
        m_passiveDb[PassiveType::PLATE_HPBOOST]         = Passive("Good Health",       Rarity::COMMON, m_animationDb["armor_plate"].As(0x0007, 0x000F, 0x0007),        1, 0, 2, "Double the heroes hp per level when equipped with plate.");
        m_passiveDb[PassiveType::PLATE_IGNOREDAMAGE]    = Passive("Craftmenship",      Rarity::COMMON, m_animationDb["armor_plate"].As(0x0007, 0x0006, 0x0009),        2, 0, 2, "Change to ignore damage when equipping plate.");
        m_passiveDb[PassiveType::LEATHER_GOLD]          = Passive("Good Eye",          Rarity::COMMON, m_animationDb["armor_leather"].As(0x0007, 0x0008, 0x000A),      3, 0, 2, "Add X gold per monster slain. X is rarity of leather equipped.");
        m_passiveDb[PassiveType::LEATHER_SCOUT]         = Passive("Scout Ahead",       Rarity::COMMON, m_animationDb["armor_leather"].As(0x0007, 0x000E, 0x000B),      4, 0, 2, "See behind doors depending on how many heroes equip leather [1:Shop, 2:Traps, 4:Fountains and Monsters].");
        m_passiveDb[PassiveType::LEATHER_DAMAGE]        = Passive("Nimble",            Rarity::COMMON, m_animationDb["armor_leather"].As(0x0007, 0x000F, 0x000C),      5, 0, 2, "Equipping Leather makes Daggers and Swords deal X extra multi. X is Level times Rarity.");
        m_passiveDb[PassiveType::ROBE_BACKLINE]         = Passive("Camoflage",         Rarity::COMMON, m_animationDb["armor_robe"].As(0x0007, 0x0006, 0x000D),         0, 1, 2, "Robes make the wearer less threatening to monsters.");
        m_passiveDb[PassiveType::ROBE_GLASSCANNON]      = Passive("Glass Cannon",      Rarity::COMMON, m_animationDb["armor_robe"].As(0x0007, 0x0008, 0x000E),         1, 1, 2, "As long as hero is wearing Robes, magical damage is doubled. Damage recieved is doubled.");
        m_passiveDb[PassiveType::ROBE_MPCOST]           = Passive("Budget Spellcraft", Rarity::COMMON, m_animationDb["armor_robe"].As(0x0007, 0x000E, 0x000F),         2, 1, 2, "As long as hero is wearing Robes, the cost of performing magical attacks is reduced.");


        //m_passiveDb[PassiveType::SWORD_SHIELD]          = Passive("Shield Training",   Rarity::COMMON, m_animationDb["weapon_sword"].As(0x000A),       2, 1, 0, "Swords deal twice as much damage with a shield.");

        /// Trap Rewards -- Succeeding traps yeilds rewards
        /// Suprise Round -- Small chance that enemies don't attack on round 1
        /// Test Subject -- Every potion consumed adds 1 AC
        /// Fairy Weapons -- Fairy Weapons now drop from rewards
        /// Artifact Weapons -- Adds Artifact Quality Weapons
        /// Time Traveler -- Go back upstairs 5 floors
        /// Elemental Potency I -- Elemental bonus increased to 4x damage
        /// Elemental Potency II -- Elemental bonus increased to 8x damage
        /// Hobbiest -- Increase Multi for Every Weapon purchased
        /// Layered Armor -- Armor is equipped over one another
        /// Skeleton Slayer I -- Deal double damage to skeletons
        /// Skeleton Slayer II -- Deal quadruple damage to skeletons
        /// Spider Slayer I -- Deal double damage to spiders
        /// Spider Slayer II -- Deal quadruple damage to spiders
        /// Blob Slayer I -- Deal double damage to blobs
        /// Blob Slayer II -- Deal quadruple damage to blobs
        /// Bat Slayer I -- Deal double damage to bats
        /// Bat Slayer II -- Deal quadruple damage to bats
        /// Hedge Fund I -- Add +1 multi for every 50 gold
        /// Hedge Fund II -- Add +1 multi for every 10 gold
        /// Double Rewards -- Incrase rewards by 1 Item
        /// Big Money --- Incrase gold rewards by double
        /// Elite Slayer I --- Deal double damage to Rare or above monsters
        /// Elite Slayer II --- Deal quadruple damage to Rare or above monsters
        /// Elite Slayer III --- Deal octuple damage to Rare or above monsters
        /// Slow Traps --- Traps are twice as easy to succeed
        /// Keen Eye --- Shops allow items to be locked for purchase in next shop


        //m_passiveDb[PassiveType::GLOVES_BASH].animation
    }
}

void Database::Reset()
{
    m_imageDb.clear();
    m_frameDb.clear();
    m_animationDb.clear();
    m_weaponDb.clear();
    m_monsterDb.clear();
    m_commonTable.clear();
    m_rareTable.clear();
    m_epicTable.clear();
    m_legendaryTable.clear();
    m_potionTable.clear();
}
