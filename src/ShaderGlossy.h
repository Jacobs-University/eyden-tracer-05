#pragma once

#include "ShaderPhong.h"
#include "Sampler.h"

class CShaderGlossy : public CShaderPhong {
public:
	/**
	* @brief Constructor
	* @param scene Reference to the scene
	* @param color The color of the object
	* @param ka The ambient coefficient
	* @param kd The diffuse reflection coefficients
	* @param ks The specular refelection coefficients
	* @param ke The shininess exponent
	* @param glossiness The glossiness coefficient, where 1 - perfect mirror and 0.01 - folly diffuse
	* @param pSampler Pointer to the sampler object
	*/
	CShaderGlossy(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke, float glossiness, ptr_sampler_t pSampler)
		: CShaderPhong(scene, color, ka, kd, ks, ke)
		, m_glossiness(glossiness)
		, m_pSampler(pSampler)

	{}
	virtual ~CShaderGlossy(void) = default;

	virtual Vec3f shade(const Ray& ray) const override {
		Vec3f res = CShaderPhong::shade(ray);

		Vec3f Dnormal = ray.hit->getNormal(ray);									// shading normal

		// --- PUT YOUR CODE HERE ---
		Vec2f d; 
		int numSamples = m_pSampler->getNumSamples();

		for (int i = 0; i < numSamples; i++) 
		{
			Vec2f sample = m_pSampler->getSample(i);
			Vec2f sa= 2 * sample - Vec2f::all(0);
			
			float theta;
			float t;

			if (fabs(sa[1]) >= fabs(sa[0]))
			{
				t = sa[1];
				theta = 0.5f * Pif - 0.25f * Pif * sa[0] / t;
			}
			else
			{
				t = sa[0];
				theta = 0.25f * Pif * sa[1] / t;
			}
			//cordinates
			float x;
			float y;
			x= t*cosf(theta); 
			y= t*sinf(theta); 
			//get glossiness
			x= x+ (-1 * x * m_glossiness);
			y= y+ (-1 * y * m_glossiness);

			d = Vec2f(x, y);

			float z = sqrtf(max(0.0f, 1.0f - d[0] * d[0] - d[1] * d[1]));

			Dnormal = Vec3f(Dnormal.val[0] + d[0], z, Dnormal.val[2] + d[1]);

			Ray reflected;
			reflected.org = ray.org + ray.dir * ray.t;
			reflected.dir = normalize(ray.dir - 2 * Dnormal.dot(ray.dir) * Dnormal);

			Vec3f reflection = m_scene.RayTrace(reflected);

			res = 0.5 * res + 0.5 * reflection;
		}

		return (res / numSamples);
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};