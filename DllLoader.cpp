///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DllLoader.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "DllLoader.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

// IDs must match the ones in DungeonCrawl.rc (appended at the bottom).
#define IDR_BLT_DLL  1001
#define IDR_FONT_TTF 1002

static const char* g_lastError = "";
static void SetErr(const char* msg) { g_lastError = msg; }

// Helper: locate the directory the .exe lives in (with trailing slash). Returns empty on failure.
static std::string GetExeDir()
{
    char buf[MAX_PATH] = { 0 };
    if (GetModuleFileNameA(nullptr, buf, MAX_PATH) == 0)
        return std::string();
    std::string path = buf;
    size_t slash = path.find_last_of("\\/");
    if (slash == std::string::npos)
        return std::string();
    return path.substr(0, slash + 1);
}

// Helper: locate an RCDATA resource and return its bytes (via outData / outSize) without copying.
// Returns true on success; the data pointer is owned by the module image and is valid for the
// lifetime of the process.
static bool LockRCData(int id, LPVOID* outData, DWORD* outSize)
{
    HRSRC hRes = FindResourceA(nullptr, MAKEINTRESOURCEA(id), MAKEINTRESOURCEA(10 /* RT_RCDATA */));
    if (!hRes)        return false;
    HGLOBAL hGlobal = LoadResource(nullptr, hRes);
    if (!hGlobal)     return false;
    DWORD size = SizeofResource(nullptr, hRes);
    if (size == 0)    return false;
    LPVOID data = LockResource(hGlobal);
    if (!data)        return false;
    *outData = data;
    *outSize = size;
    return true;
}

// Helper: write a buffer to a file. Caller must ensure the destination directory exists.
// Returns true on success; if the destination already exists and is locked, returns true anyway
// (the caller can still LoadLibrary or use the existing file).
static bool WriteFileBytes(const std::string& filePath, const void* data, DWORD size)
{
    HANDLE hFile = CreateFileA(filePath.c_str(), GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
        return true; // file may be in use; assume previous extract is still there

    DWORD written = 0;
    BOOL  ok      = WriteFile(hFile, data, size, &written, nullptr);
    CloseHandle(hFile);
    return ok && written == size;
}

// --------------------------------------------------------------------------------------------------------------------
bool LoadEmbeddedBearLibTerminal(const char** outError)
{
    g_lastError = "";

    LPVOID data = nullptr;
    DWORD  size = 0;
    if (!LockRCData(IDR_BLT_DLL, &data, &size))
    {
        SetErr("FindResource(RT_RCDATA #1001) failed - DungeonCrawl.rc RCDATA section may not have been compiled into the .exe. Try Clean + Rebuild.");
        if (outError) *outError = g_lastError;
        return false;
    }

    std::string exeDir = GetExeDir();
    if (exeDir.empty())
    {
        SetErr("GetModuleFileName() failed");
        if (outError) *outError = g_lastError;
        return false;
    }
    std::string dllPath = exeDir + "BearLibTerminal.dll";

    if (!WriteFileBytes(dllPath, data, size))
    {
        SetErr("WriteFile() failed while extracting BearLibTerminal.dll");
        if (outError) *outError = g_lastError;
        return false;
    }

    HMODULE hMod = LoadLibraryA(dllPath.c_str());
    if (!hMod)
    {
        DWORD err = GetLastError();
        static char buf[256];
        _snprintf_s(buf, _TRUNCATE,
            "LoadLibrary('%s') failed (GetLastError=%lu). Check DLL architecture matches the .exe (32 vs 64 bit).",
            dllPath.c_str(), err);
        SetErr(buf);
        if (outError) *outError = g_lastError;
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
bool ExtractEmbeddedFont(const char** outError)
{
    g_lastError = "";

    LPVOID data = nullptr;
    DWORD  size = 0;
    if (!LockRCData(IDR_FONT_TTF, &data, &size))
    {
        SetErr("FindResource(RT_RCDATA #1002) failed - Consolas-Regular.ttf may not have been embedded. Check DungeonCrawl.rc.");
        if (outError) *outError = g_lastError;
        return false;
    }

    std::string exeDir = GetExeDir();
    if (exeDir.empty())
    {
        SetErr("GetModuleFileName() failed");
        if (outError) *outError = g_lastError;
        return false;
    }
    // Match the path used in terminal_set("font: Resources/Consolas-Regular.ttf, ...").
    std::string resDir  = exeDir + "Resources";
    std::string fontPath = resDir + "\\Consolas-Regular.ttf";

    CreateDirectoryA(resDir.c_str(), nullptr); // ignore "already exists"

    if (!WriteFileBytes(fontPath, data, size))
    {
        SetErr("WriteFile() failed while extracting Consolas-Regular.ttf");
        if (outError) *outError = g_lastError;
        return false;
    }
    return true;
}
