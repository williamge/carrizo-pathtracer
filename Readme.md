#Carrizo Pathtracer


##What it is

Right now, nothing really, what will it be one day? A nice pathtracer written in C++, basically a raytracer that doesn't stop at the first bounce or so arbitrarily but considers the path of each ray of light to better model the lighting.

##What it does

So far it can create and traverse BVH structures, and compute intersections for triangles, but there's nothing set up to load objects or to shade them.

###File listing and Installation Notes
- **/image log/** : A listing of pictures rendered by the program over time
- **/include/** : Necessary libraries included with the program
- **/tests/** : Google Test unit tests for the project, details are included in a readme file inside the folder
- **carrizo.[cpp/h]** : The main file for the program, serves as the basis for the setting up of a scene and the rendering of the scene
- **cObject.[cpp/h]** : The file that holds the higher level scene objects for the program
- **Makefile** : Simple file to run GNU Make with, quite barebones and tries to compile with clang
- **rendModel.[cpp/h]** : Holds the rendering model object that is derived from **cObject** for rendering purposes. **carrizo** uses **cObject** to manage the scene then tells **cObject** to computer **rendModel** objects for use when rendering the scene.
- **UML - Class.png** : A UML Class diagram for the project, right now it is a bit out of date.
- **vecmat.[cpp/h]** : Vector math definitions for the project, simple stuff like defining a 3D vector and the equivalent operations.

The project uses the *CImg* library for image processing tasks, a copy of this library is included with the project. *CImg* requires the pthread library and *X11* or *GDI* to be available for the compilation of the program. *libpng* and *zlib* is required for saving and loading image files. *cmath* is required by the program, for obvious math reasons.

###List of things implemented
- Basic backend for rendering
- BVH creation
- BVH traversal
- Ray intersection

###Next steps to take
- Unit tests
- Hook up the asset importer so there's something to see