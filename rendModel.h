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

#include "material.h"

using namespace vecmat;

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
    std::shared_ptr<material> triangle_material;
} renderTriangle;

typedef struct bbox_t {
    point3 low, high, centroid;
} bounding_box;

typedef struct BVHnode_t {
    std::shared_ptr<BVHnode_t> left;
    std::shared_ptr<BVHnode_t> right;
    std::vector<int> triangle_list;
    bounding_box bounds;
} BVHnode;


class rendModel
{
private:    
    std::shared_ptr<BVHnode> root_;
    std::vector<renderTriangle> triangles_;
    
    static bounding_box boundsUnion(bounding_box box, point3 point);
    static bool boxIntersection(const bounding_box& b, const Ray& r, const vec3& inv_dir);
    static void bvhTraversal(std::shared_ptr<BVHnode> start, Ray &ray, std::vector<int> &triangle_list_out);
    
    std::shared_ptr<BVHnode> constructBVHSub(const std::vector<int> &index_list, const std::vector<bounding_box> &bounds_list);
    std::shared_ptr<BVHnode> constructBVH(const std::vector<bounding_box> &bounds_list);
    
public:

    rendModel(cObject &sourceObject);
    ~rendModel();
    
    bool intersect(Ray& ray);
};



#endif
