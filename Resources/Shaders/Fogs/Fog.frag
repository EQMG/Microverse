#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	vec3 planetPos;
	vec3 lightPos;
	vec3 invWavelength;
	float innerRadius;
	float outerRadius;
	float gMie;
    float KrESun;
    float KmESun;
    float Kr4PI;
    float Km4PI;
	float scale;
	float scaleDepth;
	float scaleOverScaleDepth;
	float samples;

	float hdrExposure;
} object;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inCameraPos;

layout(location = 0) out vec4 outColour;

float sqr(float x)
{
	return x * x;
}

// Returns the near intersection point of a line and a sphere.
float getNearIntersection(vec3 pos, vec3 ray, float distance2, float radius2)
{
	float b = 2.0f * dot(pos, ray);
	float c = 4.0f * (distance2 - radius2);
	float det = max(0.0f, b * b - c);
	return 0.5f * (-b - sqrt(det));
}

// The scale equation calculated by Vernier's Graphical Analysis.
float scaleAngle(float cos)
{
	float x = 1.0f - cos;
	return object.scaleDepth * exp(-0.00287f + x * (0.459f + x * (3.83f + x * (-6.80f + x * 5.25f))));
}

// Calculates the Rayleigh phase function.
float getRayleighPhase(float cosAngle)
{
	return 0.75f + 0.75f * sqr(cosAngle);
}

// Calculates the Mie phase function.
float getMiePhase(float cosAngle)
{
	float mie2 = sqr(object.gMie);
	return 1.5f * ((1.0f - mie2) / (2.0f + mie2)) * (1.0f + sqr(cosAngle)) /
		pow(1.0f + mie2 - 2.0f * object.gMie * cosAngle, 1.5f);
}

// Calculates how much light is scattered toward the direction of the camera based on the angle.
float getPhase(float cosAngle, float g)
{
	float g2 = sqr(g);
	return 1.5f * ((1.0f - g2) / (2.0f + g2)) * ((1.0f + sqr(cosAngle)) / pow(1.0f + g2 - 2.0f * g * cosAngle, 1.5f));
}

void main()
{
	vec3 relCameraPos = inCameraPos - object.planetPos;
	vec3 relPosition = inWorldPos - object.planetPos;
	float cameraHeight = length(relCameraPos);
	vec3 lightDir = normalize(object.lightPos - inWorldPos);

	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere).
	vec3 ray = relPosition - relCameraPos;
	float far = length(ray);
	ray /= far;

	// Calculate the ray's start and end positions in the atmosphere, then calculate its scattering offset.
	float startOffset;
	vec3 start;

	// Calculate the closest intersection of the ray with the outer atmosphere.
	if (cameraHeight > object.outerRadius) {
		float near = getNearIntersection(relCameraPos, ray, sqr(cameraHeight), sqr(object.outerRadius));
		start = relCameraPos + (ray * near);
		far -= near;
		float startAngle = dot(ray, start) / object.outerRadius;
		float startDepth = exp(-1.0f / object.scaleDepth);
		startOffset = startDepth * scaleAngle(startAngle);
	} else {
		start = relCameraPos;
		float height = length(start);
		float startAngle = dot(ray, start) / height;
		float startDepth = exp(object.scaleOverScaleDepth * (object.innerRadius - cameraHeight));
		startOffset = startDepth * scaleAngle(startAngle);
	}

	// Initialize the scattering loop variables.
	float sampleLength = far / object.samples;
	float scaledLength = sampleLength * object.scale;
	vec3 sampleRay = ray * sampleLength;
	vec3 samplePoint = start + sampleRay * 0.5f;

	// Now loop through the sample points.
	vec3 frontColour = vec3(0.0f);

	for (int i = 0; i < int(object.samples); i++)
	{
		float height = length(samplePoint);
		float depth = exp(object.scaleOverScaleDepth * (object.innerRadius - height));
		float lightAngle = dot(lightDir, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = startOffset + depth * (scaleAngle(lightAngle) - scaleAngle(cameraAngle));
		vec3 attenuation = exp(-scatter * (object.invWavelength * object.Kr4PI + object.Km4PI));
		frontColour += attenuation * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader.
	vec3 rayleighColor = frontColour * (object.invWavelength * object.KrESun);
	vec3 adjustedMieColor = frontColour * object.KmESun;
	vec3 rayDirection = relCameraPos - relPosition;

	float cosAngle = dot(lightDir, rayDirection) / length(rayDirection);
	float rayleighPhase = getRayleighPhase(cosAngle);
	float miePhase = getMiePhase(cosAngle);
	outColour = rayleighPhase * vec4(rayleighColor, 1.0f) + miePhase * vec4(adjustedMieColor, 1.0f);

	// Lastly, apply HDR to reduce light and dark fog spots.
	outColour = 1.0f - exp(-object.hdrExposure * outColour);
}
