#ifndef _LUCID_RENDERER_
#define _LUCID_RENDERER_

#include <cstring>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "light.hpp"
#include "ambient.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"

class Renderer {

private:
	// Shader
	Shader *shader;
	
	// Scene
		// Camera
	Camera *camera;
		// Objects
	AmbientLight *ambient;
	std::vector<Light*> lights;
	std::vector<Mesh*> meshes;
	
	// Buffers
	std::vector<GLuint*> vaos;
	
	GLuint vertexBufferObject;
	GLuint normalBufferObject;
	GLuint indexBufferObject;
	
	float *vertexBuffers;
	float *normalBuffers;
	short *indexBuffers;
	size_t color_offset;
	
	// Update
	bool needToUpdate;
	void update();
	
public:
	Renderer ();
	~Renderer ();
	void addMesh (Mesh&);
	void addLight (Light&);
	void addAmbientLight (AmbientLight&);
	void setCamera (Camera&);
	void draw ();
	void close ();
};

#endif