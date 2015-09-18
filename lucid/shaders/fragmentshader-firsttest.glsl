#version 330

uniform float colorLoopDuration;
uniform float time;

in vec4 theColor;

out vec4 outputColor;

void main () {

	vec4 reverseColor = vec4(1.0f, 1.0f, 1.0f, 2.0f) - theColor;
	float colorLerpValue = mod(time, colorLoopDuration) / colorLoopDuration;
	colorLerpValue = sin(colorLerpValue * 3.14159f * 2.0f) * 0.5f + 0.5f;
	outputColor = mix(theColor, reverseColor, colorLerpValue);
}