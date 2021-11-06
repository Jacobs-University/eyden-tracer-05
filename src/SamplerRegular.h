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
		// n - number of samples
		int n = getNumSamples();
		float i = ((s % n) + 0.5) / float(sqrt(n));
        float j = ((s / sqrt(n)) + 0.5) / float(sqrt(n));
		// return Vec2f::all(0.5f);
		return Vec2f(j, i);
	}
};
