///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Image.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "Image.h"

/// Engine Library Headers
#include "Random.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>  // GetTickCount64 for the wall-clock trailing animation

// --------------------------------------------------------------------------------------------------------------------
Image::Image()
{}

Image::Image(const char* data, int x, int y, int w, int h, uint16_t attribute)
{
	SetData(data, x, y, w, h, attribute);
}

void Image::WriteData(ConsoleEntity& entity, int x, int y)
{
	if (m_strobe)
	{
		if (m_tick++ > 100)
		{
			static uint16_t s_attributes[14] = { 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F };
			m_attribute = s_attributes[GetRandomValue(0, 13)];
			m_tick = 0;
		}
	}

	if (m_trailing)
	{
		// Wall-clock so the effect runs at the same speed no matter the framerate. All trailing
		// images share the same global clock so they bob in phase with each other (intentional -
		// makes a group of ethereal sprites feel like one breathing scene rather than chaos).
		const uint64_t now = GetTickCount64();

		// Sine-style vertical bob with discrete 1-cell steps every 2 seconds.
		// Cycle: [0,2s)=0, [2,4s)=-1, [4,6s)=0, [6,8s)=+1, then repeats.
		const uint64_t bobPhase = now % 8000ull;
		int bob = 0;
		if      (bobPhase < 2000) bob =  0;
		else if (bobPhase < 4000) bob =  0;
		else if (bobPhase < 6000) bob =  0;
		else                      bob =  0;

		// Trailing wisps: each wisp lives 1/3 second, sliding from the image's origin to
		// 3 cells left while fading 255 -> 0. A new wisp spawns every (lifetime / 3) ms so
		// up to 3 are visible at once, forming a continuous fading tail.
		const uint64_t WISP_LIFE_MS    = 1000ull;
		const uint64_t SPAWN_INTERVAL  = WISP_LIFE_MS / 3ull; // 111ms -> 3 concurrent wisps
		const int      WISP_COUNT      = 2;
		const int      WISP_MAX_OFFSET = 3;

		for (int i = 0; i < WISP_COUNT; ++i)
		{
			// Each wisp is offset in time by SPAWN_INTERVAL so they're evenly staggered.
			const uint64_t wispAge = (now + (uint64_t)i * SPAWN_INTERVAL) % WISP_LIFE_MS;

			// Move left (0 .. -3) and fade (255 .. 0) linearly over the wisp's lifetime.
			int xOffset = -(int)((wispAge * (uint64_t)WISP_MAX_OFFSET) / WISP_LIFE_MS);
			const int  alphaI  = 255 - (int)((wispAge * 255ull) / WISP_LIFE_MS);
			const uint8_t alpha = (uint8_t)(alphaI < 0 ? 0 : (alphaI > 255 ? 255 : alphaI));

			if (xOffset == 0 || alpha == 0)
				continue; // skip the spawn moment - the main image below covers that frame

			entity.WriteData(m_data.data(), m_x + x + xOffset, m_y + y + bob, m_w, m_h, m_attribute, alpha);
            entity.WriteData(m_data.data(), m_x + x - xOffset, m_y + y + bob, m_w, m_h, m_attribute, alpha);
		}

		// Main image: always full opacity at the (possibly bobbed) origin.
		entity.WriteData(m_data.data(), m_x + x, m_y + y + bob, m_w, m_h, m_attribute);
		return;
	}

	entity.WriteData(m_data.data(), m_x + x, m_y + y, m_w, m_h, m_attribute);
}

void Image::SetData(const char* data, int x, int y, int w, int h, uint16_t attribute)
{
	m_data = data;
	m_x = x;
	m_y = y;
	m_w = w;
	m_h = h;
	m_attribute = attribute;
}

void Image::SetAttribute(uint16_t attribute)
{
	m_attribute = attribute;
}

void Image::SetStrobe(bool strobe)
{
	m_strobe = true;
	m_tick = 0;
}

void Image::SetTrailing(bool trailing)
{
	m_trailing = trailing;
}