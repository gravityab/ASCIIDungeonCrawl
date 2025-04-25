///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sleep.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

#include "Sleep.h"

/// Standard template library headers
#include <chrono>
#include <thread>

/// Sleep a specific amount of time
void SleepTime(Time time)
{
	if (time != Time::Zero)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(time.AsMilliseconds()));
	}
}