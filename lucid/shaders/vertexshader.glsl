#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;

out vec4 theColor;

void main () {
	
	gl_Position = perspectiveMatrix * modelMatrix * position;
	//if ( color.g == 1.0 )
	//	theColor = vec4(0.0, 1.0, 0.0, 0.0);
	//else
	//	theColor = vec4(1.0, 0.0, 0.0, 0.0);
	theColor = color;
}