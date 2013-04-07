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
    return rendModel (triangles, triangle_count);
}

cObject::cObject(const char *filename)
{
	/*
	TODO: big ol todo right here, see that filename argument? so far that does nothing,
	 that should be changed so it reads that filename and does smart stuff with it.
	*/
    triangles = new objectTriangle[2];
	triangle_count = 2;
	
	objectTriangle triangle1;
	triangle1.vertices[0] = point3(-0.1, 0.1, -0.25);
	triangle1.vertices[2] = point3(0.0, 0.1, -0.25);
	triangle1.vertices[1] = point3(-0.1, -0.1, -0.25);
	
	triangles[0] = triangle1;
	
	
	objectTriangle triangle2;
	triangle2.vertices[0] = point3(-0.2, 0.1, -1.55);
	triangle2.vertices[2] = point3(0.3, 0.1, -1.50);
	triangle2.vertices[1]= point3(0.2, -0.1, -1.53);
	
	triangles[1] = triangle2;
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