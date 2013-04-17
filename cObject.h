//
//  cObject.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef carrizo_pathtracer_cObject_h
#define carrizo_pathtracer_cObject_h

#include "vecmat.h"

class rendModel;

enum {CTRIANGLE, CBOX, CTESTOBJECT};

typedef struct objecttriangle_t{
	point3 vertices[3];
	
} objectTriangle;

class cObject
{
private:
	objectTriangle *triangles; //why do we only count objectTriangle? should probably make a polygon thing too or something
	unsigned int triangle_count;
    vec3 translate_vector;
    vec3 rotate_vector;
    vec3 scale_vector;
    
    void makeCTRIANGLE();
    void makeCBOX();
    void makeCTESTOBJECT();
public:
    cObject(int option);
    cObject(const char *filename);
    void translate(vec3 trans_vec);
    void rotate(vec3 rotate_vec);
    void scale(vec3 scale_vec);
    void assignMaterial();	
    rendModel addToRender();
    
    friend class rendModel;
};

#endif
