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
		//from the tutorial 
		return Vec2f::all(0.5f);

		int n = getNumSamples();
		float x = s / sqrt(n);
		float y = s / sqrt(n);

		float ei = Random::U<float>();
		float ej = Random::U<float>();

		float m = float(sqrt(n)); 

		float i = (x + ei) / m;
		float j = (y + ej) / m;

		return Vec2f(i, j);
	}

};