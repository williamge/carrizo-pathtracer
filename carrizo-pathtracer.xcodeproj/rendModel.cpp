//
//  rendModel.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>

rend_model::rend_model()
{
	triangles = new triangle_s[2];
	triangle_count = 2;
	
	triangle_s triangle1;
	point3 t1v1 = point3(-0.1, 0.1, -0.25);
	point3 t1v3 = point3(0.0, 0.1, -0.25);
	point3 t1v2 = point3(-0.1, -0.1, -0.25);
	
	triangle1.u = t1v2 - t1v1;
	triangle1.v = t1v3 - t1v1;
	triangle1.a = t1v1;
    
	triangle1.normal = cross(triangle1.u, triangle1.v);
	
	float lengt = triangle1.normal * triangle1.normal;
	
	triangle1.u = cross(t1v3 - t1v1, triangle1.normal)
    * (1.0 / lengt);
	triangle1.v = cross(triangle1.normal, t1v2 - t1v1)
    * (1.0 / lengt);
	
	triangles[0] = triangle1;
	
	
	triangle_s triangle2;
	point3 t2v1 = point3(0.2, 0.1, -0.55);
	point3 t2v3 = point3(0.3, 0.1, -0.50);
	point3 t2v2 = point3(0.2, -0.1, -0.53);
	
	triangle2.u = t2v2 - t2v1;
	triangle2.v = t2v3 - t2v1;
	triangle2.a = t2v1;
    
	triangle2.normal = cross(triangle2.u, triangle2.v);
	
 	lengt = triangle2.normal * triangle2.normal;
	
	triangle2.u = cross(t2v3 - t2v1, triangle2.normal)
    * (1.0 / lengt);
	triangle2.v = cross(triangle2.normal, t2v2 - t2v1)
    * (1.0 / lengt);
	
	triangles[1] = triangle2;
	
}


bool rend_model::intersect(Ray& ray)
{
	int hit_triangle = -1;
	for (int i = 0; i < triangle_count; i++)
	{
		float lengt = triangles[i].normal * triangles[i].normal;
        
		float t1 = -triangles[i].u * triangles[i].a;
		float t2 = -triangles[i].v * triangles[i].a;
		
		vec3 tnormal = triangles[i].normal *  (1.0/ sqrt(lengt));
		
		float denom = ray.d * tnormal;
		float numer = (triangles[i].a - ray.o) * tnormal;
		
		if (denom == 0 || numer == 0)
		{
			continue;
		}
		
		float intersect_t = numer/denom;
		
		vec3 point = ray.o + intersect_t * ray.d;
		
		float u = triangles[i].u * point + t1;
		if (u < 0){ continue;}
		float v = triangles[i].v * point + t2;
		if (v < 0 || u+v > 1){continue;}
		
		if ( intersect_t > 0.0 
			&& (!ray.intersection.hit || ray.intersection.t_value > intersect_t)
			&& (ray.d * tnormal < 0.0)){
            
            hit_triangle = i;
            
            ray.intersection.t_value = intersect_t;
            ray.intersection.point = point;
            ray.intersection.normal = tnormal;
            ray.intersection.hit = true;	
		}
	}
	
	if (hit_triangle > -1)
	{
		//set material in or somethin bro
		return true;
	}
	return false;
}