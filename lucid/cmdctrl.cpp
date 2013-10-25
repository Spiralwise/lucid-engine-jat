#include "cmdctrl.hpp"

unsigned CommandControler::commandState  = ALL_STOP;
std::map<InputCode, ActionCode> CommandControler::actionCommand = std::map<InputCode, ActionCode>();

/* TODO : Is there a better way to implement a Command Pattern in C++ ? Maybe, but I don't know yet. */
CommandControler::CommandControler (GLFWwindow* myWindow) {

	currentCamera = 0;
	currentMesh   = 0;
	
	currentWindow = myWindow;
	glfwSetKeyCallback (currentWindow, key_callback);
}


/** Commands **/
void CommandControler::update () {
	switch ( commandState ) {
		
		/* Window */
		case WIN_CLOSE:
			 glfwSetWindowShouldClose(currentWindow, GL_TRUE);
			 break;
		
		/* Element */
		case ELEMENT_FORWARD: 
			if ( currentMesh != 0 )
				currentMesh->translate ( glm::vec3(0.0f, 0.0f, -0.001f) );
			break;
			
		case ELEMENT_BACKWARD:
			if ( currentMesh != 0 )
				currentMesh->translate ( glm::vec3(0.0f, 0.0f, 0.001f) );
			break;
		
		/* Camera */
		case CAM_LEFT:
			if ( currentCamera != 0 )
				currentCamera->translate ( glm::vec3( -0.001f, 0.0f, 0.0f ) );
			break;
			
		case CAM_RIGHT:
			if ( currentCamera != 0 )
				currentCamera->translate ( glm::vec3( 0.001f, 0.0f, 0.0f ) );
			break;
	}
}


InputCode CommandControler::getCode (int key, int mods) {
	return (mods << 16) | (key & 0x0000ffff);
}


void CommandControler::setKey (ActionCode code, int key, int mods) {
	actionCommand[ CommandControler::getCode (key, mods) ] = code;
}


void CommandControler::setCamera (Camera& cam) {
	currentCamera = &cam;
}


void CommandControler::setMesh (Mesh& mesh) {
	currentMesh = &mesh;
}