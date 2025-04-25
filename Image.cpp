///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Image.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

/// Dungeon Crawl Library Headers
#include "Image.h"

/// Engine Library Headers
#include "Random.h"

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