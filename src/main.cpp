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

#include "Texture.h"

#include "Sampler.h"
#include "SamplerRandom.h"
#include "SamplerRegular.h"
#include "SamplerStratified.h"

#include "LightOmni.h"
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
	auto pCamera = std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 2, -30.0f), normalize(Vec3f(0.8f, -0.5f, 1)), Vec3f(0, 1, 0), 45.0f);
	scene.add(pCamera);

#ifdef WIN32
	const std::string dataPath = "../data/";
#else
	const std::string dataPath = "../../data/";
#endif

	// Textures
	Mat cb = imread(dataPath + "cb.bmp");
	if (cb.empty()) printf("ERROR: Texture file is not found!\n");
	auto pTextureChessBoard = std::make_shared<CTexture>(cb);

	// Shaders
	auto pShaderChessBoard = std::make_shared<CShaderFlat>(pTextureChessBoard);

	// Geometry
	const float s = 100;	// size of the chess board
	const float t = 100;	// texture size of the chess board
	CSolidQuad chessBoard(pShaderChessBoard, Vec3f(-s, 0, -s), Vec3f(-s, 0, s), Vec3f(s, 0, s), Vec3f(s, 0, -s), Vec2f(0, 0), Vec2f(t, 0), Vec2f(t, t), Vec2f(0, t));

	// Add everything to the scene
	scene.add(chessBoard);

	// Build BSPTree
	scene.buildAccelStructure(20, 3);

	// Sampler
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
