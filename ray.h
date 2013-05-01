//
//  ray.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-18.
//
//

#ifndef carrizo_pathtracer_ray_h
#define carrizo_pathtracer_ray_h

#include "material.h"

using namespace vecmat;

typedef struct ray_t{
	point3 o;
	vec3 d;
	
	//with material pointer and no hit, 4 + 4 + 3*4 + 3*4 = 32 bytes, aligned in cache
	struct {
		double t_value;
		point3 point;
		vec3 normal;
        std::shared_ptr<material> ray_material;
		bool hit; //get rid of
	} intersection;
	
	ray_t()
	{
		intersection.hit = false;
	}
} Ray;

#endif
