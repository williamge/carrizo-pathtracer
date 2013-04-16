//
//  cPathtracer.h
//  carrizo-pathtracer
//
//  Created by William Gelhar on 2013-04-16.
//
//

#ifndef __carrizo_pathtracer__cPathtracer__
#define __carrizo_pathtracer__cPathtracer__

#include <iostream>
#define cimg_use_png

#include "CImg.h"
#include "vecmat.h"
#include "carrizo.h"
#include <vector>

class cPathtracer{
private:
    struct {
        point3 origin;
        point3 look_at;
        float fov;
    } camera;
    
    struct{
        cimg_library::CImg<float> *buffer;
        cimg_library::CImg<float> *normals_buffer;
        cimg_library::CImg<float> *depth_buffer;
        int width, height;
    } image;
    
    std::vector<cObject *> objects;
    std::vector<rendModel> render_models;
    
    void intersectScene(Ray* ray);
    void shadePixel(int i, int j, vec3 &direction_vector, vec3 &x_unit, vec3 &y_unit, float factor_);
    
    col3 regularShader(Ray ray);
    col3 normalsShader(Ray ray);
    col3 depthShader(Ray ray);
	
public:
    cPathtracer();
    void render();
    void addObject(cObject *obj);
    void setCamera(point3 origin, point3 look_at, float fov);
    void setDimensions(int width, int height);
    
};


#endif /* defined(__carrizo_pathtracer__cPathtracer__) */
