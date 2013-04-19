//
//  rendModel.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef carrizo_pathtracer_rendModel_h
#define carrizo_pathtracer_rendModel_h

#include <vector>

#include "vecmat.h"
#include "ray.h"

class cObject;

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
	vec3 normal;
    vec3 vertex_normals[3];
	//material *mat;
} renderTriangle;

struct bbox {
    point3 low, high, centroid;
};

typedef struct BVHnode_t {
    BVHnode_t *left;
    BVHnode_t *right;
    std::vector<int> triangle_list;
    struct bbox bounds;
} BVHnode;


class rendModel
{
private:    
    BVHnode *root_;
    renderTriangle *triangles_;
    unsigned int triangle_count_;
    
    bbox boundsUnion(bbox box, point3 point);
    static bool boxIntersection(const bbox& b, const Ray& r, const vec3& inv_dir);
    void bvhTraversal(BVHnode* start, Ray &ray, std::vector<int> &triangle_list_out);
    
    BVHnode * constructBVHSub(renderTriangle *triangle_list, std::vector<int> index_list, bbox *bounds_list);
    BVHnode * constructBVH(renderTriangle *triangle_list, int triangle_count, bbox *bounds_list);
    
public:

    rendModel(cObject * sourceObject);
    bool intersect(Ray& ray);
};



#endif
