#include "vertex.hpp"

Vertex::Vertex () {
	position = glm::vec3();
	color    = glm::vec3();
}


Vertex::Vertex (const glm::vec3 &pos) {
	position = pos;
	color    = glm::vec3();
}


void Vertex::setPosition (const glm::vec3 &pos) {
	position = pos;
}


void Vertex::translate (const glm::vec3 &t) {
	position.x += t.x;
	position.y += t.y;
	position.z += t.z;
}