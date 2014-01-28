//
//  cMaterialShader.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#include "CImg.h"

using namespace cimg_library;

#include "shaders/cShader.h"
#include "shaders/cMaterialShader.h"

#include "cPathtracer.h"
#include "ray.h"

/* The material shader, sets the pixels of each of the corresponding buffers for material colours
 to the values from the material of the object hit by the primary ray.
 */


void cMaterialShader::preparePass()
{
    pass_number_++;
}

void cMaterialShader::endPass()
{
    diffuse_display_->display(*diffuse_buffer_);
    specular_display_->display(*specular_buffer_);
    emissive_display_->display(*emissive_buffer_);
    reflective_display_->display(*reflective_buffer_);
    transparent_display_->display(*transparent_buffer_);
}

void cMaterialShader::prepareShader()
{
    std::pair<int,int> parent_dimensions = parent_pt_->getDimensions();
    width_ = parent_dimensions.first;
    height_ = parent_dimensions.second;
    
    delete diffuse_buffer_; diffuse_buffer_ = nullptr;
    delete specular_buffer_; specular_buffer_ = nullptr;
    delete emissive_buffer_; emissive_buffer_ = nullptr;
    delete reflective_buffer_; reflective_buffer_ = nullptr;
    delete transparent_buffer_; transparent_buffer_ = nullptr;
    
    diffuse_buffer_ = new CImg<double> (width_, height_, 1, 3);
    specular_buffer_ = new CImg<double> (width_, height_, 1, 3);
    emissive_buffer_ = new CImg<double> (width_, height_, 1, 3);
    reflective_buffer_ = new CImg<double> (width_, height_, 1, 3);
    transparent_buffer_ = new CImg<double> (width_, height_, 1, 3);
    
    delete diffuse_display_; diffuse_display_ = nullptr;
    delete specular_display_; specular_display_ = nullptr;
    delete emissive_display_; emissive_display_ = nullptr;
    delete reflective_display_; reflective_display_ = nullptr;
    delete transparent_display_; transparent_display_ = nullptr;
    
    diffuse_display_ = new CImgDisplay(*diffuse_buffer_, "Diffuse Shader");
    specular_display_ = new CImgDisplay(*specular_buffer_, "Specular Shader");
    emissive_display_ = new CImgDisplay(*emissive_buffer_, "Emissive Shader");
    reflective_display_ = new CImgDisplay(*reflective_buffer_, "Reflective Shader");
    transparent_display_ = new CImgDisplay(*transparent_buffer_, "Transparent Shader");
}

/* Shades the given pixel at (i,j)
 */
void cMaterialShader::shadePixel(Ray &ray, int i, int j)
{
    col3 diffuse_col;
    col3 specular_col;
    col3 emissive_col;
    col3 reflective_col;
    col3 transparent_col;
    
    diffuse_col = specular_col = emissive_col = reflective_col = transparent_col = col3(0.0, 0.0, 0.0);    
    
    if (ray.intersection.hit)
	{
		diffuse_col = ray.intersection.ray_material->diffuse;
        specular_col = ray.intersection.ray_material->specular;
        emissive_col = ray.intersection.ray_material->emissive;
        reflective_col = ray.intersection.ray_material->reflective;
        transparent_col = ray.intersection.ray_material->transparent;
	}
    else
    {
        emissive_col = parent_pt_->readEnvironmentMap(ray);
    }
    
    setImage(diffuse_buffer_, i, j, diffuse_col, pass_number_);
    setImage(specular_buffer_, i, j, specular_col, pass_number_);
    setImage(emissive_buffer_, i, j, emissive_col, pass_number_);
    setImage(reflective_buffer_, i, j, reflective_col, pass_number_);
    setImage(transparent_buffer_, i, j, transparent_col, pass_number_);
}

cMaterialShader::~cMaterialShader()
{
    delete diffuse_buffer_;
    delete specular_buffer_;
    delete emissive_buffer_;
    delete reflective_buffer_;
    delete transparent_buffer_;
    
    delete diffuse_display_;
    delete specular_display_;
    delete emissive_display_;
    delete reflective_display_;
    delete transparent_display_;
}

cMaterialShader::cMaterialShader(cPathtracer &parent_pt): parent_pt_(&parent_pt), pass_number_(0)
{
    diffuse_buffer_ = nullptr;
    specular_buffer_ = nullptr;
    emissive_buffer_ = nullptr;
    reflective_buffer_ = nullptr;
    transparent_buffer_ = nullptr;
    
    diffuse_display_ = nullptr;
    specular_display_ = nullptr;
    emissive_display_ = nullptr;
    reflective_display_ = nullptr;
    transparent_display_ = nullptr;
    
}