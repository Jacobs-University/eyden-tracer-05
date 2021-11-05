#pragma once

#include "Sampler.h"

class CSamplerRegular : public CSampler
{
public:
	/**
	* @brief Constructor
	* @param nSamples Square root of number of samples in one series
	*/
	CSamplerRegular(size_t nSamples) : CSampler(nSamples) {}
	virtual ~CSamplerRegular(void) = default;

	virtual Vec2f	getSample(size_t s) const
	{
		// --- PUT YOUR CODE HERE ---
		/*
		We implemented this second using the formula
		given for problem 1.
		First, we take the number of samples as described in Sampler.h
		and use this to sample in the middle:
		*/
		int n = getNumSamples();
		float m = sqrt(n);
		float i = s % n;
		float j = s / m;
		float x = ((i + 0.5) / m);
		float y = ((j + 0.5) / m);
		return Vec2f(y, x);
	}
};
