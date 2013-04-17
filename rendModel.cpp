//
//  rendModel.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include "vecmat.h"
#include "rendModel.h"
#include "carrizo.h"
#include "cObject.h"


/*
 Given a bounding box and a point, boundsUnion expands the bounding box to include the 
 point if necessary, then returns the new box.
*/
bbox rendModel::boundsUnion(bbox box, point3 point)
{
    box.low.x = std::min(box.low.x, point.x);
    box.low.y = std::min(box.low.y, point.y);
    box.low.z = std::min(box.low.z, point.z);
    
    box.high.x = std::max(box.high.x, point.x);
    box.high.y = std::max(box.high.y, point.y);
    box.high.z = std::max(box.high.z, point.z);
    
    return box;
}

/*
Creates a Bounding Volume Hierarchy node, doing the actual processing as 
opposed to "constructBVH(...)"
*/
BVHnode * rendModel::constructBVHSub(renderTriangle *triangle_list, std::vector<int> index_list, bbox *bounds_list)
{
    BVHnode *node = new BVHnode;
    
    bbox centroid_bounds; //bounding box for the centroids of primitives in the index_list
    centroid_bounds.low = bounds_list[index_list[0]].centroid;
    centroid_bounds.high = bounds_list[index_list[0]].centroid;
    
    node->bounds.low = bounds_list[index_list[0]].low;
    node->bounds.high = bounds_list[index_list[0]].high;
    
    if (index_list.size() == 1)
    {
        node->triangle_list = index_list;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    
    for (int i = 0; i < index_list.size(); i++)
    {
        centroid_bounds = boundsUnion(centroid_bounds, bounds_list[index_list[i]].centroid);
        
        node->bounds = boundsUnion(node->bounds, bounds_list[index_list[i]].low);
        node->bounds = boundsUnion(node->bounds, bounds_list[index_list[i]].high);
    }

    //determine the axis to split the current volume by, represented by "dim"

    float split;
    int dim;    

    //determine the biggest splits to split the volume by
    if (centroid_bounds.high.x - centroid_bounds.low.x > centroid_bounds.high.y - centroid_bounds.low.y)
    {
        if (centroid_bounds.high.x - centroid_bounds.low.x > centroid_bounds.high.z - centroid_bounds.low.z)
        {
            dim = 0;
            split = centroid_bounds.low.x + centroid_bounds.high.x;
        }
        else
        {   dim = 2;
            split = centroid_bounds.low.z + centroid_bounds.high.z;
        }
    }
    else
    {
        if (centroid_bounds.high.y - centroid_bounds.low.y > centroid_bounds.high.z - centroid_bounds.low.z)
        {
            dim = 1;
            split = centroid_bounds.low.y + centroid_bounds.high.y;
        }
        else
        {
            dim = 2;
            split = centroid_bounds.low.z + centroid_bounds.high.z;
        }
    }
    
    split *= 0.5;
    
    //store each child volume in either the left or right node, like a binary tree
    std::vector<int> left_index;
    std::vector<int> right_index;
    
    //determine which node to store the remaining bounding boxes in
    for (int i = 0; i < index_list.size(); i++)
    {
        bbox curr_bound = bounds_list[index_list[i]];

        point3 triangle_centroid = curr_bound.low + curr_bound.high;
        triangle_centroid = triangle_centroid * 0.5;
        
        if (triangle_centroid.data[dim] < split)
        {
            left_index.push_back(index_list[i]);
        }
        else
        {
            right_index.push_back(index_list[i]);
        }
    }
    
    node->left = NULL;
    node->right = NULL;    
    
    if (centroid_bounds.low.data[dim] == centroid_bounds.high.data[dim])
    {
        node->triangle_list = index_list;
    }
    else
    {        
        if (left_index.size() > 0)
        {
            node->left = constructBVHSub(triangle_list, left_index, bounds_list);         
        }
        if (right_index.size() > 0)
        {
            node->right = constructBVHSub(triangle_list, right_index, bounds_list);
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
    printf("Creating rendModel\n");
    
	triangles_ = new renderTriangle[source_object->triangle_count_];
	triangle_count_ = source_object->triangle_count_;
    
    struct bbox *bounding_boxes = new struct bbox[triangle_count_];
    
	
	for (int i=0;i<triangle_count_;i++)
	{
		point3 p1 = source_object->triangles_[i].vertices[0];
		point3 p2 = source_object->triangles_[i].vertices[1];
		point3 p3 = source_object->triangles_[i].vertices[2];
        
        //TODO: make this more OOP friendly (i.e. source_object.getScaleVector() instead)
        
        //apply transformations to each point
        p1.apply(source_object->scale_vector_);
        p2.apply(source_object->scale_vector_);
        p3.apply(source_object->scale_vector_);
        
        //TODO: implement rotate transformation
        
        p1 + source_object->translate_vector_;
        p2 + source_object->translate_vector_;
        p3 + source_object->translate_vector_;
        
        //bounding boxes are defined by two points on opposite sides of the box
        bounding_boxes[i].low.x = std::min(p1.x, std::min(p2.x,p3.x));
        bounding_boxes[i].low.y = std::min(p1.y, std::min(p2.y,p3.y));
        bounding_boxes[i].low.z = std::min(p1.z, std::min(p2.z,p3.z));
        
        bounding_boxes[i].high.x = std::max(p1.x, std::max(p2.x,p3.x));
        bounding_boxes[i].high.y = std::max(p1.y, std::max(p2.y,p3.y));
        bounding_boxes[i].high.z = std::max(p1.z, std::max(p2.z,p3.z));
        
        bounding_boxes[i].centroid = 0.5 * (bounding_boxes[i].high + bounding_boxes[i].low);
        
		
		triangles_[i].a = p1;
		triangles_[i].normal = (p2 - p1).vecCross(p3 - p1);
		float nlength = triangles_[i].normal * triangles_[i].normal;
		
		triangles_[i].u = (p3 - p1).vecCross(triangles_[i].normal)
	    	* (1.0 / nlength);
		triangles_[i].v = triangles_[i].normal.vecCross(p2 - p1)
    		* (1.0 / nlength);
        
        triangles_[i].vertex_normals[0] = source_object->triangles_[i].vertex_normal[0];
        triangles_[i].vertex_normals[1] = source_object->triangles_[i].vertex_normal[1];
        triangles_[i].vertex_normals[2] = source_object->triangles_[i].vertex_normal[2];
	}
    
    printf("    Creating BVH for rendModel\n");    
    root_ = constructBVH(triangles_, triangle_count_, bounding_boxes);
    if (!root_)
    {
        exit(EXIT_FAILURE);
        //is this correct? can a rendmodel have no bvhnode? TODO: figure it out perhaps!
    }
    printf("    Finished creating BVH\n");
    
    printf("rendModel bounds: \n    low: %f, %f, %f \n      high: %f, %f, %f\n",
           root_->bounds.low.x,
           root_->bounds.low.y,
           root_->bounds.low.z,
           root_->bounds.high.x,
           root_->bounds.high.y,
           root_->bounds.high.z);
}

/*
 Private utility function for bounding box - ray intersections, returns true if there's a proper intersection, 
 false otherwise.
 
 Branchless AABB-ray test from 
    http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
  
 */
bool rendModel::boxIntersection(const bbox& b, const Ray& ray, const vec3& inv_dir)
{
    double tx1 = (b.low.x - ray.o.x) * inv_dir.x;
    double tx2 = (b.high.x - ray.o.x) * inv_dir.x;
    
    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);
    
    double ty1 = (b.low.y - ray.o.y) * inv_dir.y;
    double ty2 = (b.high.y - ray.o.y) * inv_dir.y;
    
    tmin = std::max(tmin, std::min(ty1,ty2));
    tmax = std::min(tmax, std::max(ty1,ty2));
    
    double tz1 = (b.low.z - ray.o.z) * inv_dir.z;
    double tz2 = (b.high.z - ray.o.z) * inv_dir.z;
    
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));
    
    return (tmax >= std::max(0.0, tmin));
}

/*

Object defined method to intersect a ray with the model, traversing the BVH for the model 
in a smart way.

*/
bool rendModel::intersect(Ray& ray)
{
	int hit_triangle = -1;
    
    //bvh traversal
    
    //this looks like it should fail from divide by zero, but for reasons it doesn't, enjoy!
    vec3 inv_dir (1.0 / ray.d.x, 1.0 / ray.d.y, 1.0 / ray.d.z);    
    
    /*
     gameplan: start from root BVHnode, add each child node (left and right), each time taking one node 
     from this stack and checking if the ray intersects with it, if ti does then we care about that 
     node, otherwise we discard it and move along, adding the triangles we'll need to check to a vector 
     to be checked at the end.     
     */
    
    std::vector<int> triangle_list;
    
    BVHnode* curr_node;
    std::vector<BVHnode *> node_stack;
    node_stack.push_back(root_);
    
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
            for (int tri=0; tri < curr_node->triangle_list.size(); tri++)
            {
                triangle_list.push_back(curr_node->triangle_list[tri]);
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

		float lengt = triangles_[i].normal * triangles_[i].normal;
        
		float t1 = -triangles_[i].u * triangles_[i].a;
		float t2 = -triangles_[i].v * triangles_[i].a;
		
		vec3 tnormal = triangles_[i].normal *  (1.0/ sqrt(lengt));
		
		float denom = ray.d * tnormal;
		float numer = (triangles_[i].a - ray.o) * tnormal;
		
		if (denom == 0 || numer == 0)
		{
			continue;
		}
		
		float intersect_t = numer/denom;
		
		vec3 point = ray.o + intersect_t * ray.d;
		
		float u = triangles_[i].u * point + t1;
		if (u < 0){ continue;}
		float v = triangles_[i].v * point + t2;
		if (v < 0 || u+v > 1){continue;}
		
		if ( intersect_t > 0.0 
			&& (!ray.intersection.hit || ray.intersection.t_value > intersect_t))
        {
            
            hit_triangle = i;
            
            ray.intersection.t_value = intersect_t;
            ray.intersection.point = point;

            //interpolate the normal for the point from the vertex normals
            vec3 vertex1_normal = triangles_[i].vertex_normals[0];
            vec3 vertex2_normal = triangles_[i].vertex_normals[1];
            vec3 vertex3_normal = triangles_[i].vertex_normals[2];
            ray.intersection.normal = (1.0 - (u + v)) * vertex1_normal + vertex2_normal * u + vertex3_normal * v;
            ray.intersection.normal.normalize();
            
            /*
             If the dot product is less than 0 then we're hitting the front-face, if it's higher than 0 then we're
             hitting the back-face. I don't want to only hit front-faces because there's no real reason to do it that
             way in a pathtracer, so if we hit that side let's reverse the normal so we can treat that triangle as a
             front-face instead and get correct shading.
             */
            if (ray.d * tnormal < 0.0)
            {
                ray.intersection.normal = ray.intersection.normal;
            }
            else
            {
                ray.intersection.normal = -ray.intersection.normal;
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