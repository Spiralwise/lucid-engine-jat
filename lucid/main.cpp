// Standard Libraries
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

// OpenGL Libraries
#include <GL/glew.h>
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
	glDeleteShader( shaderList[0] );
	glDeleteShader( shaderList[0] );
	
	
	/** Object Initialisation **/
	const float vertices[] = {
		0.75f, 0.75f, 0.0f, 1.0f,
		0.75f, -0.75f, 0.0f, 1.0f,
		-0.75f, -0.75f, 0.0f, 1.0f
		};
	GLuint objectPosition;
	glGenBuffers(1, &objectPosition);

    do {
		glClearColor( 0.062f, 0.157f, 0.349f, 0.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(lucidShaderProgram);
		
		glBindBuffer(GL_ARRAY_BUFFER, objectPosition);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &objectPosition, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(0);
		
        glfwSwapBuffers();
    } while ( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS
              && glfwGetWindowParam( GLFW_OPENED ));

    glfwTerminate();

    cout << "Goodbye !" << endl;

    return EXIT_SUCCESS;
}

