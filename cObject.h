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
	unsigned int vertices[3];
    unsigned int vertex_normal[3];
	
} objectTriangle;

class cObject
{
private:
    //stuff to replace that other stuff
    point3 *vertices_;
    unsigned int vertices_count_;
    vec3 *vertex_normals_;
    
	objectTriangle *triangles_; //why do we only count objectTriangle? should probably make a polygon thing too or something
	unsigned int triangle_count_;
    vec3 translate_vector_;
    vec3 rotate_vector_;
    vec3 scale_vector_;
    
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
    
    //for interfacing with other classes
    
    
    friend class rendModel;
};

#endif
