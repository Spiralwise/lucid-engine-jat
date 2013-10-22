#include <iostream>

#include "shader.hpp"

using namespace std;

float getFrustumScale ( float fov ) { // -> Camera

	float fov_radian = fov * 3.14159f * 2.0f / 360.0f;
	return 1.0f / tan(fov_radian/2.0f);
} /* TODO : Oui, à l'arrache.... */

Shader::Shader () {

	fFrustumScale = getFrustumScale(45.0f); // -> Camera
	fZnear        = 1.0f; // -> Camera
	fZfar         = 10.0f; // -> Camera
	glm::mat4 matPerspectiveMatrix = glm::mat4 (0.0f); // -> Camera

	// Shaders
	std::vector<GLuint> shaderList;
	shaderList.push_back( CreateShader(GL_VERTEX_SHADER, LUtils::readFile("shaders/vertexshader.glsl")) );
	shaderList.push_back( CreateShader(GL_FRAGMENT_SHADER, LUtils::readFile("shaders/fragmentshader.glsl")) );
	lucidShaderProgram = CreateProgram(shaderList);
	glDeleteShader( shaderList[0] ); // TODO : Cette ligne doit être prise en charge par shader.h ?
	
	attribPosition				= glGetAttribLocation ( lucidShaderProgram, "position" );
	attribColor					= glGetAttribLocation ( lucidShaderProgram, "color" );
	
	uniformModelMatrix	 		= glGetUniformLocation ( lucidShaderProgram, "modelMatrix" );
	uniformPerspectiveMatrix 	= glGetUniformLocation ( lucidShaderProgram, "perspectiveMatrix" );
	
		// Perspective Matrix
	matPerspectiveMatrix[0].x = fFrustumScale/1.333f; // Frustum scale on X <-- Aspect ratio here
	matPerspectiveMatrix[1].y = fFrustumScale; // Frustum scale on Y
	matPerspectiveMatrix[2].z = (fZfar + fZnear) / (fZnear - fZfar);
	matPerspectiveMatrix[3].z = (2 * fZfar * fZnear) / (fZnear - fZfar);
	matPerspectiveMatrix[2].w = -1.0f;
	
	glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv ( uniformPerspectiveMatrix, 1, GL_FALSE, glm::value_ptr(matPerspectiveMatrix) );
	glUseProgram(0);
}

Shader::~Shader () {
	cout << "Shader going to be deleted\n";
}

/** TODO : Apparmement inutile, on peut laisser le programme actif tout le long de l'exécution */
void Shader::useProgram (bool activate) {
	if ( activate )
		glUseProgram(lucidShaderProgram);
	else
		glUseProgram(0);
}

void Shader::updateModelMatrix (const glm::mat4& modelMatrix) {
	glUniformMatrix4fv (
		uniformModelMatrix, 
		1, 
		GL_FALSE, 
		glm::value_ptr(modelMatrix));
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