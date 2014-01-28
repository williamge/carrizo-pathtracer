//
//  cShader.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#ifndef __carrizo_pathtracer__cShader__
#define __carrizo_pathtracer__cShader__

#include <iostream>

#include "vecmat.h"
#include "CImg.h"

using namespace vecmat;

class cPathtracer;

typedef struct camera_vectors_t camera_vectors;
typedef struct ray_t Ray;

class cShader{
    public:
        static void setImage(cimg_library::CImg<double> *image, int i, int j, col3& colour, int pass_number)
        {
            (*image)(i,j,0,0) =
            ((pass_number-1) * (*image)(i,j,0,0) +  colour.r )/ pass_number;
            
            (*image)(i,j,0,1) =
            ((pass_number-1) * (*image)(i,j,0,1) +  colour.g )/ pass_number;
            
             (*image)(i,j,0,2) =
             ((pass_number-1) * (*image)(i,j,0,2) +  colour.b )/ pass_number;
        }
    
        virtual ~cShader(){}
    
        virtual void preparePass() = 0;
        virtual void endPass() = 0;
        virtual void prepareShader() = 0;
        virtual void shadePixel(Ray &ray, int i, int j) = 0;
};

#endif /* defined(__carrizo_pathtracer__cShader__) */