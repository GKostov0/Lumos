# Using GLEW
Using GLEW

#include <GL/glew.h> <br />
After initialisation OpenGL context: <br />
glewExperimental = GL_TRUE (Using modern and advanced modern OpenGL) <br />
glewInit(); <br />
Should return GLEW_OK. If it fails, ot returns the error <br />
Can read error with glewGetErrorString(result); <br />
Check if extention exists: <br />
if (!GLEW_EXT_framebuffer_object) {} <br />
wglew.h for Windows only functions <br />