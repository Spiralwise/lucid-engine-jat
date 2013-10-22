#ifndef _LUCID_VERTEX_
#define _LUCID_VERTEX_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Vertex
{

private:
	glm::vec3 position;
	glm::vec3 color;

public:
	Vertex ();
	Vertex (const glm::vec3&);
	void setPosition (const glm::vec3&);
	void translate (const glm::vec3&);
	
};

#endif