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
#include "cmdctrl.hpp"
#include "renderer.hpp"

using namespace std;
using namespace glm;


/* Variables */
	// Rendering context
GLFWwindow* myWindow = 0; // -> Renderer
CommandControler* controler = 0;
Renderer* renderer = 0;

	// Interactions
//unsigned uInteractionState = INTERACTION_STOP;

	// Performance
double dMeanRenderTime = .0f;
unsigned uRenderTimeNumber = 0; // TODO En faire une classe dans le cadre de l'étude des performances.
double dLastTime;


/** GLFW Callback Functions */
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
	delete controler;
    glfwTerminate();

	cout << "Goodbye ! (Mean rendering time=" << dMeanRenderTime*1000 << ")" << endl;
	
	return EXIT_SUCCESS;
}


int main( int argc, char** argv ) {

    cout << "Lucid Engine v0.0.0" << endl;

	if ( !initGLFW() || !initGLEW() )
		return EXIT_FAILURE;
	
	controler = new CommandControler (myWindow);
	renderer  = new Renderer();

	Camera cam   = Camera(45.0f, 10.0f);
	Mesh object1 = Mesh::generateCube();
	Mesh object2 = Mesh::generateCube();
	
	renderer->setCamera (cam);
	renderer->addMesh (object1);
	renderer->addMesh (object2);
	
	object1.translate (glm::vec3(-0.0f, 0.0f, -3.5f));
	object2.translate (glm::vec3(-1.5f, 0.0f, -6.0f));
	
	controler->setKey (WIN_CLOSE, GLFW_KEY_ESCAPE);
	controler->setKey (CAM_LEFT, GLFW_KEY_Q);
	controler->setKey (CAM_RIGHT, GLFW_KEY_D);
	controler->setKey (ELEMENT_FORWARD, GLFW_KEY_I);
	controler->setKey (ELEMENT_BACKWARD, GLFW_KEY_K);
	controler->setCamera (cam);
	controler->setMesh (object1);

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
		controler->update();
		
    } while ( !glfwWindowShouldClose( myWindow ) );

	return close();
}

