// Area Light Source class
// Written by Sergey Kosov in 2019 for Jacobs University
#pragma once

#include "LightOmni.h"
#include "Sampler.h"

// ================================ Area Light Class ================================
/**
	* @brief Area light sourse class
	*/
class CLightArea : public CLightOmni
{
public:
	/**
		* @brief Constructor
		* @details The position and size of the area light source is given by the quadrangular luminous surface.
		* This surface is defoned by 4 points in space: \b p1, \b p2, \b p3 and \b p4
		* @param intensity The emission color and strength of the light source
		* @param p0 The first point defining the quadrangular shape of the light source
		* @param p1 The second point defining the quadrangular shape of the light source
		* @param p2 The third point defining the quadrangular shape of the light source
		* @param p3 The fourth point defining the quadrangular shape of the light source
		* @param pSampler Pointer to the sampler to be used with the area light
		* @param castShadow Flag indicatin whether the light source casts shadow
		*/
	CLightArea(Vec3f intensity, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f p3, ptr_sampler_t pSampler, bool castShadow = true)
		: CLightOmni(intensity, p0, castShadow)
		, m_org(p0)
		, m_edge1(p1 - p0)
		, m_edge2(p3 - p0)
		, m_pSampler(pSampler)
	{
		m_normal = m_edge1.cross(m_edge2);
		m_area = norm(m_normal);
		m_normal = normalize(m_normal);
	}

	virtual std::optional<Vec3f>	illuminate(Ray& ray) override
	{
		// --- PUT YOUR CODE HERE ---
		Vec2f sample(0, 0);
		m_idx = 0;
		if (m_idx < getNumSamples() - 1) {

			m_idx++;
			sample = m_pSampler->getSample(m_idx);
		}
		else {

			sample = m_pSampler->getSample(m_idx);
			m_idx = 0; 
		}

		Vec3f org = m_org + sample.val[0] + m_edge1 + sample.val[1] * m_edge2;

		setOrigin(org);
		auto res = CLightOmni::illuminate(ray);
		double cos_of_n = ray.dir.dot(m_normal) / ray.t;

		if (cos_of_n > 0){

			return m_area * cos_of_n * res.value();

		}
		else {

			return std::nullptr;
		}

	}
	virtual size_t					getNumSamples(void) const override { return m_pSampler->getNumSamples(); }

	/**
	* @brief Returns the normal of area light surface
	* @return The normal of area light surface
	*/
	Vec3f getNormal(const Vec3f& position) const { return m_normal; }


private:
	Vec3f			m_org;		///< The origin of the area light source
	Vec3f			m_edge1;	///< The vector defyning the first edge of the area
	Vec3f			m_edge2;	///< The vector defyning the second edge of the area
	double			m_area;		///< Area of the light source
	Vec3f			m_normal;	///< Normal to the light source surface
	ptr_sampler_t	m_pSampler;	///< Pointer to the sampler ref @ref CSampler
};
