#version 330

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 color;

uniform vec4 ambientColor;
uniform vec4 lightColor;
uniform vec3 lightPosition;

uniform mat4 modelMatrix;
uniform mat3 normalModelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 perspectiveMatrix; 
	// TODO : Could be an uniform block but I must to know more about it.

out vec4 vertexColor;


void main () {
	
	// Position
	vec4 worldPosition = modelMatrix * vec4(position,1.0);
	gl_Position = perspectiveMatrix * cameraMatrix * worldPosition;
	
	// Lighting
	vec3 lightDirection = normalize (lightPosition - worldPosition);
	vec3 normalInCameraSpace = normalize (normalModelMatrix * normal);
	float cosAngleIncidence = clamp ( dot(normalInCameraSpace, lightDirection), 0, 1 );
	vertexColor = lightColor * color * cosAngleIncidence + color * ambientColor;
	
// Tests
	//vertexColor = vec4(normal.x, normal.y, normal.z, 1.0);
	//if ( normalInCameraSpace.z > 0.0) 
	//	vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}