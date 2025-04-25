///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Standard Template Library Headers
#include <assert.h>
#include <chrono>
#include <cstdint>

/// Class representing the passage of time
class Time
{
public:
	/// Constructor for time
	constexpr Time() = default;

	/// Constructor for time given predefined settings
	template <typename Rep, typename Period>
	constexpr Time(const std::chrono::duration<Rep, Period>& duration)
		: m_microseconds(duration)
	{}

	/// Convert time as seconds
	constexpr float AsSeconds() const;

	/// Convert time as milliseconds
	constexpr std::int32_t AsMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::duration<std::int32_t, std::milli>>(m_microseconds).count();
	}

	/// Convert time as microseconds
	constexpr std::int64_t AsMicroseconds() const
	{
		return m_microseconds.count();
	}

	/// Convert time to a duration
	constexpr std::chrono::microseconds ToDuration() const;

	/// Get the duration by default
	template <typename Rep, typename Period>
	constexpr operator std::chrono::duration<Rep, Period>() const
	{
		return m_microseconds;
	}

	/// Predefined "zero" time value
	static const Time Zero;

private:
	/// Time value stored as microseconds
	std::chrono::microseconds m_microseconds{};
};

/// Convert amount to seconds
constexpr Time ToSeconds(float amount)
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(amount));
}

/// Convert amount to milliseconds
constexpr Time ToMilliseconds(std::int32_t amount)
{
	return std::chrono::milliseconds(amount);
}

/// Convert amount to microseconds
constexpr Time ToMicroseconds(std::int64_t amount)
{
	return std::chrono::microseconds(amount);
}

/// Check if two time values are equal
constexpr bool operator==(Time left, Time right)
{
	return left.AsMicroseconds() == right.AsMicroseconds();
}

/// Check if two time values are not equal
constexpr bool operator!=(Time left, Time right)
{
	return left.AsMicroseconds() != right.AsMicroseconds();
}

/// Check if time value is less than another
constexpr bool operator<(Time left, Time right)
{
	return left.AsMicroseconds() < right.AsMicroseconds();
}

/// Check if time value is greater than another
constexpr bool operator>(Time left, Time right)
{
	return left.AsMicroseconds() > right.AsMicroseconds();
}

/// Check if time value is less than or equal than another time value
constexpr bool operator<=(Time left, Time right);

/// Check if time value is greater than or equal to another time value
constexpr bool operator>=(Time left, Time right);

/// Subtract a time value from a time value
constexpr Time operator-(Time right);

/// Add a time value to a time value
constexpr Time operator+(Time left, Time right)
{
	return ToMicroseconds(left.AsMicroseconds() + right.AsMicroseconds());
}

/// Add a time value to an existing time value
constexpr Time& operator+=(Time& left, Time right)
{
	return left = left + right;
}

/// Subtract a time value from a time value
constexpr Time operator-(Time left, Time right)
{
	return ToMicroseconds(left.AsMicroseconds() - right.AsMicroseconds());
}

/// Subtract a time value from an existing time value
constexpr Time& operator-=(Time& left, Time right)
{
	return left = left - right;
}

/// Multiply a time value
constexpr Time operator*(Time left, float right);

/// Multiply a time value
constexpr Time operator*(Time left, std::int64_t right);

/// Multiply a time value
constexpr Time operator*(float left, Time right);

/// Multiply a time value by another time value
constexpr Time operator*(std::int64_t left, Time right);

/// Multiply a time value to an existing time value
constexpr Time& operator*=(Time& left, float right);

/// Multiply a time value to an existing time value
constexpr Time& operator*=(Time& left, std::int64_t right);

/// Divide a time value
constexpr Time operator/(Time left, float right);

/// Divide a time value
constexpr Time operator/(Time left, std::int64_t right);

/// Divide a time value from an existing time value
constexpr Time& operator/=(Time& left, float right);

/// Divide a time value from an existing time value
constexpr Time& operator/=(Time& left, std::int64_t right);

/// Divide a time value by another time value
constexpr float operator/(Time left, Time right);

/// Modulus a time value by a time value
constexpr Time operator%(Time left, Time right)
{
	assert(right.AsMicroseconds() != 0 && "Time::operator% cannot modulus by 0");
	return ToMicroseconds(left.AsMicroseconds() % right.AsMicroseconds());
}

/// Modulus a time value by an existing time value
constexpr Time& operator%=(Time& left, Time right)
{
	assert(right.AsMicroseconds() != 0 && "Time::operator%= cannot modulus by 0");
	return left = left % right;
}
