#include "renderer.hpp"
#include <iostream>

Renderer::Renderer () {

	shader        = new Shader();
	
	vertexBuffers = (float*)0;
	indexBuffers  = (short*)0;
	vaos          = std::vector<GLuint*>();
	meshes        = std::vector<Mesh*>();
	
	// Rendering options
		// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);
	
		// Face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &indexBufferObject);
	
	needToUpdate = true;
}


Renderer::~Renderer () {
	delete shader;
	delete vertexBuffers;
	delete indexBuffers;
	
	for ( std::vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh )
		delete *mesh;
	for ( std::vector<GLuint*>::iterator vao = vaos.begin(); vao != vaos.end(); ++vao ) {
		glDeleteVertexArrays (1, *vao);
		delete *vao;
	}
}


void Renderer::addMesh (Mesh& mesh) {
	meshes.push_back (&mesh);
	
	GLuint *nextVAO = new GLuint;
	glGenVertexArrays(1, nextVAO);
	vaos.push_back (nextVAO);
	
	needToUpdate = true;
}


void Renderer::update () {
	// Buffer CPU-side
	unsigned totalVertexBufferSize = 0;
	unsigned totalIndexBufferSize = 0;
	for ( std::vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh ) {
		totalVertexBufferSize += (*mesh)->getVerticesSize();
		totalIndexBufferSize += (*mesh)->getIndicesSize();
	}
	
	if (vertexBuffers != 0)
		delete vertexBuffers;
	vertexBuffers = new float [totalVertexBufferSize*2];
	if (indexBuffers != 0)
		delete indexBuffers;
	indexBuffers = new short [totalIndexBufferSize];
	
	color_offset = totalVertexBufferSize*sizeof(float);
	size_t pVertex_offset = 0;
	size_t pIndex_offset = 0;
	for ( std::vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh ) {
		memcpy ( (float*)(vertexBuffers + pVertex_offset), (*mesh)->getVertices(), (*mesh)->getVerticesSize()*sizeof(float) );
		memcpy ( (float*)(vertexBuffers + totalVertexBufferSize + pVertex_offset), (*mesh)->getColors(), (*mesh)->getVerticesSize()*sizeof(float) );
		pVertex_offset += (*mesh)->getVerticesSize(); /* TODO : So, color size and vertices size must be the same value ! */

		memcpy ( (short*)(indexBuffers + pIndex_offset), (*mesh)->getIndices(), (*mesh)->getIndicesSize()*sizeof(short) );
		pIndex_offset += (*mesh)->getIndicesSize();
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, totalVertexBufferSize*2*sizeof(float), vertexBuffers, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndexBufferSize*sizeof(short), indexBuffers, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// VAO
	pVertex_offset = 0;
	for ( int i = 0; i < vaos.size(); i++ ) {
		glBindVertexArray(*(vaos[i]));
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		
		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // Color
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)pVertex_offset);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(color_offset + pVertex_offset));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBindVertexArray(0);
	}
	
	needToUpdate = false;
}


void Renderer::draw () {
	
	// Update if needed
	if (needToUpdate)
		update();
		
	// Clear
	glClearColor( 0.062f, 0.157f, 0.349f, 0.0f );
	glClearDepth( 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->useProgram(true);
		
	// Update model matrix
	for ( int i = 0; i < vaos.size(); i++ ) {
		glBindVertexArray(*(vaos[i]));
		shader->updateModelMatrix ((meshes[i])->getModelWorldMatrix());
		glDrawElements(GL_TRIANGLES, (meshes[i])->getIndicesSize(), GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}
	
	shader->useProgram(false);
}


void Renderer::close () {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}