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

		Vec3f normal = ray.hit->getNormal(ray);									// shading normal
		
		float theta = atan2(normal[2], normal[0]);
		float phi = acos(normal[1]);
		
		const size_t sampleCount = m_pSampler->getNumSamples();
		Vec3f reflection = Vec3f::all(0);
		for (size_t s = 0; s < sampleCount; s++) {
			Vec2f sample = m_pSampler->getSample(s);
			
			//map ranges to ranges of theta and phi for a hemisphere
			//range for phi changes based on glossiness
			sample[0] = MAX(acos(sample[0]) - 0.5*Pif*m_glossiness, 0); //phi
			sample[1] *= 2*Pif; //theta
			
			//orient hemisphere around old normal
			sample[1] += theta;
			sample[0] += phi;
			
			Vec3f sampleHem = normalize(Vec3f(cosf(sample[1])*sinf(sample[0]),
									cosf(sample[1]),
									sinf(sample[1])*sinf(sample[0])));
			
			Ray reflected;
			reflected.org = ray.org + ray.dir * ray.t;
			reflected.dir = normalize(ray.dir - 2 * sampleHem.dot(ray.dir) * sampleHem);
			
			reflection += m_scene.RayTrace(reflected);
		}
		reflection *= 1.0f / sampleCount;

		res = 0.5 * res + 0.5 * reflection;

		return res;
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};