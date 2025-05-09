///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Random.h
// Copyright (C) 2019 Andrew Bellinger (gravityab@gmail.com)

#pragma once

// Standard Template Library Headers.
#include <chrono>
#include <limits>
#include <memory>
#include <random>
#include <stdint.h>
#include <vector>

//-------------------------------------------------------------------------------------------------------------
// The type of random value to acquire.
enum RandomDistributionType
{
	/// I want to see the min and max values at the same frequency as values near or at the median.
	DISTRIBUTIONTYPE_UNIFORMED = 0,

	/// I want to see values closer to the median at a higher frequency than the min and max.
	DISTRIBUTIONTYPE_NORMAL,
};

///------------------------------------------------------------------------------------------------------------
/// Seed the static global randomizer. Should be called within the programs main function.
///
/// \param[in] seed
///   The seed for the random number algorithm. Can be assigned to a static value for deterministic results.
void SeedRandomizer(uint32_t seed = std::chrono::system_clock::now().time_since_epoch().count());

///------------------------------------------------------------------------------------------------------------
/// Perform a one-time random number generator value between a range using a specific distribution method.
///
/// \param[in] min
///   The minimum value of the randomizer.
/// \param[in] max
///   The maximum value of the randomizer.
/// \param[in] type
///   The distribution method used when selecting a random number. By default it uses uniformed.
uint32_t GetRandomValue(uint32_t min, uint32_t max, RandomDistributionType type = DISTRIBUTIONTYPE_UNIFORMED);

///------------------------------------------------------------------------------------------------------------
/// Perform a one-time rounded random number normal distribution with a given mean and standard deviation. Clamp the value
/// between a min and maximum value.
///
/// \param[in] min
///   The minimum value of the randomizer.
/// \param[in] max
///   The maximum value of the randomizer.
/// \param[in] mean
///   The mean of the the normal distribution
/// \param[in] standardDeviation
///   The number of standard deviations we are from the mean
uint32_t GetNormalValue(uint32_t min, uint32_t max, double mean, double standardDeviation);

//-------------------------------------------------------------------------------------------------------------
// A helper class for acquiring random values between a defined range using a specific distribution method.
// Meant to allow randomizer rules to be saved and replicated for continuous use.
class RandomValue
{
public:
	/// The constructor for acquiring a random value.
	///
	/// If not specified for normal distribution, the median is used as the mean. 
	/// If not specified for normal distribution, the standard deviation used is 20% from the mean.
	///
	/// \param[in] min
	///   The minimum value of the randomizer.
	/// \param[in] max
	///   The maximum value of the randomizer.
	/// \param[in] type
	///   The distribution method used when selecting a random number. By default it uses uniformed.
	RandomValue(uint32_t min, uint32_t max, RandomDistributionType type = DISTRIBUTIONTYPE_UNIFORMED);

	/// Acquire a random value from rules specified upfront.
	///
	/// \return
	///   Returns a random number within the min and max range based on the rules specified.
	uint32_t operator()();

private:
	/// The minimum random value.
	uint32_t m_min = 0;

	/// The maximum random value.
	uint32_t m_max = 0;

	/// The distribution method type of the random value.
	RandomDistributionType m_type;
};
