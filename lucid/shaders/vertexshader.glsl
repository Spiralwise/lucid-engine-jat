#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform vec4 positionOffset;
uniform mat4 perspectiveMatrix;

out vec4 theColor;

void main () {

	vec4 cameraPos = position + positionOffset;
	
	gl_Position = perspectiveMatrix * cameraPos;
	theColor = color;
}