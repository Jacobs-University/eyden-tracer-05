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
		//return Vec2f::all(0.5f);
		
		
		int m = static_cast<int>(sqrt(s));
		float delta = 1.0f / m;

		for (int j = 0; j < m; j++)
			for (int i = 0; i < m; i++) {
				float param_x = (i + Random::U<float>()) / m;
				float param_y = (j + Random::U<float>()) / m;
				return delta * Vec2f(param_x, param_y);
			}

	}

};
