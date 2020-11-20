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

		// --- PUT YOUR CODE HERE ---

		Ray reflected;
		reflected.org = ray.org + ray.dir * ray.t;
		reflected.dir = normalize(ray.dir - 2 * normal.dot(ray.dir) * normal);

		Vec3f reflection = m_scene.RayTrace(reflected);

		res = 0.5 * res + 0.5 * reflection;

		return res;
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};