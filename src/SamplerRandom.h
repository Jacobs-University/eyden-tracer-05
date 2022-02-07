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
		//for(int i = 0; i < )
		float u = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Vec2f Sample = Vec2f(u, v);
		//std::cout << Sample << std::endl;
		return Sample;
		//return Vec2f::all(1.f); 
	}
};
