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
		//get num of samples
		int samples = getNumSamples();
		//get square root
		float sq = float(sqrt(samples));

		float x = s /sq;
		float y = s /sq;
		float n = (x + 0.5) /sq;
		float m = (y + 0.5) /sq;
		//return samples covering a unit square.
		return Vec2f(n, m);
	}
};
