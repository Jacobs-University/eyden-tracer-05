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

	virtual Vec2f	getSample(size_t s) const {
		// --- PUT YOUR CODE HERE ---
		// n - number of samples
		int n = getNumSamples();

		// generating random numbers
		// from 0 to 1
		float ei = Random::U<float>();
        float ej = Random::U<float>();

        float i = ((s % n) + ei) / float(sqrt(n));
        float j = ((s / sqrt(n)) + ej) / float(sqrt(n));
        return Vec2f(j, i);
		// return Vec2f::all(0.5f);
	}

};