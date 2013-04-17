#include <cstdio>
#include <cmath>

#include "CImg.h"

#include "carrizo.h"
#include "rendModel.h"
#include "cObject.h"
#include "vecmat.h"
#include "cPathtracer.h"




int main(int argc, const char* argv[])
{
	cPathtracer pt;
	
	cObject *model1 = new cObject(CTESTOBJECT);
    //pt.addObject(model1);
     
    cObject *model2 = new cObject(CBOX);
    model2->translate(vec3(0.0,0.0,0.0));
    model2->scale(vec3 (0.2,0.25,1.0));
    //pt.addObject(model2);
    
    cObject *model3 = new cObject("/Users/william/Downloads/assimp-master/test/models/NFF/NFF/cone.nff");
    //model3->scale(vec3(10.0, 10.0, 10.0));
    pt.addObject(model3);
	
	/* prototype start *
	
	cObject *model1 ("filename");
	pt.addObject(model1);
	model1.translate(...)
	model1.rotate(...);
	model1.scale(...);
	model1.assignMaterial(...);
	
	* -at render call, add objects to scenary in form of triangles, etc. 
	
	* prototype end */
	
	pt.setDimensions(640,480);
    //pt.setCamera( point3(-0.5, 0.5, 1.0), point3(0.0,0.0,0.0), 60.0);
    pt.setCamera( point3(30.0, 30.0, 30.0), point3(0.0,0.0,0.0), 60.0);
	pt.render();
	return 1;
}