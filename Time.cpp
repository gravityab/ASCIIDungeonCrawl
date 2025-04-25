///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Time.cpp
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)


#include "Time.h"

/// Standard Template Library Headers
#include <ratio>

// --------------------------------------------------------------------------------------------------------------------
constexpr float Time::AsSeconds() const
{
	return std::chrono::duration<float>(m_microseconds).count();
}

//constexpr std::int32_t Time::AsMilliseconds() const
//{
//	return std::chrono::duration_cast<std::chrono::duration<std::int32_t, std::milli>>(m_microseconds).count();
//}

//constexpr std::int64_t Time::AsMicroseconds() const
//{
//	return m_microseconds.count();
//}

constexpr std::chrono::microseconds Time::ToDuration() const
{
	return m_microseconds;
}

// --------------------------------------------------------------------------------------------------------------------
//constexpr Time ToSeconds(float amount)
//{
//	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::duration<float>(amount));
//}

//constexpr Time ToMilliseconds(std::int32_t amount)
//{
//	return std::chrono::milliseconds(amount);
//}

//constexpr Time ToMicroseconds(std::int64_t amount)
//{
//	return std::chrono::microseconds(amount);
//}

//constexpr bool operator==(Time left, Time right)
//{
//	return left.AsMicroseconds() == right.AsMicroseconds();
//}

//constexpr bool operator!=(Time left, Time right)
//{
//	return left.AsMicroseconds() != right.AsMicroseconds();
//}

//constexpr bool operator<(Time left, Time right)
//{
//	return left.AsMicroseconds() < right.AsMicroseconds();
//}

//constexpr bool operator>(Time left, Time right)
//{
//	return left.AsMicroseconds() > right.AsMicroseconds();
//}

constexpr bool operator<=(Time left, Time right)
{
	return left.AsMicroseconds() <= right.AsMicroseconds();
}

constexpr bool operator>=(Time left, Time right)
{
	return left.AsMicroseconds() >= right.AsMicroseconds();
}

constexpr Time operator-(Time right)
{
	return ToMicroseconds(-right.AsMicroseconds());
}

//constexpr Time operator+(Time left, Time right)
//{
//	return ToMicroseconds(left.AsMicroseconds() + right.AsMicroseconds());
//}

//constexpr Time& operator+=(Time& left, Time right)
//{
//	return left = left + right;
//}

//constexpr Time operator-(Time left, Time right)
//{
//	return ToMicroseconds(left.AsMicroseconds() - right.AsMicroseconds());
//}

//constexpr Time& operator-=(Time& left, Time right)
//{
//	return left = left - right;
//}

constexpr Time operator*(Time left, float right)
{
	return ToSeconds(left.AsSeconds() * right);
}

constexpr Time operator*(Time left, std::int64_t right)
{
	return ToMicroseconds(left.AsMicroseconds() * right);
}

constexpr Time operator*(float left, Time right)
{
	return right * left;
}

constexpr Time operator*(std::int64_t left, Time right)
{
	return right * left;
}

constexpr Time& operator*=(Time& left, float right)
{
	return left = left * right;
}

constexpr Time& operator*=(Time& left, std::int64_t right)
{
	return left = left * right;
}

constexpr Time operator/(Time left, float right)
{
	assert(right != 0 && "Time::operator/ cannot divide by 0");
	return ToSeconds(left.AsSeconds() / right);
}

constexpr Time operator/(Time left, std::int64_t right)
{
	assert(right != 0 && "Time::operator/ cannot divide by 0");
	return ToMicroseconds(left.AsMicroseconds() / right);
}

constexpr Time& operator/=(Time& left, float right)
{
	assert(right != 0 && "Time::operator/= cannot divide by 0");
	return left = left / right;
}

constexpr Time& operator/=(Time& left, std::int64_t right)
{
	assert(right != 0 && "Time::operator/= cannot divide by 0");
	return left = left / right;
}

constexpr float operator/(Time left, Time right)
{
	assert(right.AsMicroseconds() != 0 && "Time::operator/ cannot divide by 0");
	return left.AsSeconds() / right.AsSeconds();
}

//constexpr Time operator%(Time left, Time right)
//{
//	assert(right.AsMicroseconds() != 0 && "Time::operator% cannot modulus by 0");
//	return ToMicroseconds(left.AsMicroseconds() % right.AsMicroseconds());
//}

//constexpr Time& operator%=(Time& left, Time right)
//{
//	assert(right.AsMicroseconds() != 0 && "Time::operator%= cannot modulus by 0");
//	return left = left % right;
//}

// --------------------------------------------------------------------------------------------------------------------
constexpr Time Time::Zero;