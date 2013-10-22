#ifndef _LUCID_MESH_
#define _LUCID_MESH_

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.hpp"


class Mesh
{

private:
	glm::mat4 modelMatrix;
	std::vector<float> vertices;
	std::vector<float> colors;
	std::vector<short> indices;
	
public:
	// Constructors
	Mesh (const float*, unsigned, const short*, unsigned);
	static Mesh generateCube ();
	
	// Getters
	const float* getVertices ();
	const float* getColors ();
	const short* getIndices ();
	const unsigned getVerticesSize ();
	const unsigned getIndicesSize ();
	const glm::mat4& getModelWorldMatrix ();
	
	// Setters
	void setPosition (const glm::vec3&);
	
	// Methods
	void translate (const glm::vec3&);
	void rotate (float, const glm::vec3&);
	
};

#endif