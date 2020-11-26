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
        int N = m_pSampler -> getNumSamples();
        Vec2f disc;
        Vec3f deviatedNormal(0, 0, 0);
        for(int i = 0; i < N; i++) {
            Vec2f sample = m_pSampler -> getSample(i);

            Vec2f s1 = 2 * sample - Vec2f :: all(0);

//            // handling the origin degeneracy
//            if(s1[0] == 0 && s1[1] == 0) {
//                disc = Vec2f :: all(0);
//            }

            // applying concentric mapping to point
            float theta, r;
            if(fabs(s1[0]) > fabs(s1[1])) {
                r = s1[0];
                theta = 0.25f * Pif * s1[1] / r;
            }
            else {
                r = s1[1];
                theta = 0.5f * Pif - 0.25 * Pif * s1[0] / r;
            }

            float x = r * cosf(theta);
            float y = r * sinf(theta);

            x += ((-1) * x * m_glossiness);
            y += ((-1) * y * m_glossiness);

            disc = Vec2f(x, y);
            Vec2f s2 = disc;

            float z = sqrtf(max(0.0f, 1.0f - s2[0] * s2[0] - s2[1] * s2[1]));

            deviatedNormal = Vec3f(normal.val[0] + s2[0], z, normal.val[2] + s2[1]);

            Ray reflected;
            reflected.org = ray.org + ray.dir * ray.t;
            reflected.dir = normalize(ray.dir - 2 * deviatedNormal.dot(ray.dir) * deviatedNormal);

            Vec3f reflection = m_scene.RayTrace(reflected);

            res += 0.5 * res + 0.5 * reflection;
        }
        res /= N;

		return res;
	}

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};