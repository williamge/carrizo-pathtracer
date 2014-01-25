//
//  cPathtracer.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-16.
//
//

#include <chrono>
#include <dispatch/dispatch.h>

#include <assert.h>

#include "random.h"
#include "cPathtracer.h"
#include "cObject.h"
#include "rendModel.h"
#include "material.h"
#include "ray.h"

#include "shaders/cShader.h"
#include "shaders/cDepthShader.h"
#include "shaders/cMaterialShader.h"
#include "shaders/cNormalsShader.h"
#include "shaders/cNaivePTShader.h"



using namespace cimg_library;

/*
 * Terminology:
 * cObject is the type of object used internally by the program for
 non-rendering purposes (i.e. placement, importing)
 
 * rendModel.cpp contains the methods for turning a cObject in to
 an object that can be used to render. These render objects are
 made up of more specialized structures to make the rendering process
 easier to develop.
 
 * Each render model has its own Bounding Volume Hierarchy to speed up ray
 intersection. Each render model should also be a part of a bigger BVH to speed up
 ray intersection even further (so rays are intersected with a render model bounding
 box first, then the bounding boxes inside the render model, for efficiency).
 
 TODO: add in a more top level BVH to handle this.
 
 */

/* Adds a pointer to the cObject "obj" to the list of objects for the
 current scene.
 */
void cPathtracer::addObject(cObject &obj)
{
    objects_.push_back(&obj);
}

/* Add a shader to the scene, this shader will be run each render pass.
 */
void cPathtracer::addShader(std::shared_ptr<cShader> &shader)
{
    shaders_.push_back(shader);
}

/* Add a shader to the scene, this shader will only run once instead of being run each render pass. 
 */
void cPathtracer::addRunOnceShader(std::shared_ptr<cShader> &shader)
{
    shaders_one_pass_.push_back(shader);
}

/*
 Simply intersects each render model in the scene with the current ray,
 to trace the ray.
 */
void cPathtracer::intersectScene(Ray &ray)
{
	
	for (int i = 0; i < render_models_.size(); i++)
	{
		render_models_[i]->intersect(ray);
	}
}

/* Returns a renderTriangle that is emissive (the only type of light in the scene) from the scene and 
 the probability that that light was to be chosen instead of any other.
 */
std::pair<renderTriangle, double> cPathtracer::getRandomLight()
{
    std::pair<renderTriangle, double> return_pair;
    
    //if there are no emissive triangles in the scene then we have to return a fake one
    if (emissive_triangles_.empty())
    {        
        return_pair.first = fake_light_;
        return_pair.second = 1.0;
        return return_pair;
    }
    
    std::uniform_int_distribution<unsigned long> emissive_range (0, emissive_triangles_.size()-1);

    return_pair.first = emissive_triangles_[emissive_range(gen)];
    return_pair.second = 1.0/emissive_triangles_.size();
    return return_pair;
}

/*
 Shades the given pixel at (i,j), running through the different shaders enabled for the scene.

 */
void cPathtracer::shadePixel(int i, int j, camera_vectors &render_vectors)
{
    Ray rayt;
    rayt.o = camera_.origin;
    
    rayt.d = render_vectors.direction_vector;
    rayt.d += ((-double(image_.width)/2.0 + i)*camera_.factor) * render_vectors.x_unit;
    rayt.d += (-(-double(image_.height)/2.0 + j)*camera_.factor) * render_vectors.y_unit;
    
    //randomly jitters the pixel's ray across the screen to reduce aliasing
    rayt.d += (camera_.factor * (dis_zero_to_one(gen) - 0.5) ) * render_vectors.x_unit;
    rayt.d += (camera_.factor * (dis_zero_to_one(gen) - 0.5) ) * render_vectors.y_unit;
    
    rayt.intersection.hit = false;
    
    intersectScene(rayt);
    
    for (auto &shader : shaders_)
    {
        shader->shadePixel(rayt, i, j);
    }

}

/* Returns the colour value to be read from the environment map with the Ray "ray".
 
 TODO: implement environment maps
 */
col3 cPathtracer::readEnvironmentMap(Ray &ray)
{
    return col3(0.0, 0.0, 0.0);
}

/* Primes the scene to be rendered.
 
 TODO: give it a reason to return a bool
 */
bool cPathtracer::readyObjects()
{
    //add render models to queue to be rendered
    for (auto &object : objects_)
	{
		render_models_.push_back( object->addToRender() );
	}
    
    for (auto &render_model : render_models_)
    {
        for (auto iter = render_model->emissive_begin(); iter != render_model->emissive_end(); iter++)
        {
            //TODO: change type from renderTriangle to something new, precompute triangle area here
            emissive_triangles_.push_back(*iter);
        }
    }
    
    return false;
}

/* Cleans up the scene after a rendering.
 
 TODO: give it a reason to return a bool
 */
bool cPathtracer::cleanObjects()
{
    render_models_.clear();    
    emissive_triangles_.clear();
    
    //TODO: maybe add this?
    //might not be necessary
    /*
     for (auto &shader : shaders_)
     { 
        shader->clean();
     }
     
     */
    
    return false;
}

void cPathtracer::renderPass(camera_vectors &render_vectors)
{    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (auto &shader : shaders_)
    {
        shader->preparePass();
    }
    
    auto pixels_dispatch_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH,0);
    auto pixels_dispatch_group = dispatch_group_create();
    
    for (int i = 0; i<image_.width; i++)
    {
        for (int j = 0; j <image_.height; j++)
        {
            dispatch_group_async(pixels_dispatch_group, pixels_dispatch_queue,
                                 ^{shadePixel(i, j, render_vectors);
                                 });
        }
    }
    
    if (dispatch_group_wait(pixels_dispatch_group, DISPATCH_TIME_FOREVER))
    {
        std::cout << "Rendering pass timed out" << std::endl;
    }
    
    dispatch_release(pixels_dispatch_group);
    
    for (auto &shader : shaders_)
    {
        shader->endPass();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    
    std::cout << "Pass " << pass_number_ << " render time: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count() * 0.001 <<" seconds" << std::endl;
}

/*
 The almighty render function, takes in the width and height for the
 picture and renders the current scene, displaying it onscreen with
 CImg.
 */
void cPathtracer::render()
{
    if (!image_.width && !image_.height) //if dimensions weren't set, set them to a default value
    {
        setDimensions(640, 480);
    }
    
    if (shaders_.empty() && shaders_one_pass_.empty())
    {
        shaders_.push_back(std::shared_ptr<cShader> (new cNormalsShader(*this)));
        shaders_.push_back(std::shared_ptr<cShader> (new cNaivePTShader(*this)));
    }
    
    readyObjects();
    
    //this is just a struct to hold the vectors we need to set up the camera screen
    camera_vectors render_vectors;
    
    //p_direction starts with a point to point the camera at, then we make it the camera's direction vector
	render_vectors.direction_vector = camera_.look_at;
    render_vectors.direction_vector -= camera_.origin;
    render_vectors.direction_vector.normalize();
    
    std::cout << "p_direction: "
        << render_vectors.direction_vector.x << " "
        << render_vectors.direction_vector.y << " "
        << render_vectors.direction_vector.z << std::endl;
     
    vec3 up_vector = vec3 (0.0,1.0,0.0);
    //if the render_vector crossed with the up_vector would be messy (i.e. they're the same) 
    //then we have to make a new appropriate up_vector
    if (up_vector == render_vectors.direction_vector)
    {
        up_vector = vec3(0.0, 0.0, -1.0);
    }
    else if (render_vectors.direction_vector == vec3(0.0, -1.0, 0.0))
    {
        up_vector = vec3(0.0, 0.0, -1.0);
    }
    
    render_vectors.x_unit = render_vectors.direction_vector.vecCross(up_vector);
    render_vectors.y_unit = render_vectors.x_unit.vecCross(render_vectors.direction_vector);
    
    render_vectors.x_unit.normalize();
    render_vectors.y_unit.normalize();
    
    //factor for "sensor" screen derived from fov, each ray for the render uses this number to set the fov
	camera_.factor = 1.0/(
                         ( double(image_.height)/2 ) / tan( camera_.fov * vecmat_constant::pi/360.0 )
                         );
    
    for (auto &shader : shaders_)
    {
        shader->prepareShader();
    }
    
    for (pass_number_ = 1; pass_number_ < MAX_PASS_NUMBER+1; pass_number_++)
    {
        renderPass(render_vectors);

       /*         
        const unsigned int key_seq[] = {cimg::keyS};
        if (image_.regular_display->is_key_sequence(key_seq, 1))
        {
            break;
        } */
        
        if (pass_number_ > 1 && shaders_.empty())
        {
            break;
        }
    }
    
   /* image_.buffer->normalize(0, 255);
    image_.buffer->save_png("output.png"); */
    std::cout << "Rendering done" <<std::endl;
    
    cleanObjects();
}

/*
 Sets the dimensions for the images to be rendered.
 */
void cPathtracer::setDimensions(int width, int height)
{
    image_.width = width;
	image_.height = height;
}

/* Returns the dimensions for the images to be rendered in an (x,y) pair.
 */ 
std::pair<int, int> cPathtracer::getDimensions()
{
    return std::pair<int,int>(image_.width, image_.height);
}

/*
 Sets the camera for the scene.
 */
void cPathtracer::setCamera( point3 origin, point3 look_at, double fov)
{
    camera_.origin = origin;
    camera_.look_at = look_at;
    camera_.fov = fov;
}

cPathtracer::~cPathtracer()
{
    
}

cPathtracer::cPathtracer()
{
    pass_number_ = 0;   
    image_.width = image_.height = 0;      
    setCamera(point3(0.0, 0.0 ,0.0), point3(0.0, 0.0, -1.0), 60);
    
    //in case we need it, like if there are no emissive materials in the scene but we need
    //to return one
    nonEmissiveMaterial_ = std::shared_ptr<material> (new material);
    nonEmissiveMaterial_->diffuse = nonEmissiveMaterial_->specular = nonEmissiveMaterial_->emissive = nonEmissiveMaterial_->reflective = nonEmissiveMaterial_->transparent = col3(0.0, 0.0, 0.0);
    
    //in case we need to return a light but the scene doesn't have any
    fake_light_.a = point3(0.0);
    fake_light_.u = vec3(0.0, 1.0, 0.0);
    fake_light_.v = vec3(0.0, 0.0, 1.0);
    fake_light_.normal = fake_light_.u.vecCross(fake_light_.v);
    fake_light_.triangle_material = nonEmissiveMaterial_;
}
