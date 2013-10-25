#ifndef _LUCID_COMMANDCONTROLER_
#define _LUCID_COMMANDCONTROLER_

#include <map>

#include "mesh.hpp"
#include "camera.hpp"

#define GLFW_DLL
#include <GLFW/glfw3.h>

/* ELEMENTs Code */
#define ALL_STOP 0

#define WIN_CLOSE 1

#define ELEMENT_FORWARD 101
#define ELEMENT_BACKWARD 102
#define ELEMENT_UP 105
#define ELEMENT_DOWN 106
#define ELEMENT_ROTATION_LEFT 113
#define ELEMENT_ROTATION_RIGHT 114

#define CAM_LEFT 203
#define CAM_RIGHT 204


typedef void(*pfunc_cmd)(void);
typedef int InputCode;
typedef int ActionCode;

class CommandControler {

private:
	GLFWwindow* currentWindow;
	Camera* currentCamera;
	Mesh* currentMesh;
	
public:
	static unsigned commandState;
	static std::map<InputCode, ActionCode> actionCommand;
	
	CommandControler (GLFWwindow*);
	void update ();
	static InputCode getCode (int, int);
	void setKey (ActionCode, int, int mods=0);
	
	void setCamera (Camera&);
	void setMesh (Mesh&);
	
	inline static void key_callback (GLFWwindow *win, int key, int scancode, int action, int mods) {
		switch ( action ) {
			case GLFW_PRESS:
				commandState = actionCommand[ CommandControler::getCode (key, mods) ];
				break;
				
			case GLFW_RELEASE:
				commandState = ALL_STOP;
				break;
		}
	}
};

#endif