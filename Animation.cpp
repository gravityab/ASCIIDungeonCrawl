///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animation.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#include "Animation.h"

/// Engine Library Headers
#include "Random.h"

/// Standard Template Library Header
#include <stdint.h>

// --------------------------------------------------------------------------------------------------------------------
Animation::Animation()
{}

Animation::Animation(std::vector<Frame> frames, int fpt, bool repeat, bool animate, bool random)
{
	SetData(frames, fpt, repeat, animate, random);
}

Animation::Animation(std::vector<Frame> frames, int minFpt, int maxFpt, bool repeat, bool animate, bool random)
{
	SetData(frames, minFpt, maxFpt, repeat, animate, random);
}

void Animation::WriteData(ConsoleEntity& entity, Time delta, int x, int y, bool& complete)
{
	if (!m_animating)
		m_frames[m_currentIndex].WriteData(entity, x, y);

	m_currentTime += delta;
	if (m_frameRate == Time::Zero || m_currentTime < m_frameRate)
	{
		m_frames[m_currentIndex].WriteData(entity, x, y);
		return;
	}

	m_currentTime = ToMicroseconds(m_currentTime.AsMicroseconds() % m_frameRate.AsMicroseconds());

	if (m_random && m_frames.size() > 1)
	{
		m_currentIndex = GetRandomValue(0, (int)m_frames.size() - 1);
	}
	else
	{
		if (m_currentIndex + 1 <= (int)m_frames.size())
			m_currentIndex++;
		if (m_currentIndex == m_frames.size())
		{
			complete = true;
			if (m_repeat)
				m_currentIndex = 0;
			else
			{
				m_animating = false;
				m_currentIndex--;
			}

			if (m_maxFrameRate != Time::Zero)
			{
				m_frameRate = ToMilliseconds(GetRandomValue(m_minFrameRate.AsMilliseconds(), m_maxFrameRate.AsMilliseconds()));
			}
		}
	}
	m_frames[m_currentIndex].WriteData(entity, x, y);
}

void Animation::SetData(std::vector<Frame> frames, int fpt, bool repeat, bool animate, bool random)
{
	m_frames = frames;
	m_repeat = repeat;
	m_animating = animate;
	m_random = random;

	if (fpt > 0)
		m_frameRate = ToMilliseconds(fpt);
	else
		m_frameRate = Time::Zero;
}

void Animation::SetData(std::vector<Frame> frames, int minFpt, int maxFpt, bool repeat, bool animate, bool random)
{
	m_frames = frames;
	m_frameRate = ToMilliseconds(GetRandomValue(minFpt, maxFpt));
	m_repeat = repeat;
	m_animating = animate;
	m_random = random;

	if (minFpt > 0)
		m_minFrameRate = ToMilliseconds(minFpt);
	else
		m_minFrameRate = Time::Zero;

	if (maxFpt > 0)
		m_minFrameRate = ToMilliseconds(maxFpt);
	else
		m_minFrameRate = Time::Zero;
}

void Animation::SetAttribute(int frame, int image, uint16_t attribute)
{
	m_frames[frame].SetAttribute(image, attribute);
}

void Animation::SetAttributes(int image, uint16_t attribute)
{
	for (auto&& frame : m_frames)
	{
		frame.SetAttribute(image, attribute);
	}
}

void Animation::SetStrobe(int image, bool strobe)
{
	for (auto&& frame : m_frames)
	{
		frame.SetStrobe(image, strobe);
	}
}

void Animation::SetFramesPerTick(int fpt)
{
	if (fpt > 0)
		m_frameRate = ToMilliseconds(fpt);
	else
		m_frameRate = Time::Zero;
}

bool Animation::IsAnimating() const
{
	return m_animating;
}

void Animation::SetAnimating(bool animate)
{
	m_animating = animate;
}

void Animation::Reset()
{
	m_currentIndex = 0;
	m_currentTime = Time::Zero;
}
