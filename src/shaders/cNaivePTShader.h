//
//  cNaivePTShader.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#ifndef __carrizo_pathtracer__cNaivePTShader__
#define __carrizo_pathtracer__cNaivePTShader__

#include <iostream>

#include "CImg.h"
#include "random.h"

#include "vecmat.h"

using namespace vecmat;

class cPathtracer;
typedef struct camera_vectors_t camera_vectors;
typedef struct ray_t Ray;

class cNaivePTShader : public cShader{
    
private:
    
    const static int PRIMARY_SAMPLES = 16;
    
    int width_, height_;
    int pass_number_;
    
    cPathtracer *parent_pt_;
    
    cimg_library::CImg<double> *buffer_;
    cimg_library::CImgDisplay *display_;
    
    vec3 sampleHemisphere(vec3 direction, double, double);
    
    col3 regularShaderIterate(Ray &ray, int i, int j);
    col3 regularShaderRecurse(Ray &ray, int i, int j);
    
public:
    void preparePass();
    void endPass();
    void prepareShader();
    void shadePixel(Ray &ray, int i, int j);
    
    ~cNaivePTShader();
    cNaivePTShader(cPathtracer &parent_pt);
};

#endif /* defined(__carrizo_pathtracer__cNaivePTShader__) */
