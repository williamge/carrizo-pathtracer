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

using namespace vecmat;

class cPathtracer;

typedef struct camera_vectors_t camera_vectors;
typedef struct ray_t Ray;

class cShader{
    public:
    static void setImage(cimg_library::CImg<double> *image, int i, int j, col3& colour, int pass_number)
    {
        /*
         set the image at (i,j) to
         (pass_num/pass_num+1) //to expand out the current image to it's pre-divided amount
         * image colour at (i,j) //to set the current image colour to it's pre-divide amount
         + new pixel colour * 1/(pass_num); //to average in the new pixel colour
         */
        
        double pass_number_plus_one = ((double)pass_number+1.0);
        double pre_divide_amount = ((double)pass_number/pass_number_plus_one);
        
        (*image)(i,j,0,0) =
        pre_divide_amount
        * (*image)(i,j,0,0)
        +  colour.r / pass_number_plus_one;
        
        (*image)(i,j,0,1) =
        pre_divide_amount
        * (*image)(i,j,0,1)
        +  colour.g / pass_number_plus_one;
        
        (*image)(i,j,0,2) =
        pre_divide_amount
        * (*image)(i,j,0,2)
        +  colour.b / pass_number_plus_one;
    }
    
        virtual void preparePass() = 0;
        virtual void endPass() = 0;
        virtual void prepareShader() = 0;
        virtual void shadePixel(Ray &ray, int i, int j) = 0;
};

#endif /* defined(__carrizo_pathtracer__cShader__) */