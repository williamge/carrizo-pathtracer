//
//  cNaivePTShader.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-05-04.
//
//

#include "CImg.h"

using namespace cimg_library;

#include "cShader.h"
#include "cNaivePTShader.h"

#include "cPathtracer.h"
#include "ray.h"

/* The naive pathtracing shader for a ray in the scene, takes in ray, sets the colour for that ray 
 using a naive implementation of a pathtracing function using the Lambertian diffuse model and Phong 
 specular reflectance.
 */


void cNaivePTShader::preparePass()
{
    pass_number_++;
}

void cNaivePTShader::endPass()
{
    display_->display(*buffer_);
}

void cNaivePTShader::prepareShader()
{
    std::pair<int,int> parent_dimensions = parent_pt_->getDimensions();
    width_ = parent_dimensions.first;
    height_ = parent_dimensions.second;
    
    delete buffer_; buffer_ = nullptr;
    buffer_ = new CImg<double> (width_, height_, 1, 3);
    
    delete display_; display_ = nullptr;
    display_ = new CImgDisplay(*buffer_, "Naive Pathtracing Shader");
}

/* Shades the given pixel at (i,j)
 */
void cNaivePTShader::shadePixel(Ray &ray, int i, int j)
{
    col3 regular_col = col3(0.0,0.0,0.0); //the final colour to be set for the given primary ray
    
    if (ray.intersection.hit)
    {
        //For each primary ray we want to take a whole number of samples because the shading to
        //each primary ray makes more of an impact than the primary ray, which only really affects
        //anti-aliasing.
        //
        //So we can ignore collecting more primary rays in exchange for taking more secondary rays.

        regular_col += regularShaderIterate(ray, i, j);
    }
    else
    {
        regular_col += parent_pt_->readEnvironmentMap(ray);
    }
    
    setImage(buffer_, i, j, regular_col, pass_number_);
}

cNaivePTShader::~cNaivePTShader()
{
    delete buffer_;
    delete display_;
}

cNaivePTShader::cNaivePTShader(cPathtracer &parent_pt):  parent_pt_(&parent_pt), pass_number_(0), buffer_(nullptr), display_(nullptr)
{
}

/* The same as regularShaderRecurse() but using iteration instead of recursion, kind of
 an experimental function.
 */
col3 cNaivePTShader::regularShaderIterate(Ray &ray, int i, int j)
{
    typedef struct ray_info_t{
        Ray ray;
        double probability;
    } ray_info;
    
    col3 regular_col (0.0);
    
    for (int samples = 0; samples < PRIMARY_SAMPLES; samples++)
    {
        
        std::vector<ray_info> path;
        
        Ray outward_ray = ray;
        
        ray_info outward_ray_info;
        outward_ray_info.ray = outward_ray;
        outward_ray_info.probability = std::max(0.1, (outward_ray.intersection.ray_material->emissive.r
                                                      + outward_ray.intersection.ray_material->emissive.g
                                                      + outward_ray.intersection.ray_material->emissive.b) / 3.0);
        path.push_back(outward_ray_info);
        
        col3 ray_col(0.0, 0.0, 0.0);
        
        double probability_weight = 0.9;
        
        for (;;) //loop to build the path for the given incident ray "ray"
        {
            Ray inward_ray = outward_ray;
            
            outward_ray.o = inward_ray.intersection.point + (inward_ray.intersection.normal * 0.005); //small number, TODO: make this better
            outward_ray.d = inward_ray.intersection.normal;
            outward_ray.intersection.hit = false;
            
            //TODO: make this way way better instead of the bizarre sampling that it is now
            probability_weight = probability_weight * inward_ray.intersection.ray_material->diffuse.average();
            
            double termination_sample = dis_zero_to_one(gen);
            
            if (termination_sample > probability_weight)
            {
                break;
            }
            else
            {
                outward_ray.d = sampleHemisphere(outward_ray.d, dis_zero_to_one(gen), dis_zero_to_one(gen));
                
                parent_pt_->intersectScene(outward_ray);
                
                ray_info outward_ray_info;
                outward_ray_info.ray = outward_ray;
                outward_ray_info.probability = probability_weight;
                path.push_back(outward_ray_info);
                
                if (!outward_ray.intersection.hit)
                {
                    break;
                }
            }
            
        }
        
        //we've constructed the path, now we're solving the rendering equation from the last ray in the
        //path backwards.
        
        col3 past_col (0.0, 0.0, 0.0);
        if (path.back().ray.intersection.hit)
        {
            past_col = (1.0/path.back().probability) * path.back().ray.intersection.ray_material->emissive;
        }
        else
        {
            past_col = (1.0/ (1.0 - path.back().probability)) * parent_pt_->readEnvironmentMap(path.back().ray);
        }
        
        for (long i = (path.size() - 1) - 1; i >= 0; i--)
        {
            col3 brdf_col (0.0, 0.0, 0.0);
            
            double lambert_factor = path[i+1].ray.d * path[i].ray.intersection.normal;
            
            //diffuse (lambert)
            brdf_col += path[i].ray.intersection.ray_material->diffuse;
            
            if (path[i].ray.intersection.ray_material->shininess > 0.0)
            {
                //phong specular reflection
                vec3 specular_reflect_vec = (2.0 * (path[i+1].ray.d * path[i].ray.intersection.normal) * path[i].ray.intersection.normal) - path[i+1].ray.d;
                double specular_factor = specular_reflect_vec * path[i].ray.d;
                specular_factor = pow(specular_factor, path[i].ray.intersection.ray_material->shininess);
                
                //specular (phong)
                brdf_col += specular_factor
                *
                (path[i].ray.intersection.ray_material->specular
                 * path[i].ray.intersection.ray_material->shininess_strength);
            }        
            past_col = path[i].ray.intersection.ray_material->emissive + (1.0/ (1.0 - path[i].probability)) * brdf_col.apply_r(past_col) * lambert_factor;
        }
        
        regular_col += past_col;
    }
    
    return regular_col * (1.0/PRIMARY_SAMPLES);
}

/* Returns a random vector in the hemisphere bounded by the vector "direction".
 
 The method by which it does this is by generating a random "z" term for the new vector and an angle,
 then completing the equation "1 = x^2 + y^2 + z^2" for the x and y terms, then checking if this new
 vector and the supplied vector are in the same direction, reversing the new vector if necessary.
 */
vec3 cNaivePTShader::sampleHemisphere(vec3 direction, double random_double1, double random_double2)
{    
    double r = 2.0 * vecmat_constant::pi * random_double1;
    double theta = vecmat_constant::pi * random_double2;
    
    double z_term = std::sin(theta);
    
    vec3 random_ray_direction (
                               std::sin(r) * z_term,
                               std::cos(r) * z_term,
                               std::cos(theta));
    
    if (random_ray_direction * direction < 0.0)
    {
        random_ray_direction = -random_ray_direction;
    }
    
    return random_ray_direction;
}

