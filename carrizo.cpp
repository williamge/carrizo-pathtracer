#include "carrizo.h"
#include "CImg.h"
#include "rendModel.h"
#include "cObject.h"
#include "vecmat.h"
#include <cstdio>
#include <cmath>



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
    objects.push_back(obj);
}

/*
	Simply intersects each render model in the scene with the current ray, 
	to trace the ray.
*/
void cPathtracer::intersectScene(Ray& ray)
{
	
	for (int i = 0; i < render_models.size(); i++)
	{
		render_models[0].intersect(ray);
	}
}

/*
	"traceRay(Ray)" is to trace a ray in the scene, it calls 
	"intersectScene(Ray&)" to do the heavy lifting.

	"traceRay(Ray)" handles the more general parts of tracing a ray, like 
	how to shade the corresponding pixel while "intersectScene(Ray&)" 
	is used to actually trace the ray in the scene.

*/
col3 cPathtracer::traceRay(Ray ray)
{
	intersectScene(ray);
	if (ray.intersection.hit)
	{
		return (0.5 * ray.intersection.normal + 0.5);
		//return ray.intersection.normal;
	}
	return col3(0,0,0);
}

/*
	The almighty render function, takes in the width and height for the 
	picture and renders the current scene, displaying it onscreen with 
	CImg.
*/
void cPathtracer::render(int width, int height)
{
	image.width = width;
	image.height = height;
	image.buffer = new CImg<float> (width, height, 1, 3);
	for (int i = 0; i < objects.size(); i++)
	{
		render_models.push_back( objects[i]->addToRender() );
	}
	
	point3 origin;
	vec3 p_direction = vec3(0.0,0.0,-1.0) - origin;
	printf("p_direction: %f %f %f\n",p_direction.x,p_direction.y,p_direction.z);
	
	//Ray ray;
	
	int fov = 60;	
	float factor = 1.0/((double(height)/2)/tan(fov*M_PI/360.0));
		
	cimg_forXY(*(image.buffer), i,j)
	{
		Ray rayt;
		rayt.o = origin;
		
		rayt.d = p_direction;
		rayt.d.x = (-double(width)/2.0 + i)*factor;
		rayt.d.y = -(-double(height)/2.0 + j)*factor;
		rayt.intersection.hit = false;
		col3 col = traceRay(rayt);
		
		(*image.buffer)(i,j,0,0) = col.r;
		(*image.buffer)(i,j,0,1) = col.g;
		(*image.buffer)(i,j,0,2) = col.b;
		
	}
	image.buffer->display();
}

cPathtracer::cPathtracer()
{

}

int main(int argc, const char* argv[])
{
	cPathtracer pt;
	
	cObject *model1 = new cObject("filename.ext");
    pt.addObject(model1);
	
	/* prototype start *
	
	-cObject *model1 ("filename");
	-pt.addObject(model1);
	model1.translate(...)
	model1.rotate(...);
	model1.scale(...);
	model1.assignMaterial(...);
	
	* -at render call, add objects to scenary in form of triangles, etc. 
	
	* prototype end */
	
	
	pt.render(640,480);
	return 1;
}