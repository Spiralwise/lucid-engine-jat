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
		/* TODO : How can I simplify uniforms updating ? */
	void updateLighting (const glm::vec3&, const glm::vec4&); /* TODO : Make workable for severable light */
	void updateAmbient (const glm::vec4&); /* TODO : Merge with updateLight(...) */
	void updateModelMatrix (const glm::mat4&);
	void updateCameraMatrix (const glm::mat4&, const glm::mat4&);
	// void updateNormalCameraMatrix (const glm::mat3&);
	// void updatePerspectiveMatrix (const glm::mat4&);
	
private:
	GLuint lucidShaderProgram;
	GLuint attribPosition;
	GLuint attribColor;

	GLint uniformAmbientIntensity; /* TODO : Merge with light intensity */
	GLint uniformLightIntensity;
	GLint uniformLightPosition;
	
	GLint uniformModelMatrix;
	GLint uniformNormalModelMatrix;
	GLint uniformCameraMatrix;
	GLint uniformPerspectiveMatrix;
	
	GLuint CreateProgram(const std::vector<GLuint>&);
	GLuint CreateShader(GLenum, const std::string&);
};

#endif
