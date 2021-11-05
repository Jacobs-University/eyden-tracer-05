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
		/*We implemented this second using the formula
		given for problem 1.
		First, we take the number of samples as described in Sampler.h
		and use this to sample in the middle:
		*/
		int n = getNumSamples();
		float m = sqrt(n);
		float i = s % n;
		float j = s / m;
		float rand_i = Random::U<float>();
		float rand_j = Random::U<float>();
		float x = (i + rand_i) / m;
		float y = ((s / m) + rand_j) / m;
		return Vec2f(y, x);
		//return Vec2f::all(0.5f);
	}

};
