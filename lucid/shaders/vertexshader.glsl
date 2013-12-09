#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 color;

uniform vec4 lightIntensity;
uniform vec3 lightDirection;

uniform mat4 modelMatrix;
uniform mat3 normalModelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 perspectiveMatrix; 
	// TODO : Could be an uniform block but I must to know more about it.

out vec4 vertexColor;


void main () {
	
	// Position
	gl_Position = perspectiveMatrix * cameraMatrix * modelMatrix * vec4(position,1.0);
	
	// Lighting
	vec3 normalInCameraSpace = normalize (normalModelMatrix * normal);
	float cosAngleIncidence = clamp ( dot(normalInCameraSpace, lightDirection), 0, 1 );
	vertexColor = lightIntensity * color * cosAngleIncidence;
	//vertexColor = vec4(normalInCameraSpace.x, normalInCameraSpace.y, normalInCameraSpace.z, 1.0);
	//vertexColor = vec4(normal.x, normal.y, normal.z, 1.0);
	//if ( normalInCameraSpace.z > 0.0) 
	//	vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}