#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform float loopDuration;
uniform float time;

out vec4 theColor;

void main () {

	float timeScale = 3.14159f * 2.0f / loopDuration;
	float currentTime = mod(time, loopDuration);
	
	vec4 positionOffset = vec4 (
		cos(currentTime * timeScale) * 0.5f,
		sin(currentTime * timeScale) * 0.5f,
		0.0f,
		0.0f);
	gl_Position = position + positionOffset;
	theColor = color;
}