#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint createShaderFromFile(GLchar *path, GLenum shaderType);	