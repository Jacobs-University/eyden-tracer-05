#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"
#include "Solid.h"
#include "SolidQuad.h"
#include "SolidCone.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"
#include "ShaderGlossy.h"

#include "Texture.h"

#include "Sampler.h"
#include "SamplerRandom.h"
#include "SamplerRegular.h"
#include "SamplerStratified.h"

#include "LightOmni.h"
#include "LightArea.h"
#include "timer.h"

Mat RenderFrame(void)
{
	// Camera resolution
	const Size resolution(800, 600);

	// Background color
	const Vec3f bgColor = RGB(1, 1, 1);

	// Define a scene
	CScene scene(bgColor);

	// Camera
	scene.add(std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 2, -30.0f), normalize(Vec3f(0.8f, -0.5f, 1)), Vec3f(0, 1, 0), 45.0f));
	//I made some changes to match my file path but the original line can be uncommented for testing
#ifdef WIN32
	const std::string dataPath = "C:/Users/katri/Documents/GitHub/eyden-tracer-05/data/";
#else
	const std::string dataPath = "../../data/";
#endif

	// Textures
	Mat cb = imread(dataPath + "cb.bmp");
	if (cb.empty()) printf("ERROR: Texture file is not found!\n");
	auto pTextureChessBoard = std::make_shared<CTexture>(cb);

	// Shaders
	auto pShaderFloor = std::make_shared<CShaderFlat>(pTextureChessBoard);

	// Geometry
	const float s = 100;	// size of the chess board
	const float t = 100;	// texture size of the chess board
	
	// --- Problem 2 ---
	scene.add(std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 30.0f, 30.0f), normalize(Vec3f(0, -0.9f, -1)), Vec3f(0, 1, 0), 30.0f));
	//
	auto pAreaLightSampler = std::make_shared<CSamplerRandom>(3);
	auto pAreaLight = std::make_shared<CLightArea>(Vec3f::all(400), Vec3f(15, 32, 1), Vec3f(-15, 32, 1), Vec3f(-15, 32, -1), Vec3f(15, 32, -1), pAreaLightSampler);
	scene.add(pAreaLight);

	pShaderFloor = std::make_shared<CShaderPhong>(scene, Vec3f::all(1), 0.1f, 0.9f, 0.0f, 40.0f);
	auto pShaderDragon = std::make_shared<CShaderPhong>(scene, RGB(0.153f, 0.682f, 0.376f), 0.2f, 0.8f, 0.5f, 40.0f);
	CSolid dragon(pShaderDragon, dataPath + "Stanford Dragon.obj");
	scene.add(dragon);
	// --- ------- - ---

	// --- Problem 3 ---
	auto pGlossinessSampler = std::make_shared<CSamplerRandom>(3);
	pShaderFloor = std::make_shared<CShaderGlossy>(scene, Vec3f::all(1), 0.1f, 0.9f, 0.0f, 40.0f, 0.5f, pGlossinessSampler);
	// --- ------- - ---

	CSolidQuad floor(pShaderFloor, Vec3f(-s, 0, -s), Vec3f(-s, 0, s), Vec3f(s, 0, s), Vec3f(s, 0, -s), Vec2f(0, 0), Vec2f(t, 0), Vec2f(t, t), Vec2f(0, t));

	// Add everything to the scene
	scene.add(floor);

	// Build BSPTree
	scene.buildAccelStructure(30, 3);

	// Sampler
	/*
	For the different renders, change "CSampler" to
	"CSamplerRegular", "CSamplerRandom", or "CSamplerStratified"
	*/
	auto pSampler = std::make_shared<CSampler>(2);

	Mat img(resolution, CV_32FC3);							// image array
	Ray ray;												// primary ray
	size_t nSamples = pSampler->getNumSamples();

	img.setTo(0);
	for (int y = 0; y < img.rows; y++) {
		Vec3f* pImg = img.ptr<Vec3f>(y);					// fast processing via pointers
		for (int x = 0; x < img.cols; x++) {
			for (size_t s = 0; s < nSamples; s++) {
				Vec2f sample = pSampler->getSample(s);
				scene.getActiveCamera()->InitRay(ray, x, y, sample);	// initialize ray
				pImg[x] += scene.RayTrace(ray);
			} // s
			pImg[x] = (1.0f / nSamples) * pImg[x];
		} // x
	} // y

	img.convertTo(img, CV_8UC3, 255);
	return img;
}

int main(int argc, char* argv[])
{
	DirectGraphicalModels::Timer::start("Rendering...");
	Mat img = RenderFrame();
	DirectGraphicalModels::Timer::stop();
	imshow("Image", img);
	waitKey();
	imwrite("image.jpg", img);
	return 0;
}
