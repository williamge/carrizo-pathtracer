//
//  cPathtracer.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-16.
//
//

#include <random>
#include <chrono>

#include "cPathtracer.h"
#include "cObject.h"
#include "rendModel.h"
#include "material.h"


using namespace cimg_library;

//objects to take random samples from later
//should probably think of a better way to do this
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0,1.0);

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

/*
 Sets the pixel (i,j) in the CImg image "image" to the colour from "colour", it does this by 
 averaging out the amount already in that pixel plus a factor times the new pixel colour.
 */
void cPathtracer::setImage(CImg<double> *image, int i, int j, col3& colour, unsigned int pass_number)
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

/*
 Shades the given pixel at (i,j), running through the different shaders enabled for the scene.
 
 TODO: implement selectable shaders
 */
void cPathtracer::shadePixel(int i, int j, camera_vectors &render_vectors, double factor)
{
    Ray rayt;
    rayt.o = camera_.origin;
    
    rayt.d = render_vectors.direction_vector;
    rayt.d += ((-double(image_.width)/2.0 + i)*factor) * render_vectors.x_unit;
    rayt.d += (-(-double(image_.height)/2.0 + j)*factor) * render_vectors.y_unit;
    
    //randomly jitters the pixel's ray across the screen to reduce aliasing
    rayt.d += (factor * (dis(gen) - 0.5) ) * render_vectors.x_unit;
    rayt.d += (factor * (dis(gen) - 0.5) ) * render_vectors.y_unit;
    
    rayt.intersection.hit = false;
    
    intersectScene(rayt);
    
    regularShader(rayt, i, j);
    normalsShader(rayt, i, j);
    depthShader(rayt, i, j);
    materialShader(rayt, i, j);    
}

/* The regular default shader for a ray in the scene, takes in ray, returns the colour for that ray
 */
void cPathtracer::regularShader(Ray ray, int i, int j)
{
    col3 regular_col;
    if (ray.intersection.hit)
	{
		regular_col = 0.5 * ray.intersection.normal + 0.5;
	}
    else
    {
        regular_col = col3(0,0,0);
    }
    
    setImage(image_.buffer, i, j, regular_col, pass_number_);  
}

/* The normals shader for a ray in the scene, shading each ray with the normal vector of the surface it hits with the x value
 as red, y as blue, z as green. Takes in ray, returns the colour for that ray
 */
void cPathtracer::normalsShader(Ray ray, int i, int j)
{
    col3 normals_col;
    if (ray.intersection.hit)
	{
		normals_col = 0.5 * ray.intersection.normal + 0.5;
	}
    else
    {
        normals_col = col3(0,0,0);
    }
    
    setImage(image_.normals_buffer, i, j, normals_col, pass_number_);
}

/* The depth shader for a ray in the scene, shading each ray with the depth value on the surface of the object, taken from
 the t-value of the intersection.
 */
void cPathtracer::depthShader(Ray ray, int i, int j)
{
    col3 depth_col;
    if (ray.intersection.hit)
    {
       depth_col = col3 (ray.intersection.t_value,ray.intersection.t_value,ray.intersection.t_value);
    }
    else
    {
        depth_col = col3(0.0, 0.0, 0.0);
    }
    
    setImage(image_.depth_buffer, i, j, depth_col, pass_number_);
}

void cPathtracer::materialShader(Ray ray, int i, int j)
{
    col3 diffuse_col;
    col3 specular_col;
    col3 emissive_col;
    col3 reflective_col;
    col3 transparent_col;
    
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
        diffuse_col = specular_col = emissive_col = reflective_col = transparent_col = col3(0.0, 0.0, 0.0);
    }
    
    setImage(image_.diffuse_buffer, i, j, diffuse_col, pass_number_);
    setImage(image_.specular_buffer, i, j, specular_col, pass_number_);
    setImage(image_.emissive_buffer, i, j, emissive_col, pass_number_);
    setImage(image_.reflective_buffer, i, j, reflective_col, pass_number_);
    setImage(image_.transparent_buffer, i, j, transparent_col, pass_number_);
}

/* Primes the scene to be rendered.
 
 TODO: give it a reason to return a bool
 */
bool cPathtracer::readyObjects()
{
    for (int i = 0; i < objects_.size(); i++)
	{
		render_models_.push_back( objects_[i]->addToRender() );
	}
    
    return false;
}

void cPathtracer::renderPass(camera_vectors &render_vectors)
{    
    auto start_time = std::chrono::high_resolution_clock::now();
	
    //factor for "sensor" screen derived from fov, each ray for the render uses this number to set the fov
	double factor = 1.0/(
                            ( double(image_.height)/2 ) / tan( camera_.fov * vecmat_constant::pi/360.0 )
                         );
    
	cimg_forXY(*(image_.buffer), i,j)
	{
		shadePixel(i, j, render_vectors, factor);
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
    //done rendering so far, just display and save the render
    image_.regular_display = new CImgDisplay(*image_.buffer, "Regular Shader");
    image_.normals_display = new CImgDisplay(*image_.normals_buffer, "Normals Shader");
    image_.depth_display = new CImgDisplay(*image_.depth_buffer, "Depth Shader");
    image_.diffuse_display = new CImgDisplay(*image_.diffuse_buffer, "Diffuse Shader");
    image_.specular_display = new CImgDisplay(*image_.specular_buffer, "Specular Shader");
    image_.emissive_display = new CImgDisplay(*image_.emissive_buffer, "Emissive Shader");
    image_.reflective_display = new CImgDisplay(*image_.reflective_buffer, "Reflectivity Shader");
    image_.transparent_display = new CImgDisplay(*image_.transparent_buffer, "Transparency Shader");
    
    if (!image_.buffer) //if dimensions weren't set, set them to a default value
    {
        setDimensions(640, 480);
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
    
    for (pass_number_ = 1; pass_number_ < MAX_PASS_NUMBER+1; pass_number_++)
    {
        renderPass(render_vectors);
        
        //why does CImg only let you use display_info this way?!??!?!?!??!?!?!?!
        //image_.buffer->display(regular_display, true);
        //image_.normals_buffer->display(normals_display, true);
        //image_.depth_buffer->display(depth_display, true);
        
        image_.regular_display->display(*image_.buffer);
        image_.normals_display->display(*image_.normals_buffer);
        image_.depth_display->display(*image_.depth_buffer);
        image_.diffuse_display->display(*image_.diffuse_buffer);
        image_.specular_display->display(*image_.specular_buffer);
        image_.emissive_display->display(*image_.emissive_buffer);
        image_.reflective_display->display(*image_.reflective_buffer);
        image_.transparent_display->display(*image_.transparent_buffer);
    }
    
    image_.buffer->normalize(0, 255);
    image_.buffer->save_png("output.png");
    std::cout << "Rendering done" <<std::endl;
}

/*
 Sets the dimensions for the render of the image, then creates the image space in memory.
 */
void cPathtracer::setDimensions(int width, int height)
{
    image_.width = width;
	image_.height = height;
    
    delete image_.buffer;
    image_.buffer = nullptr;
    image_.buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.normals_buffer;
    image_.normals_buffer = nullptr;
    image_.normals_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.depth_buffer;
    image_.depth_buffer = nullptr;
    image_.depth_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.diffuse_buffer;
    image_.diffuse_buffer = nullptr;
    image_.diffuse_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.specular_buffer;
    image_.specular_buffer = nullptr;
    image_.specular_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.emissive_buffer;
    image_.emissive_buffer = nullptr;
    image_.emissive_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.reflective_buffer;
    image_.reflective_buffer = nullptr;
    image_.reflective_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
    
    delete image_.transparent_buffer;
    image_.transparent_buffer = nullptr;
    image_.transparent_buffer = new CImg<double> (image_.width, image_.height, 1, 3);
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
    delete image_.buffer;
    delete image_.normals_buffer;
    delete image_.depth_buffer;
    delete image_.diffuse_buffer;
    delete image_.specular_buffer;
    delete image_.emissive_buffer;
    delete image_.reflective_buffer;
    delete image_.transparent_buffer;
    
    delete image_.regular_display;
    delete image_.normals_display;
    delete image_.depth_display;
    delete image_.diffuse_display;
    delete image_.specular_display;
    delete image_.emissive_display;
    delete image_.reflective_display;
    delete image_.transparent_display;
    
}

cPathtracer::cPathtracer()
{
    image_.buffer = nullptr;
    image_.normals_buffer = nullptr;
    image_.depth_buffer = nullptr;
    
    image_.regular_display = nullptr;
    image_.normals_display = nullptr;
    image_.depth_display = nullptr;
    
    image_.diffuse_buffer = nullptr;
    image_.specular_buffer = nullptr;
    image_.emissive_buffer = nullptr;
    image_.reflective_buffer = nullptr;
    image_.transparent_buffer = nullptr;
}
