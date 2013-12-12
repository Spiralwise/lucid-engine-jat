#version 330

in vec4 fragmentDiffuseColor;
in vec3 fragmentNormal;
//in vec3 fragmentPosition;

uniform vec4 ambientColor;
uniform vec4 lightColor;
uniform vec3 lightPosition;

layout(std140) uniform Unprojection {
	mat4 matClipToCamera;
	ivec2 windowSize;
};

out vec4 outputColor;


vec3 getCameraSpacePosition () {

	vec4 ndcPosition;
	ndcPosition.xy = (gl_FragCoord.xy / windowSize.xy) * 2 - 1.0;
	ndcPosition.z = (2 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / (gl_DepthRange.far - gl_DepthRange.near);
	ndcPosition.w = 1.0;
	
	vec4 clipPos = ndcPosition / gl_FragCoord.w;
	
	return vec3 (matClipToCamera * clipPos);
}

vec4 applyLightIntensity (in vec3 cameraSpacePosition, out vec3 lightDirection) {

	vec3 lightDifference = lightPosition - cameraSpacePosition;
	float lightDistanceSqr = dot (lightDifference, lightDifference);
	lightDirection = lightDifference * inversesqrt(lightDistanceSqr);
	
	return lightColor * ( 1 / (1.0 + lightDistanceSqr) );
}

void main () {

	vec3 cameraSpacePosition = getCameraSpacePosition();
	vec3 lightDirection = vec3(0.0);
	vec4 attennuedIntensity = applyLightIntensity (cameraSpacePosition, lightDirection);

	float cosAngleIncidence = dot(normalize(fragmentNormal), lightDirection);
	cosAngleIncidence       = clamp (cosAngleIncidence, 0, 1);
	
	outputColor = attennuedIntensity * fragmentDiffuseColor * cosAngleIncidence + fragmentDiffuseColor * ambientColor;

	
// Test
	//if ( lightPosition.y == 0.5 )
	//	outputColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	outputColor = vec4(1.0, 0.0, 0.0, 1.0);
	//outputColor = vec4(cameraSpacePosition.x, cameraSpacePosition.y, -cameraSpacePosition.z-3.0, 1.0);
	//outputColor = vec4(cosAngleIncidence, cosAngleIncidence, cosAngleIncidence, 1.0);
}