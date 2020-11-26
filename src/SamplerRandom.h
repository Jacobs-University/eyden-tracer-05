// Random Sampler class
// Written by Sergey Kosov in 2019 for Project X
#pragma once

#include "Sampler.h"

// ================================ Random Sampler Class ================================
/**
* @brief Random Sampler class
* @details Generates random samples uniformly covering a region [0; 1) x [0; 1)
* @author Sergey G. Kosov, sergey.kosov@project-10.de
*/
class CSamplerRandom : public CSampler {
public:
	/**
	* @brief Constructor
	* @param nSamples Square root of number of samples in one series
	*/
	CSamplerRandom(size_t nSamples) : CSampler(nSamples) {}
	virtual ~CSamplerRandom(void) = default;

	virtual Vec2f	getSample(size_t) const override
	{
		// --- PUT YOUR CODE HERE ---
		/*We implemented this method first because it seemed easiest.
		In this method the pixel is sampled by n randomly placed samples
		over the entire screen space so here, we don't have to pay attention
		to stay in a given pixel or whatever.
		Coordinates x and y are randomly generated using the suggested
		method from the assignment from random.h.		*/
		float x = Random::U<float>();
		float y = Random::U<float>();
		return Vec2f(x, y);
	}
};
