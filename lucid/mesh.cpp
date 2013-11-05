#include "mesh.hpp"
#include <iostream>


/* Constructors */
Mesh::Mesh (const float *vert, unsigned verticesNum, const short *ind, unsigned indicesNum) {
	modelMatrix   = glm::mat4(1.0f);
	
	vertices = std::vector<float>();
	for ( unsigned i = 0; i < verticesNum; i++ )
		vertices.push_back (vert[i]);
		
	/* TODO : Remove it ! */
	// Default color for a cube
	colors = std::vector<float>();
	if ( verticesNum == 24 ) {
		const float cubeColors [] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
		};
		
		for ( unsigned i = 0; i < 24; i++ )
			colors.push_back (cubeColors[i]);
	} else {
		for ( unsigned i = 0; i < verticesNum; i++ )
			colors.push_back (0.0f);
	}
	
	indices = std::vector<short>();
	for ( unsigned i = 0; i < indicesNum; i++ )
		indices.push_back (ind[i]);
}


Mesh Mesh::generateCube () {
	const float cubeVertices [] = {
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f
	};
	
	const short cubeIndexes [] = {	
		// front
		0, 1, 3,
		0, 3, 2,
		
		// right
		1, 5, 7,
		1, 7, 3,
		
		// top
		3, 7, 6,
		3, 6, 2,
		
		// left
		0, 6, 4,
		0, 2, 6,
		
		// bottom
		1, 0, 4,
		1, 4, 5,
		
		// back
		5, 4, 6,
		5, 6, 7
	};
	
	return Mesh (cubeVertices, sizeof(cubeVertices)/sizeof(float), cubeIndexes, sizeof(cubeIndexes)/sizeof(short));
}


/* Getters */
const float* Mesh::getVertices () {
	return vertices.data();
}

const float* Mesh::getColors () {
	return colors.data();
}

const short* Mesh::getIndices () {
	return indices.data();
}

const unsigned Mesh::getVerticesSize () {
	return vertices.size();
}

const unsigned Mesh::getIndicesSize () {
	return indices.size();
}

const glm::mat4& Mesh::getModelWorldMatrix () {
	return modelMatrix;
}


/* Setters */
void Mesh::setPosition (const glm::vec3& pos) {
	modelMatrix[3] = glm::vec4(pos, 1.0f);
}


/* Methods */
void Mesh::translate (const glm::vec3& t) {
	modelMatrix = glm::translate( modelMatrix, t );
}

void Mesh::rotate (float angle, const glm::vec3& axis) {
	modelMatrix = glm::rotate( modelMatrix, angle, axis );
}