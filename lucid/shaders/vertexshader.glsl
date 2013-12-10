#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 color;

uniform mat4 modelMatrix;
uniform mat3 normalModelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 perspectiveMatrix; 
	// TODO : Could be an uniform block but I must to know more about it.

out vec4 fragmentDiffuseColor;
out vec3 fragmentNormal;
out vec3 fragmentPosition;
	
void main () {
	
	// Position
	vec4 worldPosition = modelMatrix * vec4(position,1.0);
	gl_Position = perspectiveMatrix * cameraMatrix * worldPosition;
	
	// Lighting
	fragmentDiffuseColor = color;
	fragmentNormal       = normalModelMatrix * normal;
	fragmentPosition     = worldPosition;
}