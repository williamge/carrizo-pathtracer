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

#include "vecmat.h"

typedef struct ray_t Ray;

using namespace vecmat;

const int MAX_PASS_NUMBER = 256;

class rendModel;
class cObject;
class cShader;

typedef struct renderTriangle_t renderTriangle;

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
        double factor;
    } camera_;
    
    struct{
        int width, height;
    } image_;
    
    std::vector<std::shared_ptr<cShader>> shaders_;
    std::vector<std::shared_ptr<cShader>> shaders_one_pass_;
    
    std::vector<cObject *> objects_;
    std::vector<std::shared_ptr<rendModel>> render_models_;
    
    std::vector<renderTriangle> emissive_triangles_;
    
    unsigned int pass_number_;
    
    bool readyObjects();
    bool cleanObjects();
    void renderPass(camera_vectors &render_vectors);
    
    void shadePixel(int i, int j, camera_vectors &render_vectors);
	
public:
    ~cPathtracer();
    cPathtracer();
     
    col3 readEnvironmentMap(Ray &ray);
    
    void intersectScene(Ray &ray);
    
    std::pair<renderTriangle, double> getRandomLight();
    
    void render();
    void addObject(cObject &obj);
    
    void addShader(std::shared_ptr<cShader> &shader);
    void addRunOnceShader(std::shared_ptr<cShader> &shader);
    
    void setCamera(point3 origin, point3 look_at, double fov);
    void setDimensions(int width, int height);
    std::pair<int, int> getDimensions();

};


#endif /* defined(__carrizo_pathtracer__cPathtracer__) */
