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
		return Vec2f(	(s%m_nSamples + 0.5)/m_nSamples,
						(s/m_nSamples + 0.5)/m_nSamples); 
	}
};
