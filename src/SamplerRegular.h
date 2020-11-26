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
		// --- PUT YOUR CODE HERE ---
		//return Vec2f::all(0.5f); 
		
		
		int m = static_cast<int>(sqrt(s));
		float delta = 1.0f / m;

		for (int j = 0; j < m; j++)
			for (int i = 0; i < m; i++) {
				float param_x = (i + 0.5f) / m;
				float param_y =  (j + 0.5f) / m;
				return delta * Vec2f(param_x, param_y);
			}
		
	}
};
