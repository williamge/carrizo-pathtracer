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

/*

Ttility functions for finding the minimum and maximum of two numbers.
TODO: put this somewhere where it would make more sense to be.

*/
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

/*

Creates a Bounding Volume Hierarchy node, doing the actual processing as 
opposed to "constructBVH(...)"

*/
BVHnode * rendModel::constructBVHSub(renderTriangle *triangle_list, std::vector<int> index_list, bbox *bounds_list)
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
        std::vector<int> triangle_list_;
        triangle_list_.push_back( index_list[0] );
        node->triangle_list = triangle_list_;
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

    //determine the axis to split the current volume by, represented by "dim"
    
    float split;
    
    int dim;
    

    //determine the biggest splits to split the volume by
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
    
    //store each child volume in either the left or right node, like a binary tree
    std::vector<int> left_index;
    std::vector<int> right_index;
    
    //determine which node to store the remaining bounding boxes in
    for (int i = 0; i < index_list.size(); i++)
    {
        bbox curr_bound = bounds_list[index_list[i]];
        if (curr_bound.low.data[dim] <= node->bounds.low.data[dim] + split)
        {
            left_index.push_back(i);
        }
        if (curr_bound.high.data[dim] >= node->bounds.low.data[dim] + split)
        {
            right_index.push_back(i);
        }
    }    

    //check to see if the BVH changed anything, stop right here if it didn't
    if (left_index.size() == index_list.size() || right_index.size() == index_list.size())
    {
        node->left = NULL;
        node->right = NULL;
        node->triangle_list = index_list;
    }
    else
    {
        if (left_index.size() > 0)
        {
            node->left = constructBVHSub(triangle_list, left_index, bounds_list);         
        }
        else
        {
            node->left = NULL;
        }
        if (right_index.size() > 0)
        {
            node->right = constructBVHSub(triangle_list, right_index, bounds_list);
        }
        else
        {
            node->right = NULL;
        }
    }
    
    return node;    
}

/*

Top level method of creating a BVH object for the scene, returning the 
root node to the constructed BVH.

*"tri_list" is a pointer to the list of triangles to place in to the BVH
*"tri_count" is the number of triangles in the scene
*"bounds_list" is a pointer to the list of bounding boxes for the scene,
    having been constructed by the "rendModel" constructor

*/
BVHnode * rendModel::constructBVH(renderTriangle *triangle_list, int triangle_count, bbox *bounds_list)
{
    std::vector<int> remaining_triangles;
    
    for (int i = 0; i < triangle_count; i++)
    {
        remaining_triangles.push_back(i);
    }
    
    return constructBVHSub(triangle_list, remaining_triangles, bounds_list);
}

/*

The constructor for a rendModel, takes in a list of triangles and their 
count, creates corresponding renderTriangles, creates bounding boxes for
the triangles, and then creates useful information for the triangle 
such as uv coordinates for the intersection plane and the normal.

*/
rendModel::rendModel(cObject * source_object)
{
    //rendModel(objectTriangle *tris, int tri_count);
	triangles = new renderTriangle[source_object->triangle_count];
	triangle_count = source_object->triangle_count;
    
    struct bbox *bounding_boxes = new struct bbox[triangle_count];
    
	
	for (int i=0;i<triangle_count;i++)
	{
		point3 p1 = source_object->triangles[i].vertices[0];
		point3 p2 = source_object->triangles[i].vertices[1];
		point3 p3 = source_object->triangles[i].vertices[2];
        
        //apply transformations to each point
        p1.apply(source_object->scale_vector);
        p2.apply(source_object->scale_vector);
        p3.apply(source_object->scale_vector);
        
        //TODO: implement rotate transformation
        
        p1 + source_object->translate_vector;
        p2 + source_object->translate_vector;
        p3 + source_object->translate_vector;
        
        //bounding boxes are defined by two points on opposite sides of the box
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

/*
 Private utility function for bounding box - ray intersections, returns true if there's a proper intersection, 
 false otherwise.
 
 Branchless AABB-ray test from 
    http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
  
 */
bool rendModel::boxIntersection(bbox b, Ray ray, vec3 inv_dir)
{
    double tx1 = (b.low.x - ray.o.x) * inv_dir.x;
    double tx2 = (b.high.x - ray.o.x) * inv_dir.x;
    
    double tmin = min(tx1, tx2);
    double tmax = max(tx1, tx2);
    
    double ty1 = (b.low.y - ray.o.y) * inv_dir.y;
    double ty2 = (b.high.y - ray.o.y) * inv_dir.y;
    
    tmin = max(tmin, min(ty1,ty2));
    tmax = min(tmax, max(ty1,ty2));
    
    double tz1 = (b.low.z - ray.o.z) * inv_dir.z;
    double tz2 = (b.high.z - ray.o.z) * inv_dir.z;
    
    tmin = max(tmin, min(tz1, tz2));
    tmax = min(tmax, max(tz1, tz2));
    
    return (tmax >= max(0.0, tmin));
}

/*

Object defined method to intersect a ray with the model, traversing the BVH for the model 
in a smart way.

*/
bool rendModel::intersect(Ray& ray)
{
	int hit_triangle = -1;
    
    //bvh traversal
    
    if (!root)
    {
        return false;
    }
    
    //this looks like it should fail from divide by zero, but for reasons it doesn't, enjoy!
    vec3 inv_dir (1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z);
    
    
    /*
     gameplan: start from root BVHnode, add each child node (left and right), each time taking one node 
     from this stack and checking if the ray intersects with it, if ti does then we care about that 
     node, otherwise we discard it and move along, adding the triangles we'll need to check to a vector 
     to be checked at the end.     
     */
    
    std::vector<int> triangle_list;
    
    BVHnode * curr_node;
    std::vector<BVHnode *> node_stack;
    node_stack.push_back(root);
    

    while (node_stack.size() > 0)
    {
        //hey why do we go through all of this instead of starting from the front?
        //well because std::vector wants you do pop from the back, apparently it's faster
        //so here we are, taking nodes from the back of the list always
        curr_node = node_stack[node_stack.size() - 1];
        node_stack.pop_back();

        if (boxIntersection(curr_node->bounds, ray, inv_dir))
        {
            //add all the triangles (if the node has them) to a vector for later
            if (curr_node->triangle_list.size() > 0)
            {
                for (int tri=0; tri < curr_node->triangle_list.size(); tri++)
                {
                    triangle_list.push_back(curr_node->triangle_list[tri]);
                }
            }
            
            if (curr_node->left)
            {
                node_stack.push_back(curr_node->left);
            }
            if (curr_node->right)
            {
                node_stack.push_back(curr_node->right);
            }
        }
    }

    
    
    int i;
    
    //tri_list being the vector of triangles to be processed, filled in from the above BVH traversal
    for (int tri=0; tri < triangle_list.size(); tri++)
    {
        i = triangle_list[tri]; //take the index of the current triangle

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
			&& (!ray.intersection.hit || ray.intersection.t_value > intersect_t))
        {
            
            hit_triangle = i;
            
            ray.intersection.t_value = intersect_t;
            ray.intersection.point = point;
            /*
             If the dot product is less than 0 then we're hitting the front-face, if it's higher than 0 then we're 
             hitting the back-face. I don't want to only hit front-faces because there's no real reason to do it that 
             way in a pathtracer, so if we hit that side let's reverse the normal so we can treat that triangle as a 
             front-face instead and get correct shading.
             */
            if (ray.d * tnormal < 0.0)
            {
                ray.intersection.normal = tnormal;
            }
            else
            {
                ray.intersection.normal = -tnormal;
            }
            ray.intersection.hit = true;
		}
	}
	
	if (hit_triangle > -1)
	{
		//TODO: set material in or something else
		return true;
	}
	return false;
}