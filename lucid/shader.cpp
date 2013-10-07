#include "shader.h"

#include <iostream>

using namespace std;

GLuint CreateProgram(const std::vector<GLuint> &shaderList) {
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
		
		cout << "Linker failure : " << infolog << endl;
	} else
		cout << "Linker success." << endl;
	
	for ( size_t i = 0; i < shaderList.size(); i++ )
		glDetachShader( program, shaderList[i] );
	
	return program;
}

GLuint CreateShader (GLenum type, const std::string &filedata) {
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