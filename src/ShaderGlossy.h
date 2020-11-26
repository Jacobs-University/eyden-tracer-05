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

	    // shading normal
        int nSamples = m_pSampler->getNumSamples();
        Vec3f agg = Vec3f::all(0);
        Vec3f res = CShaderPhong::shade(ray);
        Vec3f normal = ray.hit->getNormal(ray);
        int k = 0;
        for(int i = 0; i < nSamples; i++) {
            // deep copy of normal.
            auto normalCpy = Vec3f(normal);
            Vec2f sample = m_pSampler -> getSample(i);
            auto res2 = cwsReflection(sample, normalCpy, ray, nSamples);
            if (res2.has_value())
            {
                k++;
                agg += res2.value() * m_glossiness;
            }
        }
        res = res*0.5 + agg / k * 0.5;
        return res;
	}

protected:
    [[nodiscard]] std::optional<Vec3f> cwsReflection(const Vec2f& sample, const Vec3f& normal, const Ray& ray, int n) const
    {
        Vec2f disc;
        Vec2f s = 2 * sample - Vec2f::all(1);
        if (s[0] == 0 && s[1] == 0)
            disc = Vec2f::all(0);
        float theta, r;
        if (fabs(s[0]) > fabs(s[1])) {
            r = s[0];
            theta = 0.25f * Pif * s[1] / r;
        } else {
            r = s[1];
            theta = 0.5f * Pif - 0.25f * Pif * s[0] / r;
        }
        auto rA = normalize((Vec3f(0, 0, 1) + normal) / 2);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        s = Vec2f(x, y);
        float z = sqrtf(max(0.0f, 1.0f - s[0] * s[0] - s[1] * s[1]));
        auto nS = Vec3f(s[0], s[1], z);
        Ray reflected;
        reflected.org = ray.org + ray.dir * ray.t;
        reflected.dir = normalize(ray.dir - 2 * normal.dot(ray.dir) * normal);
        if (reflected.dir.dot(normal) < Epsilon)
            return std::nullopt;
        return m_scene.RayTrace(reflected);
    };

private:
	float			m_glossiness;		///< 1 - perfect mirror, 0.01 - fully diffuse
	ptr_sampler_t	m_pSampler;			///< Pointer to the sampler ref @ref CSampler
};