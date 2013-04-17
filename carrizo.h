#ifndef M_PI
#define M_PI 3.14159
#endif

#ifndef CARRIZO_H
#define CARRIZO_H

#define cimg_use_png

#include <vector>

#include "CImg.h"

#include "vecmat.h"


class cObject;
class rendModel;


/*

-don't group the triangles, one triangle set per object, materials are per triangle
as any other way of identifying what they should look like either uses an index in the triangle
(which takes as much space as a pointer to a material) or wouldn't work well with a kd-tree

3*4*3 + 3*12 + 4 = 76
u,v and normal = 12 + 12 + 12

ray triangle interesection takes u, v and normal, which are 3*3*4 bytes, 

3 vertices, 3 vertex normals, u v and normal = 112 bytes = aligned */
typedef struct renderTriangle_t{
	//point3 vertices[3];
	point3 a;
	vec3 u, v;
	vec3 normal; //or vec3 normal[3] to interpolate along normals
    vec3 vertex_normals[3];
	//material *mat;
} renderTriangle;

typedef struct ray_t{
	point3 o;
	vec3 d;
	
	//with material pointer and no hit, 4 + 4 + 3*4 + 3*4 = 32 bytes, aligned in cache
	struct {
		float t_value;
		point3 point;
		vec3 normal;
		//material *mat;
		bool hit; //get rid of
	} intersection;
	
	ray_t()
	{
		intersection.hit = false;
	}	
} Ray;

#endif