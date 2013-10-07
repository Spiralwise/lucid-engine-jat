#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform vec2 offset;

out vec4 theColor;

void main () {

	vec4 positionOffset = vec4( offset.x, offset.y, 0.0, 0.0 );
	gl_Position = position + positionOffset;
	theColor = color;
}