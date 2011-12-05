//
//  rendModel.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef carrizo_pathtracer_rendModel_h
#define carrizo_pathtracer_rendModel_h

#include "carrizo.h"

class rendModel
{
public:
    triangle_s *triangles;
    int triangle_count;
    rendModel();
    bool intersect(Ray& ray);
};



#endif
