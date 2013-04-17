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
#include "cObject.h"

struct renderTriangle_t;
typedef struct renderTriangle_t renderTriangle;

struct ray_t;
typedef struct ray_t Ray;

struct objecttriangle_t;
typedef struct objecttriangle_t objectTriangle;

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
    bbox boundsUnion(bbox box, point3 point);
    static bool boxIntersection(const bbox& b, const Ray& r, const vec3& inv_dir);
public:
    renderTriangle *triangles_;
    unsigned int triangle_count_;
    rendModel(cObject * sourceObject);
    BVHnode * constructBVHSub(renderTriangle *triangle_list, std::vector<int> index_list, bbox *bounds_list);
    BVHnode * constructBVH(renderTriangle *triangle_list, int triangle_count, bbox *bounds_list);
    bool intersect(Ray& ray);
};



#endif
