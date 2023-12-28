# OpenGLRayTracing
Ray tracing in OpenGL 4

# How to Run
Open the Visual Studio solution file _OpenGLRayTracing.sln_ located in the _OpenGLRayTracing/OpenGLRayTracing/_ directory.

Set your _Local Windows Debugger_ solution configuration in Visual Studio to _Release_ for optimal performance. 

Select the _Start Without Debugging_ icon to run the ray tracer.

You can edit the number of times the light ray bounces and the number of samples each pixel calculates by setting the values of the _BOUNCES_ and _SAMPLES_ constants at the top of the _Main.cpp_ file.

To make changes to or add spheres to the scene, go to the "create spheres and materials" comment in the _main_ function in _Main.cpp_

# Resources
Coding Adventure: Compute Shaders - Sebastian Lague (0:51-3:10)
https://www.youtube.com/watch?v=9RHGLZLUuwc

Ray tracing with OpenGL Compute Shaders (Part I) - LWJGL
https://github.com/LWJGL/lwjgl3-wiki/wiki/2.6.1.-Ray-tracing-with-OpenGL-Compute-Shaders-(Part-I)

OpenGL Tutorials - Victor Gordan
https://www.youtube.com/watch?v=XpBGwZNyUh0&list=PLPaoO-vpZnumdcb4tZc4x5Q-v7CkrQ6M-

Modern OpenGL Tutorial - Compute Shaders - Victor Gordan
https://www.youtube.com/watch?v=nF4X9BIUzx0
