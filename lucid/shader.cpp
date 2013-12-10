#include <iostream>

#include "shader.hpp"

using namespace std;


Shader::Shader () {

	std::vector<GLuint> shaderList;
	shaderList.push_back( CreateShader(GL_VERTEX_SHADER, LUtils::readFile("shaders/vertexshader.glsl")) );
	shaderList.push_back( CreateShader(GL_FRAGMENT_SHADER, LUtils::readFile("shaders/fragmentshader.glsl")) );
	lucidShaderProgram = CreateProgram(shaderList);
	glDeleteShader( shaderList[0] );
	
	attribPosition				= glGetAttribLocation ( lucidShaderProgram, "position" );
	attribColor					= glGetAttribLocation ( lucidShaderProgram, "color" );
	
	uniformAmbientIntensity     = glGetUniformLocation ( lucidShaderProgram, "ambientColor" );
	uniformLightIntensity       = glGetUniformLocation ( lucidShaderProgram, "lightColor" );
	uniformLightPosition        = glGetUniformLocation ( lucidShaderProgram, "lightPosition" );
	uniformModelMatrix	 		= glGetUniformLocation ( lucidShaderProgram, "modelMatrix" );
	uniformNormalModelMatrix    = glGetUniformLocation ( lucidShaderProgram, "normalModelMatrix" );
	uniformCameraMatrix         = glGetUniformLocation ( lucidShaderProgram, "cameraMatrix" );
	uniformPerspectiveMatrix 	= glGetUniformLocation ( lucidShaderProgram, "perspectiveMatrix" );
	
	glUseProgram(lucidShaderProgram);
}


/** TODO : Apparently, program could be actived all runtime */
void Shader::useProgram (bool activate) {
	if ( activate )
		glUseProgram(lucidShaderProgram);
	else
		glUseProgram(0);
}

/** TODO : Pass a light instead of two vecs **/
void Shader::updateLighting (const glm::vec3& vecPosition, const glm::vec4& vecIntensity) {

	glUniform4fv (
		uniformLightIntensity,
		1,
		glm::value_ptr(vecIntensity));
		
	glUniform3fv (
		uniformLightPosition,
		1,
		glm::value_ptr(vecPosition));
}

void Shader::updateAmbient (const glm::vec4& vecIntensity) {

	glUniform4fv (
		uniformAmbientIntensity,
		1,
		glm::value_ptr(vecIntensity));
}

void Shader::updateModelMatrix (const glm::mat4& modelMatrix) {

	//glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv (
		uniformModelMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(modelMatrix));
	
	glm::mat3 normalMatrix(modelMatrix);
	glUniformMatrix3fv (
		uniformNormalModelMatrix,
		1,
		GL_FALSE,
		glm::value_ptr(normalMatrix));
		
	//glUseProgram(0);
}

void Shader::updateCameraMatrix (const glm::mat4& perspectiveMatrix, const glm::mat4& cameraMatrix) {

	/* TODO : Why that doesn't work if I use glUseProgram here (and in updateModelMatrix) ? */
	//glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv (
		uniformPerspectiveMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(perspectiveMatrix));
		
	glUniformMatrix4fv (
		uniformCameraMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(cameraMatrix));

	//glUseProgram(0);
}

GLuint Shader::CreateProgram (const std::vector<GLuint> &shaderList) {
	// Link
	GLuint program = glCreateProgram();
	for ( size_t i = 0; i < shaderList.size(); i++ )
		glAttachShader(program, shaderList[i]);
	glLinkProgram(program);
	
	// Check and log linking
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if ( status == GL_FALSE ) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &status);
		GLchar* infolog = new GLchar[status+1];
		glGetProgramInfoLog(program, status, NULL, infolog);
		
		cout << "Link failure : " << infolog << endl;
	} else
		cout << "Link success." << endl;
	
	for ( size_t i = 0; i < shaderList.size(); i++ )
		glDetachShader( program, shaderList[i] );
	
	return program;
}

GLuint Shader::CreateShader (GLenum type, const std::string &filedata) {
	// Compile
	GLuint shader = glCreateShader(type);
	const char* cfiledata = filedata.c_str();
	glShaderSource(shader, 1, &cfiledata, NULL);
	glCompileShader(shader);
	
	// Check and log compilation
	const char* strShaderType = NULL;
	switch (type) {
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
	}
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if ( status == GL_FALSE ) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);
		GLchar* infolog = new GLchar[status+1];
		glGetShaderInfoLog(shader, status, NULL, infolog);
		
		cout << "Compile failure in " << strShaderType << " shader : \n\t" << infolog << endl;
	} else
		cout << "Compile success in " << strShaderType << " shader." << endl;
		
	return shader;
}