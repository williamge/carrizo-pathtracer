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
#include "material.h"

using namespace vecmat;

class rendModel;

enum {CTRIANGLE, CBOX, CTESTOBJECT};

typedef struct objecttriangle_t{
	unsigned int vertices[3];
    unsigned int vertex_normal[3];
    bool vertex_normals;
    
    std::shared_ptr<material> triangle_material;
} objectTriangle;

class cObject
{
private:
    std::vector<point3> vertices_;
    std::vector<vec3> vertex_normals_;
    
    std::vector<objectTriangle> triangles_; //why do we only count objectTriangle? should probably make a polygon thing too or something
    
    std::vector<std::shared_ptr<material>> materials_;

    vec3 translate_vector_;
    vec3 rotate_vector_;
    vec3 scale_vector_;
    
    void makeCTRIANGLE();
    void makeCBOX();
    void makeCTESTOBJECT();
public:
    cObject(int option);
    cObject(std::string filename);
    
    void translate(vec3 trans_vec);
    void rotate(vec3 rotate_vec);
    void scale(vec3 scale_vec);
    void assignAllMaterial(std::shared_ptr<material> new_material);
    std::shared_ptr<rendModel> addToRender();
    
    point3 getVertex(unsigned int vertex);
    vec3 getVertexNormal(unsigned int vertex);
    
    unsigned int getNumTriangle();
    
    vec3 getTranslate();
    vec3 getRotate();
    vec3 getScale();
    
    std::vector<objectTriangle>::iterator begin() {return triangles_.begin();}
    std::vector<objectTriangle>::iterator end() {return triangles_.end();}
    

};

#endif
