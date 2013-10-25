#ifndef _LUCID_CAMERA_
#define _LUCID_CAMERA_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {

private:
	float fFrustumScale;
	float fZnear;
	float fZfar;
	
	glm::mat4 matCameraMatrix;
	glm::mat4 matPerspectiveMatrix;
	glm::mat4 matCameraPerspectiveMatrix;
	
	bool isNeedUpdate;
	
public:
	Camera (float, float, float near=1.0f);
	const glm::mat4& getCameraPerspectiveMatrix();
	//const glm::mat4& getPerspectiveMatrix ();
		/* Should not be longer used */
	void translate (const glm::vec3&);
};

#endif