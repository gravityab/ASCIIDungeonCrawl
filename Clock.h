///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Clock.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Time.h"

/// Standard Template Library Headers
#include <chrono>

/// Simple steady clock
using ClockImpl = std::conditional_t<
	std::chrono::high_resolution_clock::is_steady,
	std::chrono::high_resolution_clock,
	std::chrono::steady_clock>;

/// A class representing a simple monotonic clock
class Clock
{
public:
	/// Get the amount time since last time the clock was restarted
	Time GetElapsedTime() const;

	/// Returns whether the clock is running
	bool IsRunning() const;

	/// Start the clock
	void Start();

	/// Stop the clock
	void Stop();

	/// Puts time counter back to zero and keeps clock running
	Time Restart();

	/// Puts time counter back to zero and pauses clock
	Time Reset();

private:
	/// Time of last reset
	ClockImpl::time_point m_refPoint{ ClockImpl::now() };

	/// Time of last stop
	ClockImpl::time_point m_stopPoint;
};