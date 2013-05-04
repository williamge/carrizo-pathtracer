//
//  cDepthShader.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//


#include "CImg.h"

using namespace cimg_library;

#include "cShader.h"
#include "cDepthShader.h"

#include "cPathtracer.h"

/* The depth shader for a ray in the scene, shading each ray with the depth value on the surface of
 the object, taken from the t-value of the intersection.
 */


void cDepthShader::preparePass()
{
    pass_number_++;
}

void cDepthShader::endPass()
{
    display_->display(*buffer_);
}

void cDepthShader::prepareShader()
{
    std::pair<int,int> parent_dimensions = parent_pt_->getDimensions();
    width_ = parent_dimensions.first;
    height_ = parent_dimensions.second;
    
    delete buffer_; buffer_ = nullptr;
    buffer_ = new CImg<double> (width_, height_, 1, 3);
    
    display_ = new CImgDisplay(*buffer_, "Depth Shader");
}

/* Shades the given pixel at (i,j)
 */
void cDepthShader::shadePixel(Ray &ray, int i, int j)
{
    col3 depth_col (0.0, 0.0, 0.0);
    if (ray.intersection.hit)
	{
		depth_col = col3 (ray.intersection.t_value);
	}
    
    setImage(buffer_, i, j, depth_col, pass_number_);
}


cDepthShader::cDepthShader(cPathtracer &parent_pt): pass_number_(0)
{
    parent_pt_ = &parent_pt;
}