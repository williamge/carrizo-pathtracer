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
std::uniform_real_distribution<double> dis_zero_to_one(0.0,1.0);
std::uniform_real_distribution<double> dis_minus_one_to_one(-1.0,1.0);

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
    rayt.d += (factor * (dis_zero_to_one(gen) - 0.5) ) * render_vectors.x_unit;
    rayt.d += (factor * (dis_zero_to_one(gen) - 0.5) ) * render_vectors.y_unit;
    
    rayt.intersection.hit = false;
    
    intersectScene(rayt);
    
    regularShader(rayt, i, j);
    normalsShader(rayt, i, j);
    depthShader(rayt, i, j);
    materialShader(rayt, i, j);
}

/* Returns a random vector in the hemisphere bounded by the vector "direction".
 
 The method by which it does this is by generating a random "z" term for the new vector and an angle, 
 then completing the equation "1 = x^2 + y^2 + z^2" for the x and y terms, then checking if this new 
 vector and the supplied vector are in the same direction, reversing the new vector if necessary.
 */
vec3 cPathtracer::sampleHemisphere(vec3 direction)
{
    double z = dis_minus_one_to_one(gen);
    double theta = vecmat_constant::pi * dis_minus_one_to_one(gen);
    
    double z_term = sqrt(1.0 - pow(z,2.0));
    
    vec3 random_ray_direction (
                               std::sin(theta) * z_term,
                               std::cos(theta) * z_term,
                               z);
    
    if (random_ray_direction * direction < 0.0)
    {
        random_ray_direction = -random_ray_direction;
    }
    
    return random_ray_direction;
}

/* Returns the colour value to be read from the environment map with the Ray "ray".
 
 TODO: implement environment maps
 */
col3 cPathtracer::readEnvironmentMap(Ray &ray)
{
    return col3(0.0, 0.0, 0.0);
}

/* The recursive part of the regular shader, since shaders have to fit a certain signature.
 */
col3 cPathtracer::regularShaderRecurse(Ray &ray, int i, int j)
{
    col3 ray_col(0.0, 0.0, 0.0);
    
    Ray inward_ray = ray;
    Ray outward_ray;
    
    outward_ray.o = inward_ray.intersection.point + (inward_ray.intersection.normal * 0.00005); //small number, TODO: make this better
    outward_ray.d = inward_ray.intersection.normal;
    outward_ray.intersection.hit = false;
    
    //TODO: make this way way better instead of the bizarre sampling that it is now
    double probability_weight;
    probability_weight = inward_ray.intersection.ray_material->emissive.r
    + inward_ray.intersection.ray_material->emissive.g
    + inward_ray.intersection.ray_material->emissive.b;
    probability_weight *= (1.0/3.0);
    probability_weight = std::max(probability_weight, 0.1);
    
    double termination_sample = dis_zero_to_one(gen);
    
    if (termination_sample >= probability_weight)
    {
        outward_ray.d = sampleHemisphere(outward_ray.d);
        
        intersectScene(outward_ray);
        
        if (outward_ray.intersection.hit)
        {
             //diffuse component of lighting
            double lambert_factor = outward_ray.d * inward_ray.intersection.normal;
            
            ray_col += lambert_factor * (1.0/ (1.0 - probability_weight))
                * inward_ray.intersection.ray_material->diffuse.apply_r (
                          regularShaderRecurse(outward_ray, i, j)
                    );
        }
        else
        {
            ray_col += readEnvironmentMap(outward_ray);
        }
    }
    else
    {
        ray_col += (1.0/probability_weight) * inward_ray.intersection.ray_material->emissive;
    }
    
    return ray_col;
}

/* The same as cPathtracer::regularShaderRecurse() but using iteration instead of recursion, kind of 
 an experimental function. 
 */
col3 cPathtracer::regularShaderIterate(Ray &ray, int i, int j)
{
    typedef struct ray_info_t{
        Ray ray;
        double probability;
    } ray_info;
    
    std::vector<ray_info> path;
    
    Ray outward_ray = ray;
    
    ray_info outward_ray_info;
    outward_ray_info.ray = outward_ray;
    outward_ray_info.probability = std::max(0.1, (outward_ray.intersection.ray_material->emissive.r
                                                 + outward_ray.intersection.ray_material->emissive.g
                                                 + outward_ray.intersection.ray_material->emissive.b) / 3.0);
    path.push_back(outward_ray_info);
    
    col3 ray_col(0.0, 0.0, 0.0);
    for (;;)
    {
        Ray inward_ray = outward_ray;
        
        outward_ray.o = inward_ray.intersection.point + (inward_ray.intersection.normal * 0.00005); //small number, TODO: make this better
        outward_ray.d = inward_ray.intersection.normal;
        outward_ray.intersection.hit = false;
        
        //TODO: make this way way better instead of the bizarre sampling that it is now
        double probability_weight;
        probability_weight = inward_ray.intersection.ray_material->emissive.r
        + inward_ray.intersection.ray_material->emissive.g
        + inward_ray.intersection.ray_material->emissive.b;
        probability_weight *= (1.0/3.0);
        probability_weight = std::max(probability_weight, 0.1);
        
        double termination_sample = dis_zero_to_one(gen);
        
        if (termination_sample < probability_weight)
        {
            break;
        }
        else
        {
            outward_ray.d = sampleHemisphere(outward_ray.d);
            
            intersectScene(outward_ray);
            
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
    
    col3 past_col (0.0, 0.0, 0.0);
    if (path.back().ray.intersection.hit)
    {
        past_col = (1.0/path.back().probability) * path.back().ray.intersection.ray_material->emissive;
    }
    else
    {
        past_col = readEnvironmentMap(path.back().ray);
    }
    
    for (long i = (path.size() - 1) - 1; i >= 0; i--)
    {
        double lambert_factor = path[i+1].ray.d * path[i].ray.intersection.normal;
        
        past_col = lambert_factor * (1.0/ (1.0 - path[i].probability)) * path[i].ray.intersection.ray_material->diffuse.apply_r(past_col);
    }
    

    
    return past_col;
}

/* The regular default shader for a ray in the scene, takes in ray, sets the colour for that ray
 
 Currently using a straight up pathtracing method with a simple BRDF from the Lambertian diffuse model
 */
void cPathtracer::regularShader(Ray &ray, int i, int j)
{
    col3 regular_col = col3(0.0,0.0,0.0); //the final colour to be set for the given primary ray
    
    if (ray.intersection.hit)
    {
        //For each primary ray we want to take a whole number of samples because the shading to
        //each primary ray makes more of an impact than the primary ray, which only really affects
        //anti-aliasing.
        //
        //So we can ignore collecting more primary rays in exchange for taking more secondary rays.
        for (int samples = 0; samples < PRIMARY_SAMPLES; samples++)
        {
            regular_col += regularShaderIterate(ray, i, j);
        }
        
        regular_col = regular_col * (1.0/PRIMARY_SAMPLES);        
    }
    else
    {
        regular_col += readEnvironmentMap(ray);
    }
    
    setImage(image_.buffer, i, j, regular_col, pass_number_);  
}

/* The normals shader for a ray in the scene, shading each ray with the normal vector of the surface 
 it hits with the x value as red, y as blue, z as green. Takes in ray, sets the colour for that ray
 */
void cPathtracer::normalsShader(Ray ray, int i, int j)
{
    col3 normals_col (0.0, 0.0, 0.0);
    if (ray.intersection.hit)
	{
		normals_col = 0.5 * ray.intersection.normal + 0.5;
	}
    
    setImage(image_.normals_buffer, i, j, normals_col, pass_number_);
}

/* The depth shader for a ray in the scene, shading each ray with the depth value on the surface of
 the object, taken from the t-value of the intersection.
 */
void cPathtracer::depthShader(Ray ray, int i, int j)
{
    col3 depth_col (0.0, 0.0, 0.0);
    if (ray.intersection.hit)
    {
       depth_col = col3 (ray.intersection.t_value,ray.intersection.t_value,ray.intersection.t_value);
    }
    
    setImage(image_.depth_buffer, i, j, depth_col, pass_number_);
}

/* The material shader, sets the pixels of each of the corresponding buffers for material colours 
 to the values from the material of the object hit by the primary ray.
 */
void cPathtracer::materialShader(Ray ray, int i, int j)
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
        emissive_col = readEnvironmentMap(ray);
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
        
        const unsigned int key_seq[] = {cimg::keyS};
        if (image_.regular_display->is_key_sequence(key_seq, 1))
        {
            break;
        }
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
    
    pass_number_ = 0;
    
    setCamera(point3(0.0, 0.0 ,0.0), point3(0.0, 0.0, -1.0), 60);
}
