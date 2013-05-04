//
//  cMaterialShader.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#ifndef __carrizo_pathtracer__cMaterialShader__
#define __carrizo_pathtracer__cMaterialShader__

#include <iostream>

#include "CImg.h"

#include "vecmat.h"

using namespace vecmat;

class cPathtracer;
typedef struct camera_vectors_t camera_vectors;
typedef struct ray_t Ray;

class cMaterialShader : public cShader{
    
private:
    
    int width_, height_;
    int pass_number_;
    
    cPathtracer *parent_pt_;
    
    cimg_library::CImg<double> *diffuse_buffer_;
    cimg_library::CImg<double> *specular_buffer_;
    cimg_library::CImg<double> *emissive_buffer_;
    cimg_library::CImg<double> *reflective_buffer_;
    cimg_library::CImg<double> *transparent_buffer_;
    
    cimg_library::CImgDisplay *diffuse_display_;
    cimg_library::CImgDisplay *specular_display_;
    cimg_library::CImgDisplay *emissive_display_;
    cimg_library::CImgDisplay *reflective_display_;
    cimg_library::CImgDisplay *transparent_display_;
    
public:
    void preparePass();
    void endPass();
    void prepareShader();
    void shadePixel(Ray &ray, int i, int j);
    
    ~cMaterialShader();
    cMaterialShader(cPathtracer &parent_pt);
};

#endif /* defined(__carrizo_pathtracer__cMaterialShader__) */
