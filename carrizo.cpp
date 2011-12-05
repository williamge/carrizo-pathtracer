#include "carrizo.h"
#include "CImg.h"
#include "rendModel.h"
#include "cObject.h"
#include "vecmat.h"
#include <cstdio>
#include <cmath>



using namespace cimg_library;


void cPathtracer::addObject(cObject *obj)
{
    objects.push_back(obj);
}

void cPathtracer::intersectScene(Ray& ray)
{
	
	for (int i = 0; i < render_models.size(); i++)
	{
		render_models[0]->intersect(ray);
	}
}

col3 cPathtracer::traceRay(Ray ray)
{
	intersectScene(ray);
	if (ray.intersection.hit)
	{
		//return (0.5 * ray.intersection.normal + 0.5);
		return ray.intersection.normal;
	}
	return col3(0,0,0);
}

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
	
	cObject *model1 ("filename");
	pt.addObject(model1);
	model1.translate(...)
	model1.rotate(...);
	model1.scale(...);
	model1.assignMaterial(...);
	
	* -at render call, add objects to scenary in form of triangles, etc. 
	
	* prototype end */
	
	
	pt.render(640,480);
	return 1;
}