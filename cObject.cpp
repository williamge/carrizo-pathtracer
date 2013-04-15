//
//  cObject.cpp
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "rendModel.h"
#include "cObject.h"

/* 
 Takes the cObject object and turns it in to a returned rendModel 
 to be rendered.
*/
rendModel cObject::addToRender()
{
    return rendModel (this);
}

/*
 Private function to create an identity triangle centered at (0.0, 0.0, 0.0) 
 */
void cObject::makeCTRIANGLE()
{
    triangles = new objectTriangle[1];
    triangle_count = 1;
    
    objectTriangle triangle;
    triangle.vertices[0] = point3(0, 0.5, 0.0);
    triangle.vertices[1] = point3(-0.5, -0.5, 0.0);
    triangle.vertices[2] = point3(0.5, -0.5, 0.0);
    
    triangles[0] = triangle;
}

/*
 Create an object in the scene from the provided objects.
 */
cObject::cObject(int option)
{
    translate_vector = vec3 (0.0, 0.0, 0.0);
    rotate_vector = vec3(0.0, 0.0, 0.0);
    scale_vector = vec3(1.0, 1.0, 1.0);
    
    switch (option)
    {
        case CTRIANGLE:
            makeCTRIANGLE();
    }
}

/* 
 Load object from filename.
 
 TODO: Actually load an object from a filename, right now it just serves as a temporary testbed of a scene
 */
cObject::cObject(const char *filename)
{
    translate_vector = vec3 (0.0, 0.0, 0.0);
    rotate_vector = vec3(0.0, 0.0, 0.0);
    scale_vector = vec3(1.0, 1.0, 1.0);
	/*
	TODO: big ol todo right here, see that filename argument? so far that does nothing,
	 that should be changed so it reads that filename and does smart stuff with it.
	*/
    triangles = new objectTriangle[3];
	triangle_count = 3;
	
    //left
	objectTriangle triangle1;
	triangle1.vertices[0] = point3(-0.1, 0.1, -0.25);
	triangle1.vertices[2] = point3(0.0, 0.1, -0.25);
	triangle1.vertices[1] = point3(-0.1, -0.1, -0.25);
	
	triangles[0] = triangle1;
	
	//right top
	objectTriangle triangle2;
	triangle2.vertices[0] = point3(-0.2, 0.1, -1.55);
	triangle2.vertices[2] = point3(0.3, 0.1, -1.50);
	triangle2.vertices[1]= point3(0.2, -0.1, -1.53);
	
	triangles[1] = triangle2;
    
    //right bottom
    objectTriangle triangle3;
	triangle3.vertices[0] = point3(-0.1, -0.15, -1.55);
	triangle3.vertices[2] = point3(0.4, -0.1, -1.50);
	triangle3.vertices[1]= point3(0.3, -0.3, -1.0);
	
	triangles[2] = triangle3;
}
void cObject::translate()
{
    
}
void cObject::rotate()
{
    
}
void cObject::scale(){
    
}
void cObject::assignMaterial(){
    
}