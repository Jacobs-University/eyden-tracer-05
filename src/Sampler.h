// Sampler interface class
// Written by Sergey Kosov in 2019 for Project X
#pragma once

#include "types.h"
#include "random.h"
 
// ================================ Sampler Class ================================
/**
* @brief Sampler abstract class
* @warning This class is not thread-safe
*/
class CSampler {
public:
	/**
	* @brief Constructor
	* @param nSamples Square root of number of samples in one series
	*/
	CSampler(size_t nSamples) : m_nSamples(nSamples) {}
	CSampler(const CSampler&) = delete;
	virtual ~CSampler(void) = default;
	const CSampler& operator=(const CSampler&) = delete;
		
	/**
	* @brief Returns the sample \b s from a series
	* @details This function returns a pair of uniformly distributed random variables \f$(\xi_1, \xi_2)\f$ in square \f$[0; 1)^2\f$. 
	* Thus, it returs samples uniformly covering a unit square.
	* @param s The sequence number of the required sample
	* @return The s-th sample from a series
	*/
	virtual Vec2f	getSample(size_t s) const { return Vec2f::all(0.5f); }
	/**
	* @brief Returns the number of samples in a series 
	* @return The number of samples in a series 
	*/
	size_t	getNumSamples(void) const { return m_nSamples * m_nSamples; }
		
		
private:
	const size_t	m_nSamples;
	int  m_idx = 0; 
};
using ptr_sampler_t = std::shared_ptr<CSampler>;

