///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HighScore.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)
//
// High-score tracking for a single run plus persisted-list management.
// File: %LOCALAPPDATA%\ASCIIDungeonCrawl\highscores.txt (pipe-delimited, one entry per line).
//
// Visible columns in the UI:  Floor, Monsters Killed (total), Max Hero Level.
// Other fields are persisted for later balance analysis (not displayed yet).

#pragma once

#include <string>
#include <vector>

// Per-run statistics. Populated incrementally during play; finalised when the party wipes.
struct RunStats
{
    std::string initials = "AAA";

    int floor = 0;

    // Monster kills broken down by rarity (Common=1 ... Legendary=4)
    int killsCommon    = 0;
    int killsRare      = 0;
    int killsEpic      = 0;
    int killsLegendary = 0;

    int maxDamage   = 0; // highest single-hit damage dealt by any hero this run
    int shopsFound  = 0;
    int trapsFound  = 0;

    // Up to 4 entries, one per hero who joined the party at any point. Order matches party order.
    std::vector<int> heroLevels;

    // Names of passives picked during this run (display names from the Passive database).
    std::vector<std::string> passiveNames;

    // Per-hero inventory snapshot at end-of-run. One entry per hero, slash-separated:
    //   weapon1 / weapon2 / weapon3 / weapon4 / armor
    std::vector<std::string> inventory;

    int TotalKills() const { return killsCommon + killsRare + killsEpic + killsLegendary; }

    int MaxLevel() const
    {
        int m = 0;
        for (int l : heroLevels)
            if (l > m) m = l;
        return m;
    }
};

// Ordering for high-score ranking: Floor > Monsters Killed > Max Hero Level.
// Returns true if `a` should rank higher than `b`.
inline bool RunStatsBetter(const RunStats& a, const RunStats& b)
{
    if (a.floor       != b.floor)       return a.floor       > b.floor;
    int ak = a.TotalKills(), bk = b.TotalKills();
    if (ak            != bk)            return ak            > bk;
    return a.MaxLevel() > b.MaxLevel();
}

// Persisted high-score table. Top N entries are kept; older lower-ranked entries are dropped.
class HighScoreFile
{
public:
    static constexpr int kMaxScores = 10;

    // Load from %LOCALAPPDATA%\ASCIIDungeonCrawl\highscores.txt. Returns true on success
    // (a missing file is considered "successful load of an empty table").
    bool Load();

    // Persist to disk. Returns true on success.
    bool Save();

    // Insert a new run; the table is then re-sorted and truncated to kMaxScores.
    // Returns the 1-based rank the new entry landed at (1 = top). Returns 0 if it didn't
    // make the cut.
    int Insert(const RunStats& stats);

    const std::vector<RunStats>& GetAll() const { return m_scores; }

    // Overwrite the initials of the entry at 1-based rank (e.g. after the player edits "AAA").
    // No-op if rank is out of range.
    void SetInitialsAt(int rank, const std::string& initials)
    {
        if (rank < 1 || rank > (int)m_scores.size()) return;
        m_scores[rank - 1].initials = initials;
    }

    // The full path of the high-score file, exposed for diagnostics.
    static std::string GetFilePath();

private:
    std::vector<RunStats> m_scores;
};
