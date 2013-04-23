#include <cstdio>
#include <cmath>
#include <vector>

#include "CImg.h"

#include "carrizo.h"
#include "rendModel.h"
#include "cObject.h"
#include "vecmat.h"
#include "cPathtracer.h"

using namespace vecmat;

void debugSceneTESTOBJECT()
{
    cPathtracer pt;
	
	cObject model1(CTESTOBJECT);
    pt.addObject(model1);
    
    cObject model2 (CBOX);
    model2.translate(vec3(0.0,0.0,0.0));
    model2.scale(vec3 (0.2,0.25,1.0));
    pt.addObject(model2);
    
	pt.setDimensions(640,480);
    pt.setCamera( point3(-0.5, 0.5, 1.0), point3(0.0,0.0,0.0), 60.0); //test scene

	pt.render();
}

void debugSceneCone()
{
    cPathtracer pt;

    cObject model3 ("/Users/william/Downloads/assimp-master/test/models/NFF/NFF/cone.nff");

    model3.scale(vec3(10.0, 10.0, 10.0));
    pt.addObject(model3);
    
	
	pt.setDimensions(640,480);
    pt.setCamera( point3(30.0, 30.0, 30.0), point3(0.0,0.0,0.0), 60.0); //cone
    
	pt.render();
}

void debugSceneSpider()
{
    cPathtracer pt;
	
    cObject spider_obj ("/Users/william/Downloads/assimp-master/test/models/OBJ/spider.obj");
    
	pt.addObject(spider_obj);
    
    cObject floor_obj (CBOX);
    floor_obj.scale(vec3 (250.0, 1.0, 250.0));
    floor_obj.translate(vec3(0.0, -43.3, 0.0));
    pt.addObject(floor_obj);
	
	pt.setDimensions(640,480);
    pt.setCamera( point3(200.0, 200.0, 200.0), point3(0.0,0.0,0.0), 55.0); //spider
	pt.render();
}

int main(int argc, const char* argv[])
{    
	debugSceneSpider();
	return 1;
}