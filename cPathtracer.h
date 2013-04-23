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

using namespace vecmat;

const int MAX_PASS_NUMBER = 32;

class rendModel;
class cObject;

typedef struct camera_vectors_t {
    vec3 direction_vector;
    vec3 x_unit;
    vec3 y_unit;
} camera_vectors;

class cPathtracer{
private:
    struct {
        point3 origin;
        point3 look_at;
        double fov;
    } camera_;
    
    struct{
        cimg_library::CImg<double> *buffer;
        cimg_library::CImg<double> *normals_buffer;
        cimg_library::CImg<double> *depth_buffer;
        
        cimg_library::CImgDisplay *regular_display;
        cimg_library::CImgDisplay *normals_display;
        cimg_library::CImgDisplay *depth_display;
        
        int width, height;
    } image_;
    
    std::vector<cObject*> objects_;
    std::vector<rendModel*> render_models_;
    
    unsigned int pass_number_;
    
    void intersectScene(Ray &ray);
    static void setImage(cimg_library::CImg<double> *image, int i, int j, col3& colour, unsigned int pass_number);
    void shadePixel(int i, int j, camera_vectors &render_vectors, double factor);
    
    col3 regularShader(Ray ray);
    col3 normalsShader(Ray ray);
    col3 depthShader(Ray ray);
    
    bool readyObjects();
    void renderPass(camera_vectors &render_vectors);
	
public:
    ~cPathtracer();
    cPathtracer();
    
    void render();
    void addObject(cObject *obj);
    void setCamera(point3 origin, point3 look_at, double fov);
    void setDimensions(int width, int height);
    
};


#endif /* defined(__carrizo_pathtracer__cPathtracer__) */
