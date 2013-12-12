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

	// Compute diffuse lighting
	vec3 cameraSpacePosition = getCameraSpacePosition();
	vec3 lightDirection      = vec3(0.0);
	
	vec4 attennuedIntensity  = applyLightIntensity (cameraSpacePosition, lightDirection);
	
	float cosAngleIncidence  = dot(normalize(fragmentNormal), lightDirection);
	cosAngleIncidence        = clamp (cosAngleIncidence, 0, 1);
	
	// Compute specular lighting
	vec3 viewDirection      = normalize (-cameraSpacePosition);
	vec3 reflectDirection   = reflect (-lightDirection, normalize(fragmentNormal));
	float specularIntensity = dot (viewDirection, reflectDirection);
	specularIntensity       = clamp (specularIntensity, 0, 1);
	specularIntensity       = cosAngleIncidence != 0.0 ? specularIntensity : 0.0;
	specularIntensity       = pow (specularIntensity, 4);
	
	// Render color
		// Ambient
	outputColor = fragmentDiffuseColor * ambientColor;
		// Diffuse
	outputColor += attennuedIntensity * fragmentDiffuseColor * cosAngleIncidence;
		// Specular
	outputColor += attennuedIntensity * specularIntensity * lightColor;

	
// Test
	//if ( lightPosition.y == 0.5 )
	//	outputColor = vec4(0.0, 1.0, 0.0, 1.0);
	//else
	//	outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}