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
		float i = ((s / sqrt(n)) + 0.5) / float(sqrt(n));
		float j = ((s % n) + 0.5) / float(sqrt(n));
		return Vec2f(i,j); 
	}
};
