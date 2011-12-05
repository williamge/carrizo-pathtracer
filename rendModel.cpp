//
//  rendModel.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "vecmat.h"
#include "rendModel.h"
#include "carrizo.h"
#include "cObject.h"
#include <iostream>


float min(const float a, const float b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}

float max(const float a, const float b)
{
    if (a > b)
    {
        return a;
    }
    return b;
}


BVHnode * rendModel::constructBVHSub(renderTriangle *tri_list, std::vector<int> index_list, bbox *bounds_list)
{
    BVHnode *node = new BVHnode;
    node->bounds.low.x = bounds_list[index_list[0]].low.x;
    node->bounds.low.y = bounds_list[index_list[0]].low.y;
    node->bounds.low.z = bounds_list[index_list[0]].low.z;
    
    node->bounds.high.x = bounds_list[index_list[0]].high.x;
    node->bounds.high.y = bounds_list[index_list[0]].high.y;
    node->bounds.high.z = bounds_list[index_list[0]].high.z;
    
    if (index_list.size() == 1)
    {
        std::vector<int> tri_list_;
        tri_list_.push_back( index_list[0] );
        node->tri_list = tri_list_;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    
    for (int i = 0; i < index_list.size(); i++)
    {
        node->bounds.low.x = min(node->bounds.low.x, bounds_list[index_list[i]].low.x);
        node->bounds.low.y = min(node->bounds.low.y, bounds_list[index_list[i]].low.y);
        node->bounds.low.z = min(node->bounds.low.z, bounds_list[index_list[i]].low.z);
        
        node->bounds.high.x = max(node->bounds.high.x, bounds_list[index_list[i]].high.x);
        node->bounds.high.y = max(node->bounds.high.y, bounds_list[index_list[i]].high.y);
        node->bounds.high.z = max(node->bounds.high.z, bounds_list[index_list[i]].high.z);
    }
    
    float split; //((node->bounds.high.x - node->bounds.low.x)/2,
                //(node->bounds.high.y - node->bounds.low.y)/2,
                //(node->bounds.high.z - node->bounds.low.z)/2);
    
    int dim;
    
    if (node->bounds.high.x - node->bounds.low.x > node->bounds.high.y - node->bounds.low.y)
    {
        if (node->bounds.high.x - node->bounds.low.x > node->bounds.high.z - node->bounds.low.z)
        {
            dim = 0;
            split = node->bounds.high.x - node->bounds.low.x;
        }
        else
        {   dim = 2;
            split = node->bounds.high.z - node->bounds.low.z;
        }
    }
    else
    {
        if (node->bounds.high.y - node->bounds.low.y > node->bounds.high.z - node->bounds.low.z)
        {
            dim = 1;
            split = node->bounds.high.y - node->bounds.low.y;
        }
        else
        {
            dim = 2;
            split = node->bounds.high.z - node->bounds.low.z;
        }
    }
    
    split = 0.5 * split;
    
    std::vector<int> left_index;
    std::vector<int> right_index;
    
    for (int i = 0; i < index_list.size(); i++)
    {
        bbox curr_bound = bounds_list[index_list[i]];
        if (curr_bound.low.data[dim] < node->bounds.low.data[dim] + split)
        {
            left_index.push_back(i);
        }
        if (curr_bound.high.data[dim] > node->bounds.low.data[dim] + split)
        {
            right_index.push_back(i);
        }
    }    
    if (left_index.size() == index_list.size())
    {
        node->left = NULL;
        node->right = NULL;
        node->tri_list = left_index;
    }
    else
    {
        node->left = constructBVHSub(tri_list, left_index, bounds_list);
        
        if (right_index.size() == index_list.size())
        {
            free(node->left);
            node->left = NULL;
            node->right = NULL;
            node->tri_list = right_index;            
        }
        else
        {
            node->right = constructBVHSub(tri_list, right_index, bounds_list);
        }
    }
    

    
    return node;    
}

BVHnode * rendModel::constructBVH(renderTriangle *tri_list, int tri_count, bbox *bounds_list)
{
    std::vector<int> remaining_tri;
    
    for (int i = 0; i < tri_count; i++)
    {
        remaining_tri.push_back(i);
    }
    
    return constructBVHSub(tri_list, remaining_tri, bounds_list);
}

rendModel::rendModel(objectTriangle *tris, int tri_count)
{
	triangles = new renderTriangle[tri_count];
	triangle_count = tri_count;
    
    struct bbox *bounding_boxes = new struct bbox[tri_count];
    
	
	for (int i=0;i<tri_count;i++)
	{
		point3 p1 = tris[i].vertices[0];
		point3 p2 = tris[i].vertices[1];
		point3 p3 = tris[i].vertices[2];
        
        bounding_boxes[i].low.x = min(p1.x, min(p2.x,p3.x));
        bounding_boxes[i].low.y = min(p1.y, min(p2.y,p3.y));
        bounding_boxes[i].low.z = min(p1.z, min(p2.z,p3.z));
        
        bounding_boxes[i].high.x = max(p1.x, max(p2.x,p3.x));
        bounding_boxes[i].high.y = max(p1.y, max(p2.y,p3.y));
        bounding_boxes[i].high.z = max(p1.z, max(p2.z,p3.z));
        
		
		triangles[i].a = p1;
		triangles[i].normal = (p2 - p1).vecCross(p3 - p1);
		float nlength = triangles[i].normal * triangles[i].normal;
		
		triangles[i].u = (p3 - p1).vecCross(triangles[i].normal)
	    	* (1.0 / nlength);
		triangles[i].v = triangles[i].normal.vecCross(p2 - p1)
    		* (1.0 / nlength);
	}
    
    root = constructBVH(triangles, triangle_count, bounding_boxes);
}


bool rendModel::intersect(Ray& ray)
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