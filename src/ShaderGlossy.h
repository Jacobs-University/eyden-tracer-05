#pragma once

#include "ShaderPhong.h"
#include "Sampler.h"


const int nAreaSamples = 1;

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

		Vec3f normal = ray.hit->getNormal(ray);	
		
		Ray reflected;
		reflected.org = ray.org + ray.dir * ray.t;
		reflected.dir = normalize(ray.dir - 2 * normal.dot(ray.dir) * normal);

		Vec3f reflection = m_scene.RayTrace(reflected);

		res = 0.5 * res + 0.5 * reflection;// shading normal

		// --- PUT YOUR CODE HERE ---
		for (auto pLight : m_scene.getLights()) {
			for (int s = 0; s < nAreaSamples; s++) {
				std::optional<Vec3f> lightIntensity = pLight->illuminate(shadow);
				if (lightIntensity) {
					float cosLightNormal = shadow.dir.dot(normal);
					if (cosLightNormal > 0) {
						if (m_scene.occluded(shadow))
							continue;

						Vec3f diffuseColor = m_kd * color;
						res += (diffuseColor * cosLightNormal).mul(lightIntensity.value());
					}

					// specular term
					float cosLightReflect = shadow.dir.dot(reflect);
					if (cosLightReflect > 0) {
						Vec3f specularColor = m_ks * RGB(1, 1, 1); // white highlight;
						res += (specularColor * powf(cosLightReflect, m_ke)).mul(lightIntensity.value());
					}
				}
		}
		

		return res;
			
			if (nAreaSamples > 1)
			res /= nAreaSamples;
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};
