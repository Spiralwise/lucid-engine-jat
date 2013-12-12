#include <iostream>

#include "renderer.hpp"

Renderer::Renderer () {

	shader        = new Shader();
	
	camera        = (Camera*)0;
	lights        = std::vector<Light*>();
	meshes        = std::vector<Mesh*>();
	
	vertexBuffers = (float*)0;
	normalBuffers = (float*)0;
	indexBuffers  = (short*)0;
	vaos          = std::vector<GLuint*>();
	
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
	
	glGenBuffers(1, &vertexBufferObject); // TODO Merge these lines ?
	glGenBuffers(1, &normalBufferObject);
	glGenBuffers(1, &indexBufferObject);
	
	needToUpdate = true;
}


Renderer::~Renderer () {
	delete shader;
	delete camera;
	delete vertexBuffers;
	delete normalBuffers;
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


void Renderer::addLight (Light& light) {
	lights.push_back (&light);
}


void Renderer::addAmbientLight (AmbientLight& light) {
	ambient = &light;
}


void Renderer::setCamera (Camera& cam) {
	camera = &cam;
}


void Renderer::update () {
	// Buffer CPU-side
	unsigned totalVertexBufferSize = 0;
	unsigned totalNormalBufferSize = 0;
	unsigned totalIndexBufferSize = 0;
	std::vector<unsigned> vertexSizes = std::vector<unsigned>();
	std::vector<unsigned> indexSizes = std::vector<unsigned>();
	for ( std::vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh ) {
		totalVertexBufferSize += (*mesh)->getVerticesSize();
		if ( (*mesh)->hasNormals() )
			totalNormalBufferSize += (*mesh)->getVerticesSize();
		totalIndexBufferSize += (*mesh)->getIndicesSize();
		vertexSizes.push_back ((*mesh)->getVerticesSize());
		indexSizes.push_back ((*mesh)->getIndicesSize());
	}
	
	if (vertexBuffers != 0)
		delete vertexBuffers;
	vertexBuffers = new float [totalVertexBufferSize*2];
	if (normalBuffers != 0)
		delete normalBuffers;
	normalBuffers = new float [totalNormalBufferSize];
	if (indexBuffers != 0)
		delete indexBuffers;
	indexBuffers = new short [totalIndexBufferSize];
	
	color_offset = totalVertexBufferSize*sizeof(float);
	size_t pVertex_offset = 0;
	size_t pNormal_offset = 0;
	size_t pIndex_offset = 0;
	for ( std::vector<Mesh*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++mesh ) {
		// Position
		memcpy ( (float*)(vertexBuffers + pVertex_offset), (*mesh)->getVertices(), (*mesh)->getVerticesSize()*sizeof(float) );
		// Normals
		if ( (*mesh)->hasNormals() ) {
			memcpy ( (float*)(normalBuffers + pNormal_offset), (*mesh)->getNormals(), (*mesh)->getVerticesSize()*sizeof(float) );
			pNormal_offset += (*mesh)->getVerticesSize();
		}
		// Color
		memcpy ( (float*)(vertexBuffers + totalVertexBufferSize + pVertex_offset), (*mesh)->getColors(), (*mesh)->getVerticesSize()*sizeof(float) );
		pVertex_offset += (*mesh)->getVerticesSize();
		// Indices
		memcpy ( (short*)(indexBuffers + pIndex_offset), (*mesh)->getIndices(), (*mesh)->getIndicesSize()*sizeof(short) );
		pIndex_offset += (*mesh)->getIndicesSize();
	}

// check data
/*std::cout << "Check data vertex:\n";
for ( int i = 0; i < totalVertexBufferSize; i++ ) {
	std::cout << *(normalBuffers+i) << " ";
}*/

/*std::cout << "Check data index:\n";
for ( int i = 0; i < totalIndexBufferSize; i++ ) {
	std::cout << *(indexBuffers+i) << " ";
}*/

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, totalVertexBufferSize*2*sizeof(float), vertexBuffers, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, totalNormalBufferSize*sizeof(float), normalBuffers, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndexBufferSize*sizeof(short), indexBuffers, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	// VAO
	pVertex_offset = 0;
	for ( int i = 0; i < vaos.size(); i++ ) {
		glBindVertexArray(*(vaos[i]));
		
			/* TODO : Retreive VertexAttrib ID with the correct method instead. */
		glEnableVertexAttribArray(0); // Position
		glEnableVertexAttribArray(1); // Normal
		glEnableVertexAttribArray(2); // Color
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)pVertex_offset);
			/* TODO Warning : Some objects doesn't have normal coord. */
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)pVertex_offset);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(color_offset + pVertex_offset));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
		glBindVertexArray(0);
		
		pVertex_offset += vertexSizes[i] * sizeof(GL_FLOAT); 
	}
	
	needToUpdate = false;
}


void Renderer::draw () {
	
	shader->useProgram(true);
	
	// Update if needed
	if (needToUpdate)
		update();
		
	// Clear
	glClearColor( 0.062f, 0.157f, 0.349f, 0.0f );
	glClearDepth( 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Update lighting matrix
	shader->updateAmbient (ambient->getColorIntensity());
	if ( lights.size() > 0 && camera != 0 )
		shader->updateLighting ((lights[0])->getPosition(), camera->getCameraMatrix(), (lights[0])->getColorIntensity());
	// Update perspective-camera matrix
	shader->updateCameraMatrix (camera->getPerspectiveMatrix(), camera->getCameraMatrix());
	
	// Update model matrices
	unsigned indices_offset = 0;
	for ( int i = 0; i < vaos.size(); i++ ) {
		glBindVertexArray(*(vaos[i]));
		shader->updateModelMatrix ((meshes[i])->getModelWorldMatrix());
		glDrawElements (
			GL_TRIANGLES, 
			(meshes[i])->getIndicesSize(), 
			GL_UNSIGNED_SHORT, 
			(GLvoid*)indices_offset);
		indices_offset += (meshes[i])->getIndicesSize() * sizeof(short);
		glBindVertexArray(0);
	}
	
	shader->useProgram(false);
}


void Renderer::close () {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);
}