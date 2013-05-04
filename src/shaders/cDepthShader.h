//
//  cDepthShader.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#ifndef __carrizo_pathtracer__cDepthShader__
#define __carrizo_pathtracer__cDepthShader__

#include <iostream>

#include "CImg.h"

#include "vecmat.h"

using namespace vecmat;

class cPathtracer;
typedef struct camera_vectors_t camera_vectors;
typedef struct ray_t Ray;

class cDepthShader : public cShader{
    
private:
    
    int width_, height_;
    int pass_number_;
    
    cPathtracer *parent_pt_;
    
    cimg_library::CImg<double> *buffer_;
    cimg_library::CImgDisplay *display_;
    
public:
    void preparePass();
    void endPass();
    void prepareShader();
    void shadePixel(Ray &ray, int i, int j);
    
    ~cDepthShader();
    cDepthShader(cPathtracer &parent_pt);
};

#endif /* defined(__carrizo_pathtracer__cDepthShader__) */
