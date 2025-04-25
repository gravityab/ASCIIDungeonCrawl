///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Frame.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Image.h"

/// Engine Library Headers
#include "Entity.h"

/// Standard Template Library Headers
#include <vector>

/// Class that represents a frame with a collection of overlaying images in an animation
class Frame
{
public:
	/// Constructor for a frame
	Frame();

	/// Constructor for a frame with predefined images
	Frame(std::vector<Image> images);

public:
	/// Write the frame to an existing console entity
	void WriteData(ConsoleEntity& entity, int x, int y);

	/// Initialize the frame with a set of images
	void SetData(std::vector<Image> images);

	/// Set the attribute of a given image within a frame
	void SetAttribute(int imageIndex, uint16_t attribute);

	/// Set the attribute to strobe every color
	void SetStrobe(int imageIndex, bool strobe);

private:
	/// The list of overlaying images associated with a frame
	std::vector<Image> m_images;
};
