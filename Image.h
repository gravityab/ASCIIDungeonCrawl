///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Image.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Engine Library Headers
#include "Entity.h"

/// Standard Template Library Includes
#include <vector>
#include <stdint.h>

/// Class represting a single image to be added to frames within an animation
class Image
{
public:
	/// Constructor for an image
	Image();

	/// Constructor for an image with predefined data
	Image(const char* data, int x, int y, int w, int h, uint16_t attribute);

public:
	/// Write the image data to a console entity
	void WriteData(ConsoleEntity& entity, int x, int y);

	/// Define the data associated with an image with relative offsets and attributes
	void SetData(const char* data, int x, int y, int w, int h, uint16_t attribute);

	/// Set the attribute of the given image
	void SetAttribute(uint16_t attribute);

	/// Set strobe color effect
	void SetStrobe(bool strobe);

private:
	/// The data of the image
	std::string m_data;

	/// The width of the image data
	int m_w = 0;

	/// The height of the image data
	int m_h = 0;

	/// The relative x position to write the data to
	int m_x = 0;

	/// The relative y position to write the data to
	int m_y = 0;

	/// The attribute to display the image as
	uint16_t m_attribute;

	/// Whether to change color every 100 ticks
	bool m_strobe = false;

	/// How many ticks have gone by
	int m_tick = 0;
};
