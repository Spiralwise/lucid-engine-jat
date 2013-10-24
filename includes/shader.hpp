#ifndef _LUCID_SHADER_
#define _LUCID_SHADER_

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.hpp"


class Shader {
	
public:
	Shader ();
	void useProgram (bool);
	void updateModelMatrix (const glm::mat4&);
	void updatePerspectiveMatrix (const glm::mat4&);
	
private:
	GLuint lucidShaderProgram;
	GLuint attribPosition;
	GLuint attribColor;

	GLint uniformModelMatrix;
	GLint uniformPerspectiveMatrix;
	
	GLuint CreateProgram(const std::vector<GLuint>&);
	GLuint CreateShader(GLenum, const std::string&);
};

#endif
