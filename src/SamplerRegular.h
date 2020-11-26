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
		int n = getNumSamples();

		float x = s / sqrt(n);
		float y = s / sqrt(n);

		float m = float(sqrt(n)); //m is square root of n

		float i = (x + 0.5) / m;
		float j = (y + 0.5) / m;

		return Vec2f(i, j);

	}
};
