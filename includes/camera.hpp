#ifndef _LUCID_CAMERA_
#define _LUCID_CAMERA_

#include <GL/glew.h>
#include <glm/glm.hpp>


class Camera {

private:
	float fFrustumScale;
	float fZnear;
	float fZfar;
	
	glm::mat4 matPerspectiveMatrix;
	
public:
	Camera (float, float, float near=1.0f);
	const glm::mat4& getPerspectiveMatrix ();
};

#endif