#Image log

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-15-01.png)

One of the first images saved from the rendering, just a (buggy) render of the normals of a few triangles and a cube

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-15-02.png)

After the bug was fixed that made the projection of the scene on to the screen a bit weird

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-15-03.png)

The same scene but with the depth rendered instead of the normal vectors, just taking the t-value from the intersection for each ray.

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-17-01.png)

Importing models by file was implemented. This model is made up of about 12k triangles.

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-17-02.png)

The same day I also remembered that vertex normals were a thing and decided to interpolate them so there's some nice smoothing on low-poly models.

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-04-19-01.png)

Multiple passes of the image can happen now, as can be seen here with this image that was taken with 32 passes, each pixel's ray having been jittered a bit to reduce aliasing.

![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-05-01-01.png)

 Basic shading with a basic path tracing routine, this took who knows how many passes (it was a large number) and is still quite noisy, need to come up with some better sampling methods to lower the variance, maybe eventually implement bidirectional path tracing.
 
 ![image](https://bitbucket.org/williamg/carrizo-pathtracer/raw/master/image%20log/2013-05-03-01.png)
 
 Turns out there was a stupid bug causing the weird look of the last image, the recursive part of tracing a path through the scene wasn't computing the rendering equation correctly, now that's been fixed and here's the proof!
 