///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HighScore.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "HighScore.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>

#include <algorithm>
#include <fstream>
#include <sstream>

// --------------------------------------------------------------------------------------------------------------------
// Helpers: filesystem
// --------------------------------------------------------------------------------------------------------------------
static bool DirectoryExists(const std::string& path)
{
    DWORD attrs = GetFileAttributesA(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY);
}

// Mirrors the Tetris convention: %LOCALAPPDATA%\<AppName>\<file>.
static std::string GetUserProfilePath(const std::string& fileName)
{
    PWSTR pszPath = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &pszPath);
    std::string path;
    if (SUCCEEDED(hr))
    {
        int bytes = WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, NULL, 0, NULL, NULL);
        path.resize(bytes - 1);
        WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, &path[0], bytes, NULL, NULL);
        CoTaskMemFree(pszPath);
    }
    path.append("\\ASCIIDungeonCrawl");
    if (!DirectoryExists(path))
        CreateDirectoryA(path.c_str(), nullptr);
    path.append("\\");
    path.append(fileName);
    return path;
}

// --------------------------------------------------------------------------------------------------------------------
// Helpers: split / join for sub-list fields (";" inside a column, "/" inside an inventory slot)
// --------------------------------------------------------------------------------------------------------------------
static std::vector<std::string> Split(const std::string& s, char sep)
{
    std::vector<std::string> out;
    if (s.empty()) return out;
    std::stringstream ss(s);
    std::string tok;
    while (std::getline(ss, tok, sep))
        out.push_back(tok);
    return out;
}

static std::string Join(const std::vector<int>& v, char sep)
{
    std::string out;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i) out += sep;
        out += std::to_string(v[i]);
    }
    return out;
}

static std::string Join(const std::vector<std::string>& v, char sep)
{
    std::string out;
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (i) out += sep;
        out += v[i];
    }
    return out;
}

// --------------------------------------------------------------------------------------------------------------------
// HighScoreFile
// --------------------------------------------------------------------------------------------------------------------
std::string HighScoreFile::GetFilePath()
{
    return GetUserProfilePath("highscores.txt");
}

bool HighScoreFile::Load()
{
    m_scores.clear();
    const std::string path = GetFilePath();

    std::ifstream in(path);
    if (!in.is_open())
        return true; // missing file is fine: empty table

    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty()) continue;

        // Format (all fields separated by '|', sub-lists by ';', inventory slots by '/'):
        //   initials | floor | kc | kr | ke | kl | maxDmg | shops | traps |
        //   heroLevels | passives | inventory
        auto cols = Split(line, '|');
        if (cols.size() < 12) continue; // tolerate older / truncated rows by skipping

        RunStats s;
        s.initials       = cols[0];
        s.floor          = std::atoi(cols[1].c_str());
        s.killsCommon    = std::atoi(cols[2].c_str());
        s.killsRare      = std::atoi(cols[3].c_str());
        s.killsEpic      = std::atoi(cols[4].c_str());
        s.killsLegendary = std::atoi(cols[5].c_str());
        s.maxDamage      = std::atoi(cols[6].c_str());
        s.shopsFound     = std::atoi(cols[7].c_str());
        s.trapsFound     = std::atoi(cols[8].c_str());

        for (const auto& v : Split(cols[9],  ';'))
            s.heroLevels.push_back(std::atoi(v.c_str()));
        s.passiveNames = Split(cols[10], ';');
        s.inventory    = Split(cols[11], ';');

        m_scores.push_back(s);
    }
    in.close();

    std::sort(m_scores.begin(), m_scores.end(), RunStatsBetter);
    if ((int)m_scores.size() > kMaxScores)
        m_scores.resize(kMaxScores);
    return true;
}

bool HighScoreFile::Save()
{
    const std::string path = GetFilePath();
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open())
        return false;

    for (const auto& s : m_scores)
    {
        out  << s.initials       << '|'
             << s.floor          << '|'
             << s.killsCommon    << '|'
             << s.killsRare      << '|'
             << s.killsEpic      << '|'
             << s.killsLegendary << '|'
             << s.maxDamage      << '|'
             << s.shopsFound     << '|'
             << s.trapsFound     << '|'
             << Join(s.heroLevels,   ';') << '|'
             << Join(s.passiveNames, ';') << '|'
             << Join(s.inventory,    ';') << '\n';
    }
    out.close();
    return true;
}

int HighScoreFile::Insert(const RunStats& stats)
{
    m_scores.push_back(stats);
    std::sort(m_scores.begin(), m_scores.end(), RunStatsBetter);

    // Find the rank of the just-inserted run. We identify it by pointer-stable comparison
    // (all the simple fields). If multiple ties exist, the earliest matching one is returned.
    int rank = 0;
    for (size_t i = 0; i < m_scores.size(); ++i)
    {
        const RunStats& s = m_scores[i];
        if (s.floor == stats.floor
            && s.TotalKills() == stats.TotalKills()
            && s.MaxLevel()   == stats.MaxLevel()
            && s.maxDamage    == stats.maxDamage
            && s.initials     == stats.initials)
        {
            rank = (int)i + 1;
            break;
        }
    }

    if ((int)m_scores.size() > kMaxScores)
        m_scores.resize(kMaxScores);

    if (rank > kMaxScores)
        rank = 0;
    return rank;
}
