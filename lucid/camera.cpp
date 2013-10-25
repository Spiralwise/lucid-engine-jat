#include "camera.hpp"
#include <iostream>
Camera::Camera (float fov, float far, float near) {

	// Intrinsic parameters
	float fov_radian = fov * 3.14159f * 2.0f / 360.0f;
	fFrustumScale = 1.0f / tan(fov_radian/2.0f);
	fZnear        = near;
	fZfar         = far;
	
	// Extrinsic parameters
	matCameraMatrix           = glm::mat4();
	
	// Perspective matrix
	matPerspectiveMatrix      = glm::mat4 (0.0f);
	matPerspectiveMatrix[0].x = fFrustumScale/1.333f; // Frustum scale on X <-- Aspect ratio here
	matPerspectiveMatrix[1].y = fFrustumScale; // Frustum scale on Y
	matPerspectiveMatrix[2].z = (fZfar + fZnear) / (fZnear - fZfar);
	matPerspectiveMatrix[3].z = (2 * fZfar * fZnear) / (fZnear - fZfar);
	matPerspectiveMatrix[2].w = -1.0f;
	
	isNeedUpdate = true;
}

const glm::mat4& Camera::getCameraPerspectiveMatrix () {

	if ( isNeedUpdate ) {
		matCameraPerspectiveMatrix = matPerspectiveMatrix * matCameraMatrix;
		isNeedUpdate = false;
	}
	return matCameraPerspectiveMatrix;
}

/*const glm::mat4& Camera::getPerspectiveMatrix () {
	return matPerspectiveMatrix;
}*/

void Camera::translate (const glm::vec3& t) {
	matCameraMatrix = glm::translate (matCameraMatrix, t);
	isNeedUpdate = true;
}