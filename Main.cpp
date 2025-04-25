///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "DungeonCrawl.h"

/// Engine Library Headers
#include "Random.h"

/// Standard Template Library Headers
#include <clocale>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

// Windows Library Headers.
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// --------------------------------------------------------------------------------------------------------------------
int main()
{
	SetConsoleTitleA("DungeonCrawl");

	SeedRandomizer();
	std::setlocale(LC_ALL, "en_US.UTF-8");
	
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(stdOut);

	DungeonCrawl dungeon;
	if (!dungeon.Initialize(stdOut, 60))
		printf("ERROR: Failed to initialize dungeon crawler.");

	dungeon.RunLoop();
	dungeon.Shutdown();
	return 0;
}