#ifndef _LUCID_SHADER_
#define _LUCID_SHADER_

#include <string>
#include <vector>
#include <GL/glew.h>

GLuint CreateProgram(const std::vector<GLuint>&);
GLuint CreateShader(GLenum, const std::string&);

#endif
