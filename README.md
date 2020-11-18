# Practical Assignment 5
**Dealine**: 26.11.2020

Please put your name here:  
**Name:** .......
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
3. In the framework you can find an base class ```CSampler``` with the virtual method ```Vec2f getSample(size_t s) const;```, which is supposed to works as follows: _s_ is the sequential number of required sample to be generated. One sample consists of two coordinates (_u_, _v_) that specify a position within a unit square _[0 .. 1) X [0 .. 1)_. Pay extra attention on the difference between member-variable ```size_t m_nSamples``` and returning value of the method ```size_t CSampler::getNu,mSamples()```.
4. Check how sampler is incorporated in your main loop in **main.cpp** file. Method ```void ICamera::InitRay(Ray& ray, int x, int y, const Vec2f& sample)``` was modified to take an additional parameter - two dimensional sample. The method ```Vec2f CSampler::getSample(size_t)``` returns ```Vec2f::all(0.5f)``` to provide the original functioanlity of the classical ray tracer.
5. Implement the ```getSample()``` methods in **SamplerRegular.h**, **SamplerRandom.h**, and **SamplerStratified.h** which are derived classes from ```CSampler```.
Use cb.bmp texture to render your image with 4 samples (2 x 2) and compare them to the following images: (regular) (random) (stratified)

<img src="./doc/regular.jpg" alt="regular" width="280px"> <img src="./doc/random.jpg" alt="random" width="280px"> <img src="./doc/stratified.jpg" alt="stratified" width="280px">

## Problem 2
### Area Light
As you have learned in the class, shadows can add important visual information to an image. Until now we have only considered point lights. Point lights create _hard shadows_ because a point light can not be partly occluded and is either blocked or not. To render more realistic shadows we need amore advanced light source. _Area Lights_ are able to produce _soft shadows_ which are more natural. In this exercise we implement a ```CLightArea``` (in **LightArea.h**) which is defined by four points in space:
1. Study the constructor ```CLightArea::CLightArea()```. It takes a pointer to the sampler object, developed in the previouse problem.
2. Implement method ```std::optional<Vec3f>	CLightArea::illuminate(Ray& ray)``` using the random samples as the locations of the _verual_ point light sources at the surface of the LightArea, normal and the area of the LightArea.

**Hint:** You may incorporate a static variable to track the sequence number **_s_** of a sample.
3. Modify your shader method ```Vec3f CShaderPhong::shade(const Ray& ray)``` to integrate the incoming light using the sample points as as described in the lecture.
4. Uncomment the code for problem 2 in **main.cpp** and render an image with 64 shadow rays per pixel
If everything is implemented correct your images should look like this:


## Problem 3
### Stochastic Bump Mapping

## Problem 4
### Chrome Shader



## Problem 5.1 
### Reflection (Points 20)
In contrast to the rasterization algorithms, raytracing can simulate physically correct reflections and refractions. In this assignmnet we will extend the Phong shader to handle both reflections and refractions. 
Proceed as follows:
1. Fork the current repository.
2. Modify the README.md file in your fork and put your name (or names if you work in a group) above.
3. You will find the _torus knot_ model in your data folder.
4. Turn on BSP-support by enabling the flag `ENABLE_BSP` in types.h file or in cmake-gui.exe application.
5. Extend ```CShaderPhong::Shade()``` to also support reflections.
6. Test your implementation with torus knot.obj. 
If everything is correct your images should look like this:  
![](./doc/mirror.jpg) 

## Problem 5.2
### Refraction (Points 30)
Now we need to extend ```CShaderPhong::Shade()``` to also support refractions.
Test your implementation with torus knot.obj and refraction coefficient k = 1.5 (glass) or k = 2.4 (diamond)
If everything is correct your images should look like this:  
![](./doc/glass.jpg) 

## Problem 5.3
### Glass Shading (Points 50)
In order to create the naturally-looking glass matherial, we need to combine reflections and refractions from the previouse problems. Please use the following proportions: 
- Ambient coefficient: 0
- Diffuse reflection coefficient: 0.1
- Specular refelection coefficient: 2
- Shininess exponent: 80
- Perfect reflection (mirror) coefficient: 0.2
- Perfect transmission coefficient: 0.8
- The refractive index of the medium: 1.5

Test your implementation on torus knot.obj. If everything is correct your image should look like this:
![](./doc/torus.jpg) 

## Problem 5.4
### Procedual Bump Mapping (Points 20)
In the last exercise you have learned that the appearence of a surface can be changed by using a modified _surface normal_. In this exercise we will implement a technique called _bump mapping_ which bumps the surface normal sideways such that a surface has the impression of beeing bumped. This often allows to generate the appearance of highly complex surface with only very few primitives. In order to do this, three parameters have to be known for each surface point:
1. The original surface normal _N_.
2. A local coordinate frame for this surface point. Though any coordinate frame can be used (as long as it is consistent), the usual way is to use the surface derivates in _u_ and _v_ direction, called _dPdu_ and _dPdv_.
3. The amount _delta_u_, _delta_v_ of deviation along these tangent vectors. The deviation is usually either read from a texture, or is computed procedurally. The final normal during shading (also for reflections) is then _N' = Normalized(N + delta_u * dPdu + delta_v * dPdv)_.  

In this exercise, you will implement a very basic version of this:
- As surface derivatives, use _dPdu = (1, 0, 0)_ and dPdv = _(0, 0, 1)_.
- For the amount of deviation, use a simple procedural approach, by computing _delta_u = 0.5 * cos(3 * H_x * sin(H_z))_, _delta_v = 0.5 * sin(13 * H_z)_. _H_ denotes the hit point of the ray with the surface.  

For your implementation, proceed as follows:
- Implement the Shade-method in ShaderPhongBumpMapped.h by first copying the ```Shade()```-method from the basic phong shader and then modifying the normal at the beginning of the ```Shade()``` function, following the guidlines given above.
If your shader work correct you should get an image like this using the scene description in main.cpp:  
![bump](./doc/bump_mapping.jpg)

**Tip:** The concept for a local coordinate frame will also be necessary for many other shading concepts, like _e.g._ procedural shading. For the exam, you will probably need such a concept, as the above hardcoded version will not be powerful enough.  
As getting the correct surface derivates can be somewhat complicated, there is also a simpler (though not as powerful) way of getting an orthonormal surface coordinate frame from the surface normal, which is similar to our way of generating the orthonormal camera coordinate frame from the camera direction. For a detailed discussion about surface derivatives you can also read Matt Pharr’s book _Physically Based Rendering_.

## Submission
Please submit the assignment by making a pull request.
**Important** : Please make sure that
- No _extra files_ are submitted (except those, which were mentioned in the assignment)
- The changes were made _only_ in those files where you were asked to write your code
- The Continiouse Integration system (appVeyor) can build the submitted code
- The rendered images are also submitted in the folder "renders" 
