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
	//Regular Sampling: The Pixel is subdivided into n = m x m equally sized regions, which are sampled in the middle:
	//((i + 0.5) / m, (j + 0.5) / n) for i, j = [0 ..m - 1]
	virtual Vec2f	getSample(size_t s) const 
	{ 
		// --- PUT YOUR CODE HERE ---
		size_t nSamples = static_cast<size_t>(sqrt(getNumSamples()));
		return Vec2f((static_cast<float>(s / nSamples) + 0.5f) / nSamples, (static_cast<float>(s % nSamples) + 0.5f) / nSamples);
		//return Vec2f::all(0.5f); 
	}
};
