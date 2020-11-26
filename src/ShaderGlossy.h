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
		//disc transformation 
		Vec3f dev_normal= Vec3f(0, 0, 0);
		Vec2f disc;
		Vec3f sample;
		Vec2f s;
		float theta, r;
		int S = m_pSampler->getNumSamples();
		for (int i = 0; i < S; i++) {

			sample = m_pSampler->getSample(i);

			s = 2 * sample - Vec2f::all(1);

			if (s[0] == 0 && s[1] == 0)
				disc = Vec2f::all(0);

			if (fabs(s[1]) > fabs(s[0])) {

				r = s[1];
				theta = 0.5f * Pif - 0.25f * Pif * s[0] / r;


			}
			else {

				r = s[0];
				theta = 0.25f * Pif * s[1] / r;

			}

			float x = r * cosf(theta);
			float y = r * sinf(theta);
			x += (-1 * x * m_glossiness);
			y += (-1 * y * m_glossiness);
			disc = Vec2f(x, y);
			Vec2f samples_ = disc;
			float z = sqrtf(max(0.0f, 1.0f - samples_[0] * samples_[0] - samples_[1] * samples_[1]));
			// the deviated normal 

			dev_normal = Vec3f(normal.val[0] + samples_[0], z, normal.val[2] + samples_[1]);

			Ray reflected;
			reflected.org = ray.org + ray.dir * ray.t;
			reflected.dir = normalize(ray.dir - 2 * dev_normal.dot(ray.dir) * dev_normal);

			Vec3f reflection = m_scene.RayTrace(reflected);

			res += 0.5 * res + 0.5 * reflection;
		}

		return res/S;
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};