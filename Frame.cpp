///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Frame.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Frame.h"

// --------------------------------------------------------------------------------------------------------------------
Frame::Frame()
{}

Frame::Frame(std::vector<Image> images)
{
	SetData(images);
}

void Frame::WriteData(ConsoleEntity& entity, int x, int y)
{
	for (auto&& image : m_images)
	{
		image.WriteData(entity, x, y);
	}
}

void Frame::SetData(std::vector<Image> images)
{
	m_images = images;
}

void Frame::SetAttribute(int imageIndex, uint16_t attribute)
{
	m_images[imageIndex].SetAttribute(attribute);
}

void Frame::SetStrobe(int imageIndex, bool strobe)
{
	m_images[imageIndex].SetStrobe(strobe);
}