//
//  cObject.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 11-12-04.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef carrizo_pathtracer_cObject_h
#define carrizo_pathtracer_cObject_h

class rendModel;

class cObject
{
public:
    cObject(const char *filename);
    void translate();
    void rotate();
    void scale();
    void assignMaterial();	
    rendModel * addToRender();
};

#endif
