#ifndef _LUCID_LIGHT_
#define _LUCID_LIGHT_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Light
{

protected:
	glm::vec3 position; /* TODO : Create a generic 3D object for all objects (like meshes...) */
	glm::vec4 intensity;
	
public:
	// Constructor
	Light (const glm::vec3&, const glm::vec3&, const float power=1.0f);
	
	// Getters
	const glm::vec3& getPosition ();
	const glm::vec4& getColorIntensity ();
};

#endif