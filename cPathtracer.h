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
#include <vector>
#define cimg_use_png

#include "CImg.h"

#include "vecmat.h"
#include "ray.h"

#ifndef M_PI
#define M_PI 3.14159
#endif

class rendModel;
class cObject;

class cPathtracer{
private:
    struct {
        point3 origin;
        point3 look_at;
        float fov;
    } camera_;
    
    struct{
        cimg_library::CImg<float> *buffer;
        cimg_library::CImg<float> *normals_buffer;
        cimg_library::CImg<float> *depth_buffer;
        int width, height;
    } image_;
    
    std::vector<cObject *> objects_;
    std::vector<rendModel> render_models_;
    
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
