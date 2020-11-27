// StratifiedSampler class
// Written by Sergey Kosov in 2019 for Project X
#pragma once

#include "Sampler.h"

// ================================ Stratified Sampler Class ================================
/**
* @brief Stratified Sampler class
* @note This class is not thread-safe
* @author Sergey G. Kosov, sergey.kosov@project-10.de
*/
class CSamplerStratified : public CSampler {
public:
	/**
	* @brief Constructor
	* @param nSamples Square root of number of samples in one series
	*/
	CSamplerStratified(size_t nSamples) : CSampler(nSamples) {}
	virtual ~CSamplerStratified(void) = default;

	virtual Vec2f	getSample(size_t s) const
	{
		// --- PUT YOUR CODE HERE ---
		//get num of samples
		int samples = getNumSamples();
		//get square root
		float sq = float(sqrt(samples));
		float x = s /sq;
		float y = s / sq;
		
		float i = Random::U<float>();
		float j = Random::U<float>();

		float n = (x + i) /sq;
		float m = (y + j) /sq;

		return Vec2f(n, m);	
	}

};