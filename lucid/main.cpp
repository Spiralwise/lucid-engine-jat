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
    //glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
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
	
	// Shaders
	std::vector<GLuint> shaderList;
	GLuint lucidShaderProgram;
	
	shaderList.push_back( CreateShader(GL_VERTEX_SHADER, readFile("shaders/vertexshader.glsl")) );
	shaderList.push_back( CreateShader(GL_FRAGMENT_SHADER, readFile("shaders/fragmentshader.glsl")) );
	lucidShaderProgram = CreateProgram(shaderList);
	glDeleteShader( shaderList[0] ); // TODO : Cette ligne doit être prise en charge par shader.h ?
	
	GLint uniformPositionOffset = glGetUniformLocation ( lucidShaderProgram, "positionOffset" );
	GLint uniformPerspectiveMatrix = glGetUniformLocation ( lucidShaderProgram, "perspectiveMatrix" );
	
		// Perspective Matrix
	float fZnear = 1.5f;
	float fZfar = 5.0f;
	float fPerspectiveMatrix[16];
	memset ( fPerspectiveMatrix, 0, sizeof(float)*16 );
	fPerspectiveMatrix[0] = 1.0f; // Frustum scale on X
	fPerspectiveMatrix[5] = 1.0f; // Frustum scale on Y
	fPerspectiveMatrix[10] = (fZfar + fZnear) / (fZnear - fZfar);
	fPerspectiveMatrix[14] = (2 * fZfar * fZnear) / (fZnear - fZfar);
	fPerspectiveMatrix[11] = -1.0f;
	
	glUseProgram(lucidShaderProgram);
	glUniformMatrix4fv ( uniformPerspectiveMatrix, 1, GL_FALSE, fPerspectiveMatrix );
	glUseProgram(0);
	
	// Rendering options
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	/** Object Initialisation **/
	const float cubeVertices [] = {
		// Positions object 1
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		
		// Positions object 2
		-0.75f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.75f,
		-0.75f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.75f,
		0.0f, -0.5f, -0.75f,
		0.75f, -0.5f, 0.0f,
		0.0f, 0.5f, -0.75f,
		0.75f, 0.5f, 0.0f,
		
		// Colors
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};
	
	const GLshort cubeIndexes [] = {
		
		// front
		0, 1, 3,
		0, 3, 2,
		
		// right
		1, 5, 7,
		1, 7, 3,
		
		// top
		3, 7, 6,
		3, 6, 2,
		
		// left
		0, 6, 4,
		0, 2, 6,
		
		// bottom
		1, 0, 4,
		1, 4, 5,
		
		// back
		5, 4, 6,
		5, 6, 7
	};
	
	float fLoopDuration = 5.0f;
	
	size_t cubeVerticesOffset = sizeof(float) * 3 * 8;
	size_t colorDataOffset = sizeof(float) * 3 * 8 * 2;
	
	// Buffers
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	GLuint vao1, vao2;
	
		// Vertex Array
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
		// Element Array
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
		// VAO's
	glGenVertexArrays(1, &vao1);
	glBindVertexArray(vao1);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	
	glBindVertexArray(0);
	
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)cubeVerticesOffset);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)colorDataOffset);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	
	glBindVertexArray(0);
	

	/** Display **/
	dLastTime = glfwGetTime();
	size_t indexArraySize = sizeof(cubeIndexes) / sizeof(GLshort);
	float fTimeScale = 3.14159f * 2.0f / fLoopDuration;
	float fTimeOffset;
    do {
	
		fTimeOffset = fmod(glfwGetTime(), fLoopDuration);
		
		glClearColor( 0.062f, 0.157f, 0.349f, 0.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(lucidShaderProgram);
		
		glBindVertexArray(vao1);
		glUniform4f(uniformPositionOffset,
			cos(fTimeOffset * fTimeScale),
			sin(fTimeOffset * fTimeScale),
			-3.0f,
			0.0f);
		glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_SHORT, 0);
		
		glBindVertexArray(vao2);
		glUniform4f(uniformPositionOffset,
			cos(fTimeOffset * fTimeScale) -0.5f,
			sin(fTimeOffset * fTimeScale) -0.5f,
			-2.5f,
			0.0f);
		glDrawElements(GL_TRIANGLES, indexArraySize, GL_UNSIGNED_SHORT, 0);
		
		glBindVertexArray(0);

		glUseProgram(0);
		
        glfwSwapBuffers();
		
		dMeanRenderTime = uRenderTimeNumber * dMeanRenderTime;
		dMeanRenderTime += glfwGetTime() - dLastTime;
		dMeanRenderTime /= ++uRenderTimeNumber;
		dLastTime = glfwGetTime();
		
    } while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS
              && glfwGetWindowParam( GLFW_OPENED ));

	/** Termination **/
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    glfwTerminate();

	cout << "Goodbye ! (Mean rendering time=" << dMeanRenderTime*1000 << ")" << endl;

    return EXIT_SUCCESS;
}

