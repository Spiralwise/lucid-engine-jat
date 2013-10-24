#ifndef _LUCID_RENDERER_
#define _LUCID_RENDERER_

#include <cstring>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Renderer {

private:
	// Shader
	Shader *shader;
	
	// Buffers
	std::vector<GLuint*> vaos;
	
	GLuint vertexBufferObject;
	GLuint indexBufferObject;
	
	float *vertexBuffers;
	short *indexBuffers;
	size_t color_offset;
	
	// Objects
	std::vector<Mesh*> meshes;
	
	// Update
	bool needToUpdate;
	void update();
	
public:
	Renderer ();
	~Renderer ();
	void addMesh (Mesh&);
	void setCamera (Camera&);
	void draw ();
	void close ();
};

#endif