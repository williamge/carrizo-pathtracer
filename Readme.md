#Carrizo Pathtracer


##What it is

A nice pathtracer written in C++, basically a raytracer that doesn't stop at the first bounce or so arbitrarily but considers the path of each ray of light to better model the lighting.

##What it does

So far it can create and traverse BVH structures, compute intersections for triangles, import models by file, and then shade them from their vector normals and depth, not actually shading them mind you but creating images shaded with those two things.

###File listing and Installation Notes
- **/cmake/** : cmake-specific items such as modules
- **/contrib/** : Third-party code, mostly libraries that don't fit in with being installed separately
- **/docs/** : Documents related to the project such as UML diagrams
- **/image log/** : A listing of pictures rendered by the program over time
- **/include/** : Header files for the main program
- **/src/** : Source files for the main program
- **/tests/** : Google Test unit tests for the project, details are included in a readme file inside the folder
- **UML - Class.png** : A UML Class diagram for the project, right now it is a bit out of date.

The project uses the *CImg* library for image processing tasks, a copy of this library is included with the project. *CImg* requires the pthread library and *X11* or *GDI* to be available for the compilation of the program. *libpng* and *zlib* is required for saving and loading image files. 

The *Open Asset Importing Library* is used to import 3D assets into the program, it must be linked and included to the program, a copy is not supplied.

*cmath* is required by the program, for obvious math reasons.

###List of things implemented
- Basic backend for rendering
- BVH creation
- BVH traversal
- Ray intersection
- Importing models/assets by file
- Multiple rendering passes
- Materials and shading of diffused light through the Lambertian model
- Basic unbiased path tracing methods
- Selectable shaders

###Next steps to take
- More unit tests
- Better shading (specular, reflections, etc.)
- Faster shading