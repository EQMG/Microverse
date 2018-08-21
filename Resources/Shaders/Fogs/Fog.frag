#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 cameraPos;
	vec3 lightDir;
	vec3 invWavelength;
	float innerRadius;
	float outerRadius;
	float scaleDepth;
	float samples;
} object;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColour;

const float pi = 3.1415926535897932384626433832795f;
const float gMie = -0.95f;
const float gMie2 = gMie * gMie;
const float KrESun = 0.0025f * 15.0f;
const float KmESun = 0.0015f * 15.0f;
const float Kr4PI = 0.0025f * 4.0f * pi;
const float Km4PI = 0.0015f * 4.0f * pi;

float sqr(float x)
{
	return x * x;
}

// Returns the near intersection point of a line and a sphere.
float getNearIntersection(vec3 pos, vec3 ray, float distance2, float radius2)
{
	float b = 2.0f * dot(pos, ray);
	float c = 4.0f * (distance2 - radius2);
	float det = max(0.0f, sqr(b) - c);
	return 0.5f * (-b - sqrt(det));
}

// The scale equation calculated by Vernier's Graphical Analysis.
float scaleAngle(float cos)
{
	float x = 1.0f - cos;
	return object.scaleDepth * exp(-0.00287f + x * (0.459f + x * (3.83f + x * (-6.80f + x * 5.25f))));
}

// Calculates the Rayleigh phase function.
float getRayleighPhase(float cosAngle2)
{
	return 0.75f + 0.75f * cosAngle2;
}

// Calculates the Mie phase function.
float getMiePhase(float cosAngle, float cosAngle2)
{
	return 1.5f * ((1.0f - gMie2) / (2.0f + gMie2)) * (1.0f + cosAngle2) /
		pow(1.0f + gMie2 - 2.0f * gMie * cosAngle, 1.5f);
}

void main()
{
	float scale = 1.0f / (object.outerRadius - object.innerRadius);
	float scaleOverScaleDepth = scale / object.scaleDepth;
	float cameraHeight = length(object.cameraPos);

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere).
	vec3 ray = inPosition - object.cameraPos;
	float far = length(ray);
	ray /= far;

	// Calculate the ray's start and end positions in the atmosphere, then calculate its scattering offset.
	float startOffset;
	vec3 start;

	// Calculate the closest intersection of the ray with the outer atmosphere.
	if (cameraHeight > object.outerRadius) {
		float near = getNearIntersection(object.cameraPos, ray, sqr(cameraHeight), sqr(object.outerRadius));
		start = object.cameraPos + (ray * near);
		far -= near;
		float startAngle = dot(ray, start) / object.outerRadius;
		float startDepth = exp(-1.0f / object.scaleDepth);
		startOffset = startDepth * scaleAngle(startAngle);
	} else {
		start = object.cameraPos;
		float height = length(start);
		float startAngle = dot(ray, start) / height;
		float startDepth = exp(scaleOverScaleDepth * (object.innerRadius - cameraHeight));
		startOffset = startDepth * scaleAngle(startAngle);
	}

	// Initialize the scattering loop variables.
	float sampleLength = far / object.samples;
	float scaledLength = sampleLength * scale;
	vec3 sampleRay = ray * sampleLength;
	vec3 samplePoint = start + sampleRay * 0.5f;

	// Now loop through the sample points.
	vec3 frontColour = vec3(0.0f);

	for (int i = 0; i < int(object.samples); i++)
	{
		float height = length(samplePoint);
		float depth = exp(scaleOverScaleDepth * (object.innerRadius - height));
		float lightAngle = dot(object.lightDir, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = startOffset + depth * (scaleAngle(lightAngle) - scaleAngle(cameraAngle));
		vec3 attenuation = exp(-scatter * (object.invWavelength * Kr4PI + Km4PI));
		frontColour += attenuation * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader.
	vec3 rayleighColor = frontColour * (object.invWavelength * KrESun);
	vec3 adjustedMieColor = frontColour * KmESun;
	vec3 rayDirection = object.cameraPos - inPosition;

	float cosAngle = dot(object.lightDir, rayDirection) / length(rayDirection);
	float rayleighPhase = getRayleighPhase(sqr(cosAngle));
	float miePhase = getMiePhase(cosAngle, sqr(cosAngle));
	outColour = rayleighPhase * vec4(rayleighColor, 1.0f) + miePhase * vec4(adjustedMieColor, 1.0f);

	const float hdrExposure = 0.9f;
	outColour = 1.0f - exp(outColour * -hdrExposure);
}
