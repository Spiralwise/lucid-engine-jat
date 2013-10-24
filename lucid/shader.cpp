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
	
	uniformModelMatrix	 		= glGetUniformLocation ( lucidShaderProgram, "modelMatrix" );
	uniformPerspectiveMatrix 	= glGetUniformLocation ( lucidShaderProgram, "perspectiveMatrix" );
	
	glUseProgram(lucidShaderProgram);
}


/** TODO : Apparmement inutile, on peut laisser le programme actif tout le long de l'exécution */
void Shader::useProgram (bool activate) {
	if ( activate )
		glUseProgram(lucidShaderProgram);
	else
		glUseProgram(0);
}

void Shader::updateModelMatrix (const glm::mat4& modelMatrix) {

	//glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv (
		uniformModelMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(modelMatrix));
	//glUseProgram(0);
}

void Shader::updatePerspectiveMatrix (const glm::mat4& perspectiveMatrix) {

	/* TODO : Pourquoi ça ne marche pas si on utilise glUseProgram ici (et dans updateModelMatrix) ? */
	//glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv (
		uniformPerspectiveMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(perspectiveMatrix));

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