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
	    size_t n = sqrt(this->getNumSamples());
	    // get i, j from row major format
        float i = (s % getNumSamples()) + 0.5;
        float j = (s / n + 0.5);
		return Vec2f(j / n, i / n);
	}
};
