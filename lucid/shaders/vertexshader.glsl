#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform float loopDuration;
uniform float time;
uniform float zNear;
uniform float zFar;
uniform float frustumScale;

out vec4 theColor;

void main () {

	float timeScale = 3.14159f * 2.0f / loopDuration;
	float currentTime = mod(time, loopDuration);
	
	vec4 positionOffset = vec4 (
		cos(currentTime * timeScale) - 0.25f,
		sin(currentTime * timeScale) - 0.25f,
		-3.0f,
		0.0f);
	vec4 cameraPos = position + positionOffset;
	vec4 clipPos;
	
	clipPos.xy = cameraPos.xy * frustumScale;
	clipPos.z = cameraPos.z * (zNear + zFar) + 2 * zNear * zFar;
	clipPos.z /= zNear - zFar;
	clipPos.w = -cameraPos.z;
	
	gl_Position = clipPos;
	theColor = color;
}