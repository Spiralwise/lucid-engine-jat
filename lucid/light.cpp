#include "light.hpp"

/* Constructor */
Light::Light (const glm::vec3& pos, const glm::vec3& color, const float power) {

	position = pos;
	intensity = glm::vec4(color, power);
}


/* Getters */
const glm::vec3& Light::getPosition () {

	return position;
}

const glm::vec4& Light::getColorIntensity () {

	return intensity;
}