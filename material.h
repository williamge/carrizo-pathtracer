//
//  material.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-19.
//
//

#ifndef carrizo_pathtracer_material_h
#define carrizo_pathtracer_material_h

#include "vecmat.h"

using namespace vecmat;

typedef struct material_t {
    col3 diffuse;
    col3 specular;
    col3 emissive;
    
    col3 reflective;
    col3 transparent;
    //col3 opacity; //should i? //i'm not even sure what this means anymore
    
    float shininess; float shininess_strength;
    float reflectivity;
    float refractivity;
    
    material_t()
    {
        diffuse = col3(0.0, 0.0, 0.0);
        specular = emissive = reflective = transparent = col3(0.0, 0.0, 0.0);
        
        shininess = refractivity = 0.0;
        shininess_strength = refractivity = 1.0;
    };
    
} material;

#endif
