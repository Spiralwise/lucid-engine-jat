// Standard Libraries
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

// OpenGL Libraries>
#include <GL/glew.h>
#define GLFW_DLL
#include <GL/glfw.h>
#include <glm/glm.hpp>

// Lucid Includes
#include "shader.h"

using namespace std;
using namespace glm;


/**
 * LUCID_UTILS
 * Put a file content in a string.
 * Useful for shader programs reading.
 */
string readFile( const string &filename ) {
	string output = "";
	
	ifstream file(filename.c_str());
	string line;
	if ( file.is_open() ) {
		while ( file.good() ) {
			getline (file, line);
			output += line + "\n";
		}
		file.close();
	}
	
	return output;
}


int main( int argc, char** argv ) {

    cout << "Lucid Engine v0.0.0" << endl;
	
	double dMeanRenderTime = .0f;
	unsigned uRenderTimeNumber = 0; // TODO En faire une classe dans le cadre de l'étude des performances.
	double dLastTime; 

    /** GLFW **/
    if ( !glfwInit() ) {
        cout << "Failed to initialize GLFW. The application is terminated." << endl;
        return EXIT_FAILURE;
    }

    // FIXME What is it ?
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if ( !glfwOpenWindow( 1024, 768, 0, 0, 0, 0, 32, 0, (int) GLFW_WINDOW ) ) {
        cout << "Failed to open GLFW window. The application is terminated." << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    cout << "GLFW initialized." << endl;

	
    /** GLEW/OpenGL **/
    if ( glewInit() != GLEW_OK ) {
        cout << "Failed to initialize GLEW. The application is terminated." << endl;
        return EXIT_FAILURE;
    }

    cout << "GLEW initialized." << endl;

	
	/** Initialization **/
    glfwSetWindowTitle( "Lucid 0.0.0" );
	
	std::vector<GLuint> shaderList;
	GLuint lucidShaderProgram;
	
	shaderList.push_back( CreateShader(GL_VERTEX_SHADER, readFile("shaders/vertexshader.glsl")) );
	shaderList.push_back( CreateShader(GL_FRAGMENT_SHADER, readFile("shaders/fragmentshader.glsl")) );
	lucidShaderProgram = CreateProgram(shaderList);
	glDeleteShader( shaderList[0] ); // TODO : Cette ligne doit être prise en charge par shader.h ?
	GLint uniformLoop = glGetUniformLocation ( lucidShaderProgram, "loopDuration" );
	GLint uniformTime = glGetUniformLocation ( lucidShaderProgram, "time" );
	GLint uniformPerspectiveMatrix = glGetUniformLocation ( lucidShaderProgram, "perspectiveMatrix" );
	
	float fZnear = 0.5f;
	float fZfar = 5.0f;
	float fPerspectiveMatrix[16];
	memset ( fPerspectiveMatrix, 0, sizeof(float)*16 );
	fPerspectiveMatrix[0] = 1.0f; // Frustum scale on X
	fPerspectiveMatrix[5] = 1.0f; // Frustum scale on Y
	fPerspectiveMatrix[10] = (fZfar + fZnear) / (fZnear - fZfar);
	fPerspectiveMatrix[14] = (2 * fZfar * fZnear) / (fZnear - fZfar);
	fPerspectiveMatrix[11] = -1.0f;
	
	glUseProgram(lucidShaderProgram);
	glUniform1f ( uniformLoop, 5.0f );
	glUniformMatrix4fv ( uniformPerspectiveMatrix, 1, GL_FALSE, fPerspectiveMatrix );
	glUseProgram(0);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	/** Object Initialisation **/
	/*const float vertices[] = {
		 0.0f,    0.5f, 0.0f, 1.0f,
		 0.5f, -0.366f, 0.0f, 1.0f,
		-0.5f, -0.366f, 0.0f, 1.0f,
		 1.0f,    0.0f, 0.0f, 1.0f,
		 0.0f,    1.0f, 0.0f, 1.0f,
		 0.0f,    0.0f, 1.0f, 1.0f,
	};
	const float vertices2[] = {
		 0.0f,    0.35f, 0.0f, 1.0f,
		 0.35f, -0.2f, 0.0f, 1.0f,
		-0.35f, -0.2f, 0.0f, 1.0f,
		 1.0f,    0.0f, 0.0f, 1.0f,
		 0.0f,    1.0f, 0.0f, 1.0f,
		 0.0f,    0.0f, 1.0f, 1.0f,
	};*/
	const float cubeVertices[] = {
		// front
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// right
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		
		// top
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		
		1.0f, 1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, -1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// left
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, -1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
		
		0.0f, 1.0f, -1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// bottom
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 1.0f,
		
		// back
		0.0f, 0.0f, -1.0f, 1.0f,
		0.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		
		1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 0.0f, -1.0f, 1.0f,
		0.0f, 0.0f, -1.0f, 1.0f,
		
		// COLOR
		// front
		0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// right
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		
		// top
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// left
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		
		// bottom
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		
		// back
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};
	
	GLuint objectPosition;
	glGenBuffers(1, &objectPosition);

	/** Display */
	dLastTime = glfwGetTime();
	size_t colorData = sizeof(cubeVertices) / 2;
    do {
	
		glClearColor( 0.062f, 0.157f, 0.349f, 0.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(lucidShaderProgram);
		glUniform1f ( uniformTime, glfwGetTime() );
		
		glBindBuffer(GL_ARRAY_BUFFER, objectPosition);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)colorData);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(0);
		
        glfwSwapBuffers();
		
		dMeanRenderTime = uRenderTimeNumber * dMeanRenderTime;
		dMeanRenderTime += glfwGetTime() - dLastTime;
		dMeanRenderTime /= ++uRenderTimeNumber;
		dLastTime = glfwGetTime();
		
    } while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS
              && glfwGetWindowParam( GLFW_OPENED ));

	/** Termination **/
    glfwTerminate();

	cout << "Goodbye ! (Mean rendering time=" << dMeanRenderTime*1000 << ")" << endl;

    return EXIT_SUCCESS;
}

