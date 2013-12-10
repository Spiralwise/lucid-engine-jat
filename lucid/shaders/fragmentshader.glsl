#version 330

in vec4 fragmentDiffuseColor;
in vec3 fragmentNormal;
in vec3 fragmentPosition;

uniform vec4 ambientColor;
uniform vec4 lightColor;
uniform vec3 lightPosition;

out vec4 outputColor;

void main () {

	vec3 lightDirection = normalize (lightPosition - fragmentPosition);
	float lightDistance = distance ( lightPosition, fragmentPosition );

	float cosAngleIncidence = clamp ( dot(normalize(fragmentNormal), lightDirection), 0, 1);
	
	vec4 attenuedLightColor = lightColor / ( 1.0 + (lightDistance/4.0) );
	
	outputColor = attenuedLightColor * fragmentDiffuseColor * cosAngleIncidence + fragmentDiffuseColor * ambientColor;

	
// Tests
	//vertexColor = vec4(normal.x, normal.y, normal.z, 1.0);
	//if ( normalInCameraSpace.z > 0.0) 
	//	vertexColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	vertexColor = vec4(1.0, 0.0, 0.0, 1.0);
}