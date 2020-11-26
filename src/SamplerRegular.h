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
		int n = getNumSamples();
        int i = s / n;
        int j = s % n;
        int m = sqrt(n);
		return Vec2f((i+0.5)/m, (j+0.5)/n);
	}
};
