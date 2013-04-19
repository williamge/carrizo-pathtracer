//
//  cPathtracer.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-16.
//
//

#include <chrono>

#include "cPathtracer.h"
#include "cObject.h"
#include "rendModel.h"


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

/* Adds the cObject pointed to by "obj" to the list of objects for the
 current scene
 */
void cPathtracer::addObject(cObject *obj)
{
    objects_.push_back(obj);
}

/*
 Simply intersects each render model in the scene with the current ray,
 to trace the ray.
 */
void cPathtracer::intersectScene(Ray * ray)
{
	
	for (int i = 0; i < render_models_.size(); i++)
	{
		render_models_[i].intersect(*ray);
	}
}

/*
 Shades the given pixel at (i,j).
 
 "direction_vector", "x_unit", "y_unit", "factor" are all needed but don't belong in the class spec so they get
 passed in with the function call.
 */
void cPathtracer::shadePixel(int i, int j, vec3 &direction_vector, vec3 &x_unit, vec3 &y_unit, float factor_)
{
    Ray rayt;
    rayt.o = camera_.origin;
    
    rayt.d = direction_vector;
    rayt.d += ((-double(image_.width)/2.0 + i)*factor_) * x_unit;
    rayt.d += (-(-double(image_.height)/2.0 + j)*factor_) * y_unit;
    
    rayt.intersection.hit = false;
    
    intersectScene(&rayt);
    col3 regular_col = regularShader(rayt);
    col3 normals_col = normalsShader(rayt);
    col3 depth_col = depthShader(rayt);
    
    (*image_.buffer)(i,j,0,0) = regular_col.r;
    (*image_.buffer)(i,j,0,1) = regular_col.g;
    (*image_.buffer)(i,j,0,2) = regular_col.b;
    
    (*image_.normals_buffer)(i,j,0,0) = normals_col.r;
    (*image_.normals_buffer)(i,j,0,1) = normals_col.g;
    (*image_.normals_buffer)(i,j,0,2) = normals_col.b;
    
    (*image_.depth_buffer)(i,j,0,0) = depth_col.r;
    (*image_.depth_buffer)(i,j,0,1) = depth_col.g;
    (*image_.depth_buffer)(i,j,0,2) = depth_col.b;
    
    if (i % 20  == 0 && j + 1 >= image_.height)
    {
        std::cout << "Done line " << i << std::endl;
        std::cout << std::flush;
    }
}

/* The regular default shader for a ray in the scene, takes in ray, returns the colour for that ray
 */
col3 cPathtracer::regularShader(Ray ray)
{
    if (ray.intersection.hit)
	{
		return (0.5 * ray.intersection.normal + 0.5);
	}
	return col3(0,0,0);
}

/* The normals shader for a ray in the scene, shading each ray with the normal vector of the surface it hits with the x value
 as red, y as blue, z as green. Takes in ray, returns the colour for that ray
 */
col3 cPathtracer::normalsShader(Ray ray)
{
    if (ray.intersection.hit)
	{
		return (0.5 * ray.intersection.normal + 0.5);
	}
	return col3(0,0,0);
}

/* The depth shader for a ray in the scene, shading each ray with the depth value on the surface of the object, taken from
 the t-value of the intersection.
 */
col3 cPathtracer::depthShader(Ray ray)
{
    if (ray.intersection.hit)
    {
        return col3 (ray.intersection.t_value,ray.intersection.t_value,ray.intersection.t_value);
    }
    return col3(0.0, 0.0, 0.0);
}

/*
 The almighty render function, takes in the width and height for the
 picture and renders the current scene, displaying it onscreen with
 CImg.
 */
void cPathtracer::render()
{
    //TODO: examine moving this to its own function
	for (int i = 0; i < objects_.size(); i++)
	{
		render_models_.push_back( objects_[i]->addToRender() );
	}
    
    //timing start
    auto start_time = std::chrono::high_resolution_clock::now();
    
    //p_direction starts with a point to point the camera at, then we make it the camera's direction vector
	vec3 p_direction = camera_.look_at;
    p_direction = p_direction - camera_.origin; //TODO: write a -= operator for vec3
    p_direction.normalize();
    
    std::cout << "p_direction: " << p_direction.x << " " << p_direction.y << " " << p_direction.z << std::endl;
    
    vec3 x_unit;
    vec3 y_unit;
    
    vec3 up_vector = vec3 (0.0,1.0,0.0);
    if (up_vector == p_direction)
    {
        up_vector = vec3(0.0, 0.0, -1.0);
    }
    
    x_unit = p_direction.vecCross(up_vector);
    y_unit = x_unit.vecCross(p_direction);
	
    //factor for "sensor" screen derived from fov, each ray for the render uses this number to set the fov
	float factor = 1.0/((double(image_.height)/2)/tan( camera_.fov * M_PI/360.0));
    
	cimg_forXY(*(image_.buffer), i,j)
	{
		shadePixel(i, j, p_direction, x_unit, y_unit, factor);
	}
    
    auto end_time = std::chrono::high_resolution_clock::now();
    
    std::cout << "Render time: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count() * 0.001 <<" seconds" << std::endl;
    
    //done rendering so far, just display and save the render
    CImgDisplay regular_display (*image_.buffer, "Regular Shader");
    CImgDisplay normals_display (*image_.normals_buffer, "Normals Shader");
    CImgDisplay depth_display (*image_.depth_buffer, "Depth Shader");
    
    image_.buffer->display(regular_display, true);
    image_.normals_buffer->display(normals_display, true);
    image_.depth_buffer->display(depth_display, true);
    
    image_.buffer->normalize(0, 255);
    image_.buffer->save_png("output.png");
}

/*
 Sets the dimensions for the render of the image, then creates the image space in memory.
 */
void cPathtracer::setDimensions(int width, int height)
{
    image_.width = width;
	image_.height = height;
    
    if (image_.buffer) {delete image_.buffer;}
    image_.buffer = new CImg<float> (image_.width, image_.height, 1, 3);
    
    if (image_.normals_buffer) {delete image_.normals_buffer;}
    image_.normals_buffer = new CImg<float> (image_.width, image_.height, 1, 3);
    
    if (image_.depth_buffer) {delete image_.depth_buffer;}
    image_.depth_buffer = new CImg<float> (image_.width, image_.height, 1, 3);
}

/*
 Sets the camera for the scene.
 */
void cPathtracer::setCamera( point3 origin, point3 look_at, float fov)
{
    camera_.origin = origin;
    camera_.look_at = look_at;
    camera_.fov = fov;
}

cPathtracer::cPathtracer()
{
    image_.buffer = NULL;
    image_.normals_buffer = NULL;
    image_.depth_buffer = NULL;
}
