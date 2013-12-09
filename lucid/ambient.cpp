#include "ambient.hpp"

/* Constructor */
AmbientLight::AmbientLight (const glm::vec3& color, const float power) : Light (glm::vec3(0.0f, 0.0f, 0.0f), color, power) {};