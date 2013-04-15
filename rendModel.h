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
    point3 low, high;
};

typedef struct BVHnode_t {
    BVHnode_t *left;
    BVHnode_t *right;
    std::vector<int> tri_list;
    struct bbox bounds;
} BVHnode;


class rendModel
{
private:
    BVHnode *root;
    static bool boxIntersection(bbox b, Ray r, vec3 inv_dir);
public:
    renderTriangle *triangles;
    int triangle_count;
    rendModel(cObject * sourceObject);
    //rendModel(objectTriangle *tris, int tri_count);
    BVHnode * constructBVHSub(renderTriangle *tri_list, std::vector<int> index_list, bbox *bounds_list);
    BVHnode * constructBVH(renderTriangle *tri_list, int tri_count, bbox *bounds_list);
    bool intersect(Ray& ray);
};



#endif
