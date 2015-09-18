#version 330

layout(location=0) in vec4 position;
layout(location=1) in vec4 color;

uniform mat4 modelMatrix;
uniform mat4 perspectiveMatrix;

out vec4 theColor;

void main () {
	
	float ndc_z;
	vec4 computedColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	gl_Position = perspectiveMatrix * modelMatrix * position;
	if ( color.r == 0 && color.g == 0 && color.b == 0 ) {
		ndc_z = gl_Position.z / gl_Position.w;
		computedColor.r = 1.0 - min(ndc_z, 0.5)*2;
		computedColor.b = (max(ndc_z, 0.5) - 0.5)*2;
		if ( ndc_z < 0.5 )
			computedColor.g = max(ndc_z-0.25, 0.0)*4;
		else
			computedColor.g = max(0.75-ndc_z, 0.0)*4;
		theColor = computedColor;
	}
	else
		theColor = color;
}