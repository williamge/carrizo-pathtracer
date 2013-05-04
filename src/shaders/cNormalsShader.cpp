//
//  cNormalsShader.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#include "CImg.h"

using namespace cimg_library;

#include "cShader.h"
#include "cNormalsShader.h"

#include "cPathtracer.h"

/* The normals shader for a ray in the scene, shading each ray with the normal vector of the surface
 it hits with the x value as red, y as blue, z as green. Takes in ray, sets the colour for that ray
 */


void cNormalsShader::preparePass()
{
    pass_number_++;
}

void cNormalsShader::endPass()
{
     display_->display(*buffer_);
}

void cNormalsShader::prepareShader()
{
    std::pair<int,int> parent_dimensions = parent_pt_->getDimensions();
    width_ = parent_dimensions.first;
    height_ = parent_dimensions.second;
    
    delete buffer_; buffer_ = nullptr;
    buffer_ = new CImg<double> (width_, height_, 1, 3);
    
    display_ = new CImgDisplay(*buffer_, "Normals Shader");
}

/* Shades the given pixel at (i,j)
 */
void cNormalsShader::shadePixel(Ray &ray, int i, int j)
{
    col3 normals_col (0.0, 0.0, 0.0);
    if (ray.intersection.hit)
	{
		normals_col = 0.5 * ray.intersection.normal + 0.5;
	}
    
    setImage(buffer_, i, j, normals_col, pass_number_);
}


cNormalsShader::cNormalsShader(cPathtracer &parent_pt): pass_number_(0)
{
    parent_pt_ = &parent_pt;
}