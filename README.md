# Practical Assignment 5
**Dealine**: 11.11.2021

Please put your name here:  
**Name:** SHOROUK G. Awwad
## Problem 1
### Supersampling (Points 10 + 10 + 10)
In this assignment we will concentrate on stochastic ray-tracin and start with developing samplers, which generate random samples covering a unit square area.

A pixel actually corresponds to a square area. Currently you are sampling the pixels only at their center, which lead to aliasing. As you have learned in the lecture, the most simple way for removing aliasing artifacts from your image is _supersampling_, _i.e._ to shoot more than one ray per pixels. The three most frequently used supersampling strategies are:  

**Regular Sampling:** The Pixel is subdivided into _n = m x m_ equally sized regions, which are sampled in the middle:  
_((i+0.5)/m, (j+0.5)/n)_ for _i,j=[0 .. m-1]_

**Random Sampling:** The Pixel is sampled by _n_ randomly placed samples _e<sub>i</sub> in [0; 1)_  
_(e<sub>i,1</sub>,e<sub>i,2</sub>)_ for _i=[0 .. n-1]_

**Stratified Sampling:** Stratified sampling is a combination of regular and random sampling. One sample is randomly placed in each of the _n = m x m_ regions with _e<sub>i,1</sub>,e<sub>i,2</sub> in [0, 1)_  
_((i+e<sub>i</sub>)/m, (j+e<sub>j</sub>)/m)_ for _i,j=[0 .. m-1]_

In this exercise your task is to implement these sampling strategies:
1. Fork the current repository
2. Modify the README.md file in your fork and put your name above.
3. In the framework you can find an base class ```CSampler``` with the virtual method ```Vec2f getSample(size_t s) const;```, which is supposed to works as follows: _s_ is the sequential number of required sample to be generated. One sample consists of two coordinates (_u_, _v_) that specify a position within a unit square _[0 .. 1) X [0 .. 1)_. Pay extra attention on the difference between member-variable ```size_t m_nSamples``` and returning value of the method ```size_t CSampler::getNumSamples()```.
4. Check how sampler is incorporated in your main loop in **main.cpp** file. Method ```void ICamera::InitRay(Ray& ray, int x, int y, const Vec2f& sample)``` was modified to take an additional parameter - two dimensional sample. The method ```Vec2f CSampler::getSample(size_t)``` returns ```Vec2f::all(0.5f)``` to provide the original functioanlity of the classical ray tracer.
5. Implement the ```getSample()``` methods in **SamplerRegular.h**, **SamplerRandom.h**, and **SamplerStratified.h** which are derived classes from ```CSampler```. You can use method ```Random::U<float>()``` from **random.h** file to generate a uniform random number in range [0; 1).
Use cb.bmp texture to render your image with 4 samples (2 x 2) and compare them to the following images: (regular) (random) (stratified)

<img src="./doc/regular.jpg" alt="regular" width="275px"> <img src="./doc/random.jpg" alt="random" width="275px"> <img src="./doc/stratified.jpg" alt="stratified" width="275px">

## Problem 2
### Area Light (Points 20)
As you have learned in the class, shadows can add important visual information to an image. Until now we have only considered point lights. Point lights create _hard shadows_ because a point light can not be partly occluded and is either blocked or not. To render more realistic shadows we need a more advanced light source. _Area Lights_ are able to produce _soft shadows_ which are more natural. In this exercise we implement a ```CLightArea``` (in **LightArea.h**) which is defined by four points in space:
1. Study the constructor ```CLightArea::CLightArea()```. It takes a pointer to the sampler object, developed in the previouse problem.
2. Implement method ```std::optional<Vec3f>	CLightArea::illuminate(Ray& ray)``` using (1) the random samples as the locations of the _virtual_ point light sources at the surface of the LightArea, (2) normal and (3) the area of the light source.
    **Hint 1:** You may incorporate a static variable to track the sequence number **_s_** of a sample.
    **Hint 2:** In order to place a virtual point light source you can use method ```void	CLightOmni::setOrigin(const Vec3f& org)```.
3. Modify the shader method ```Vec3f CShaderPhong::shade(const Ray& ray)``` to integrate the incoming light using the sample points as described in the lecture.
4. Uncomment the code for problem 2 in **main.cpp** and render an image with 4 shadow rays and 4 primary rays per pixel. Play with different samplers and submit your best results in the "renders" folder.

If everything is implemented correct your images should look like this:
<img src="./doc/area light.jpg" alt="Area Light" width="800px">

## Problem 3
### Stochastic Glossy Reflection (Points 50)
Now we will incorporate stochastic raytracing for simulation of glossy surfaces. In particular we will develop a new shader: ```CShaderGlossy```  wich will make use of a random samples generator. 

To implement the shader follow the steps:
1. Study a draft of the class ```CShaderGlossy``` in **ShaderGlossy.h**. It is derived from ```CShaderPhong``` class and its implementation is based on the ```CShaderMirror``` class from previous assignments. In the ```CShaderGlossy::shade(const Ray& ray)``` method you will find an implementation which mixes 50% of the Phong shader color and 50% of the reflected light. This implementation imitates a perfectly smooth surface and results in the first image below.
In contrast to the Phong shader, the glossy shader constructor takes two additional arguments: 
    * ```float glossiness``` which should control the level of glossiness: 1 - Perfect glossiness and 0 - Diffuse glossiness (see images below), and
    * ``` ptr_sampler_t pSampler```pointer to the sampler object.
2. Uncomment the code for problem 3 in **main.cpp** and render an image with 4 glossy rays, 4 shadow rays and 4 primary rays per pixel. You should achive an image, corresponding to the perfect reflection depicted below.
    > **Note:** Using 4 glossy rays, 4 shadow rays and 4 primary rays per pixel will result in approximately 64 rays per pixel, which may cause long rendering time (10 - 60 seconds). If the rendering time exceeds 60 seconds on your machine, you may replace the dragon solid with a sphere primitive (with the same shader) in order to speed up rendering.
3. Modify ```CShaderGlossy::shade(const Ray& ray)``` to use random samples and ```m_glossiness``` parameter. Your goal is to achieve "Normal glossiness" and "Diffuse glossiness" renders as depicted below. In order to do that proceed as follows:
    * Perform transformation of the random variables from sampler object which samples a unit sqhare to sample a unit hemisphere
    * Transform the samples from their coordinate system (_e.g_ in the lecture examples we had z axis looking to the top) to the object's (in our case it is floor object, where y axis looking to the top) coordinate system.
    * Deviate the normal to look at the random location at the hemisphere.
    * Trace a number of reflected rays, derived in respect to the deviated normal and then average the results. The number of reflection rays is equal to number of random samples.
    * In order to control the glossiness level, you need to perform a second transformation of the random variables: try to derive such transformation yourself, whch will depend on the ```m_glossiness``` parameter:
        * if the ```m_glossiness``` parameter is equal to zero - you need to achieve a uniformly sample hemisphere. Take into account that reflected ray may go undet the surfaces.
        * if the ```m_glossiness``` parameter is equal to one - you need to achieve samples, which more probably lie on top of the hemisphere - thus the normal will have a minor probability to deviate, and all reflected rays will match the perfectly reflected ray.
            > **Note:** Recal the difference between uniform sampling and cosine-weighted sampling from the lecture. Elaborate on the way to force the sample to conentrate closer to the top of the hemisphere.
    * If everything is implemented correctly, you should achieve the following results, depending on the ```m_glossiness``` parameter:
   
From left to right: Perfect glossiness (_glossiness = 1.0f_); Normal glossiness (_glossiness = 0.5f_); Diffuse glossiness (_glossiness = 0.0f_)

<img src="./doc/glossy perf.jpg" alt="mirrow" width="275px"> <img src="./doc/glossy mid.jpg" alt="glossy" width="275px"> <img src="./doc/glossy low.jpg" alt="diffuse" width="275px">

## Submission
Please submit the assignment by making a pull request.
**Important** : Please make sure that
- No _extra files_ are submitted (except those, which were mentioned in the assignment)
- The changes were made _only_ in those files where you were asked to write your code
- The Continiouse Integration system (appVeyor) can build the submitted code
- The rendered images are also submitted in the folder "renders" 
