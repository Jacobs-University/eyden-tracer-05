# Practical Assignment 5
**Name:** .......
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
### Supersampling (Points 10 + 10 + 10)
A pixel actually corresponds to a square area. Currently you are sampling the pixels only at their center, which lead to aliasing. As you have learned in the lecture, the most simple way for removing aliasing artifacts from your image is _supersampling_, _i.e._ to shoot more than one ray per pixels. The three most frequently used supersampling strategies are:  

**Regular Sampling:** The Pixel is subdivided into _n = m x m_ equally sized regions, which are sampled in the middle:  
_((i+0.5)/m, (j+0.5)/n)_ for _i,j=[0 .. m-1]_

**Random Sampling:** The Pixel is sampled by _n_ randomly placed samples _e<sub>i</sub> in [0; 1)_  
_(e<sub>i,1</sub>,e<sub>i,2</sub>)_ for _i=[0 .. n-1]_

**Stratified Sampling:** Stratified sampling is a combination of regular and random sampling. One sample is randomly placed in each of the _n = m x m_ regions with _e<sub>i,1</sub>,e<sub>i,2</sub> in [0, 1)_  
_((i+e<sub>i</sub>)/m, (j+e<sub>j</sub>)/m)_ for _i,j=[0 .. m-1]_

In this exercise your task is to implement these sampling strategies:
- In the framework you can find an abstract base class ```CSampleGenerator``` with one single virtual method ```void SampleGenerator::GetSamples(int n, float *u, float *v, float *weight)```, which is supposed to works as follows: _n_ is the number of samples to be generated for a pixel. One sample consists of two coordinates (_u_, _v_) that specify a position on a pixel. The _n_ samples generated are then to be returned in the _u_ and _v_ arrays, where (_u_, _v_) should be in the domain _[0 .. 1) X [0 .. 1)_. The weights for the individual samples should sum up to 1, here just use uniform weights with ```weight[i]=1.0f/n```.
- In your main loop, produce _n_ samples, and fire _n_ rays through the pixel at the respective sample position. The resulting color values must be weighted by ```weight[i]``` and summed up, which yields the final pixel result.
- Implement the ```getSamples()``` method in SampleGeneratorRegular.h, SampleGeneratorRandom.h, and SampleGeneratorStratified.h which are derived classes from ```CSampleGenerator```.
Use ground.obj and cb.bmp to render your image with 4 samples and compare them to the following images: (regular) (random) (stratified)

<img src="./doc/regular.jpg" alt="regular" width="280px"><img src="./doc/random.jpg" alt="random" width="280px"><img src="./doc/stratified.jpg" alt="stratified" width="280px">

