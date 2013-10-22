// Standard Libraries
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

// OpenGL Libraries
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Lucid Includes
#include "utils.hpp"
#include "renderer.hpp"


// Defines intearction
#define INTERACTION_STOP 0
#define INTERACTION_FORWARD 1
#define INTERACTION_BACKWARD 2
#define INTERACTION_UP 5
#define INTERACTION_DOWN 6

#define INTERACTION_ROTATION_LEFT 13
#define INTERACTION_ROTATION_RIGHT 14


using namespace std;
using namespace glm;


/* Variables */
	// Rendering context
GLFWwindow* myWindow = 0; // -> Renderer
Renderer* renderer   = 0;

	// Interactions
unsigned uInteractionState = INTERACTION_STOP;

	// Performance
double dMeanRenderTime = .0f;
unsigned uRenderTimeNumber = 0; // TODO En faire une classe dans le cadre de l'étude des performances.
double dLastTime;


/** GLFW Callback Functions */
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch ( action ) {
	
		case GLFW_PRESS:
			switch ( key ) {
			
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
					
				case GLFW_KEY_Z:
					uInteractionState = INTERACTION_FORWARD;
					break;
					
				case GLFW_KEY_S:
					uInteractionState = INTERACTION_BACKWARD;
					break;
					
				case GLFW_KEY_A:
					uInteractionState = INTERACTION_ROTATION_LEFT;
					break;
					
				case GLFW_KEY_E:
					uInteractionState = INTERACTION_ROTATION_RIGHT;
					break;
					
				case GLFW_KEY_R:
					uInteractionState = INTERACTION_UP;
					break;
					
				case GLFW_KEY_F:
					uInteractionState = INTERACTION_DOWN;
					break;
			}
			break;
			
		case GLFW_RELEASE:
			uInteractionState = INTERACTION_STOP;
			break;
	}
}

static void error_callback(int error, const char* description) {
	cout << "GLFW has catch an error (" << error << ") : " << description << endl;
}

/** GLFW **/
unsigned initGLFW () {

	if ( !glfwInit() ) {
        cout << "Failed to initialize GLFW. The application is terminated." << endl;
        return 0;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	myWindow = glfwCreateWindow( 1024, 768, "Lucid 0.0.0", NULL, NULL );
    if ( !myWindow ) {
        cout << "Failed to open GLFW window. The application is terminated." << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
	glfwMakeContextCurrent ( myWindow );
	glfwSetKeyCallback ( myWindow, key_callback );
	glfwSetErrorCallback ( error_callback );

    cout << "GLFW initialized." << endl;
	
	return 1;
}


/** GLEW/OpenGL **/
unsigned initGLEW () {

	glewExperimental = true;
    if ( glewInit() != GLEW_OK ) {
        cout << "Failed to initialize GLEW. The application is terminated." << endl;
        return EXIT_FAILURE;
    }
	
	cout << "GLEW initialized." << endl;
	
	return 1;
}


/** Termination **/
int close () {

	renderer->close();
	delete renderer;
    glfwTerminate();

	cout << "Goodbye ! (Mean rendering time=" << dMeanRenderTime*1000 << ")" << endl;
	
	return EXIT_SUCCESS;
}


int main( int argc, char** argv ) {

    cout << "Lucid Engine v0.0.0" << endl;

	if ( !initGLFW() || !initGLEW() )
		return EXIT_FAILURE;
	
	renderer = new Renderer();

	Mesh object1 = Mesh::generateCube();
	Mesh object2 = Mesh::generateCube();
	
	renderer->addMesh (object1);
	renderer->addMesh (object2);
	
	object1.translate (glm::vec3(-0.0f, 0.0f, -3.5f));
	object2.translate (glm::vec3(-1.5f, 0.0f, -6.0f));

	/** Display **/
	dLastTime = glfwGetTime();
	
    do {
		renderer->draw();
		
        glfwSwapBuffers ( myWindow );
		
		dMeanRenderTime = uRenderTimeNumber * dMeanRenderTime;
		dMeanRenderTime += glfwGetTime() - dLastTime;
		dMeanRenderTime /= ++uRenderTimeNumber;
		dLastTime = glfwGetTime();
		
		// Events
		glfwPollEvents();
		switch ( uInteractionState ) {
		
			case INTERACTION_FORWARD: 
				object1.translate( glm::vec3(0.0f, 0.0f, -0.001f) );
				break;
				
			case INTERACTION_BACKWARD:
				object1.translate( glm::vec3(0.0f, 0.0f, 0.001f) );
				break;
				
			case INTERACTION_ROTATION_LEFT:
				object1.rotate( 0.03f, glm::vec3(0.0f, 1.0f, 0.0f) );
				break;
				
			case INTERACTION_ROTATION_RIGHT:
				object1.rotate( 0.03f, glm::vec3(0.0f, -1.0f, 0.0f) );
				break;
				
			case INTERACTION_UP:
				object1.translate( glm::vec3(0.0f, 0.001f, 0.f) );
				break;
				
			case INTERACTION_DOWN:
				object1.translate( glm::vec3(0.0f, -0.001f, 0.0f) );
				break;
		}
		
    } while ( !glfwWindowShouldClose( myWindow ) );

	return close();
}

