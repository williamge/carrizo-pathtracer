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
    triangle.vertices[0] = point3(0.0, 0.5, 0.0);
    triangle.vertices[1] = point3(-0.5, -0.5, 0.0);
    triangle.vertices[2] = point3(0.5, -0.5, 0.0);
    
    triangles[0] = triangle;
}

void cObject::makeCBOX()
{
    triangles = new objectTriangle[12];
    triangle_count = 12;
    
    //front side
    objectTriangle triangle1;
    triangle1.vertices[0] = point3(-0.5, 0.5, 0.5); //top left
    triangle1.vertices[1] = point3(-0.5, -0.5, 0.5); //bottom left
    triangle1.vertices[2] = point3(0.5, 0.5, 0.5); //top right
    
    objectTriangle triangle2;
    triangle2.vertices[0] = point3(0.5, 0.5, 0.5); //top right
    triangle2.vertices[1] = point3(-0.5, -0.5, 0.5); //bottom left
    triangle2.vertices[2] = point3(0.5, -0.5, 0.5); //bottom right
    
    //back side
    objectTriangle triangle3;
    triangle3.vertices[0] = point3(0.5, 0.5, -0.5); //top left
    triangle3.vertices[1] = point3(-0.5, -0.5, -0.5); //bottom left
    triangle3.vertices[2] = point3(-0.5, 0.5, -0.5); //top right
    
    objectTriangle triangle4;
    triangle4.vertices[0] = point3(0.5, 0.5, -0.5); //top right
    triangle4.vertices[1] = point3(0.5, -0.5, -0.5); //bottom left 
    triangle4.vertices[2] = point3(-0.5, -0.5, -0.5); //bottom right
    
    //bottom side
    objectTriangle triangle5;
    triangle5.vertices[0] = point3(-0.5, -0.5, 0.5); //close left
    triangle5.vertices[1] = point3(-0.5, -0.5, -0.5); //far left
    triangle5.vertices[2] = point3(0.5, -0.5, -0.5); //far right
    
    objectTriangle triangle6;
    triangle6.vertices[0] = point3(0.5, -0.5, -0.5); //far right
    triangle6.vertices[1] = point3(-0.5, -0.5, 0.5); //close left
    triangle6.vertices[2] = point3(0.5, -0.5, 0.5); //close right
    
    //top side
    objectTriangle triangle7;
    triangle7.vertices[0] = point3(-0.5, 0.5, 0.5); //close left
    triangle7.vertices[1] = point3(-0.5, 0.5, -0.5); //far left
    triangle7.vertices[2] = point3(0.5, 0.5, -0.5); //far right
    
    objectTriangle triangle8;
    triangle8.vertices[0] = point3(0.5, 0.5, -0.5); //far right
    triangle8.vertices[1] = point3(-0.5, 0.5, 0.5); //close left
    triangle8.vertices[2] = point3(0.5, 0.5, 0.5); //close right
    
    //left side
    objectTriangle triangle9;
    triangle9.vertices[0] = point3(-0.5, -0.5, -0.5); //far bottom
    triangle9.vertices[1] = point3(-0.5, 0.5, -0.5); //far top
    triangle9.vertices[2] = point3(-0.5, 0.5, 0.5); //close top
    
    objectTriangle triangle10;
    triangle10.vertices[0] = point3(-0.5, -0.5, -0.5); //far bottom
    triangle10.vertices[1] = point3(-0.5, 0.5, 0.5); //close top
    triangle10.vertices[2] = point3(-0.5, -0.5, 0.5); //close bottom
    
    //right side
    objectTriangle triangle11;
    triangle11.vertices[0] = point3(0.5, -0.5, -0.5); //far bottom
    triangle11.vertices[1] = point3(0.5, 0.5, -0.5); //far top
    triangle11.vertices[2] = point3(0.5, 0.5, 0.5); //close top
    
    objectTriangle triangle12;
    triangle12.vertices[0] = point3(0.5, -0.5, -0.5); //far bottom
    triangle12.vertices[1] = point3(0.5, 0.5, 0.5); //close top
    triangle12.vertices[2] = point3(0.5, -0.5, 0.5); //close bottom
    
    triangles[0] = triangle1;
    triangles[1] = triangle2;
    triangles[2] = triangle3;
    triangles[3] = triangle4;
    triangles[4] = triangle5;
    triangles[5] = triangle6;
    triangles[6] = triangle7;
    triangles[7] = triangle8;
    triangles[8] = triangle9;
    triangles[9] = triangle10;
    triangles[10] = triangle11;
    triangles[11] = triangle12;
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
        case CBOX:
            makeCBOX();
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
	triangle1.vertices[1] = point3(-0.1, 0.1, -0.25);
	triangle1.vertices[2] = point3(0.0, 0.1, -0.25);
	triangle1.vertices[0] = point3(-0.1, -0.1, -0.25);
	
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
void cObject::translate(vec3 trans_vec)
{
    translate_vector = trans_vec;
}
void cObject::rotate(vec3 rotate_vec)
{
    rotate_vector = rotate_vec;
}
void cObject::scale(vec3 scale_vec)
{
    scale_vector = scale_vec;
}
void cObject::assignMaterial()
{
    
}