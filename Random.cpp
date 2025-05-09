///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Random.cpp
// Copyright (C) 2019 Andrew Bellinger (gravityab@gmail.com)

#include "./Random.h"

//---------------------------------------------------------------------------------------------------------------------
namespace
{
    //-----------------------------------------------------------------------------------------------------------------
    // The static instance of a mersenne twister pseudo-random algorithm to use for random number generation.
    static std::mt19937 s_randomAlgorithm;
}

//---------------------------------------------------------------------------------------------------------------------
void SeedRandomizer(uint32_t seed)
{
    s_randomAlgorithm.seed(seed);

}

//---------------------------------------------------------------------------------------------------------------------
uint32_t GetRandomValue(uint32_t min, uint32_t max, RandomDistributionType type)
{
    return RandomValue(min, max, type)();
}

//---------------------------------------------------------------------------------------------------------------------
uint32_t GetNormalValue(uint32_t min, uint32_t max, double mean, double standardDeviation)
{
    // Handle a simple normal distribution.
    std::normal_distribution<double> normal(mean, standardDeviation);

    double value = (double)normal(s_randomAlgorithm);

    // Clamp the result
    if (value < min)
        value = min;
    if (value > max)
        value = max;

    // Return the random clamped value
    return std::lround(value);
}

//---------------------------------------------------------------------------------------------------------------------
RandomValue::RandomValue(uint32_t min, uint32_t max, RandomDistributionType type)
    : m_min(min), m_max(max), m_type(type)
{}

//---------------------------------------------------------------------------------------------------------------------
uint32_t RandomValue::operator()()
{
    if (m_type == DISTRIBUTIONTYPE_UNIFORMED)
    {
        // Handle a simple uniform distribution.
        std::uniform_int_distribution<uint32_t> uniformed(m_min, m_max);
        return uniformed(s_randomAlgorithm);
    }
    else if (m_type == DISTRIBUTIONTYPE_NORMAL)
    {
        // Use the median as the mean.
        double mean = (m_min + m_max) / 2;

        // Keep the min and max about 3 standard deviations away.
        double standardDeviation = (m_max - m_min) / 6;

        // Handle a simple normal distribution.
        std::normal_distribution<double> normal(mean, standardDeviation);

        // Should only generate numbers outside the min and max 0.3% of the time. Otherwise, try again.
        uint32_t value;
        do { value = (uint32_t)normal(s_randomAlgorithm); } while (value < m_min || value > m_max);
        return value;
    }
    return 0;
}