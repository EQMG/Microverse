#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
	vec3 cameraPos;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	vec3 planetPos;
	vec3 lightPos;
	vec3 invWavelength;
	float innerRadius;
	float outerRadius;
	float gMie;
	float gMie2;
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

layout(location = 0) out vec4 outColour;

float getNearIntersection(vec3 pos, vec3 ray, float distance2, float radius2)
{
	float b = 2.0f * dot(pos, ray);
	float c = distance2 - radius2;
	float det = max(0.0f, b * b - 4.0f * c);
	return 0.5f * (-b - sqrt(det));
}

float scale(float cos)
{
	float x = 1.0f - cos;
	return object.scaleDepth * exp(-0.00287f + x * (0.459f + x * (3.83f + x * (-6.80f + x * 5.25f))));
}

float getRayleighPhase(float cosAngle2)
{
	return 0.75f * (1.0f + cosAngle2);
}

float getMiePhase(float cosAngle, float cosAngle2)
{
    return 1.5f * ((1.0f - object.gMie2) / (2.0f + object.gMie2)) * (1.0f + cosAngle2) / 
        pow(1.0f + object.gMie2 - 2.0f * object.gMie * cosAngle, 1.5f);
}

void main()
{
	vec3 relCameraPos = scene.cameraPos - object.planetPos;
	vec3 relPosition = inWorldPos - object.planetPos;
	float cameraHeight = length(relCameraPos);
	float cameraHeight2 = cameraHeight * cameraHeight;
	vec3 lightDir = normalize(object.lightPos - inWorldPos);

	vec3 ray = relPosition - relCameraPos;
	float far = length(ray);
	ray /= far;

	float startOffset;
	vec3 start;

	if (cameraHeight > object.outerRadius)
	{
		float near = getNearIntersection(relCameraPos, ray, cameraHeight2, object.outerRadius * object.outerRadius);
		start = relCameraPos + ray * near;
		far -= near;
		float startAngle = dot(ray, start) / object.outerRadius;
		float startDepth = exp(-1.0f / object.scaleDepth);
		startOffset = startDepth * scale(startAngle);
	}
	else
	{
		start = relCameraPos;
		float height = length(start);
		float startAngle = dot(ray, start) / height;
		float startDepth = exp(object.scaleOverScaleDepth * (object.innerRadius - cameraHeight));
		startOffset = startDepth * scale(startAngle);
	}

	float sampleLength = far / object.samples;
	float scaledLength = sampleLength * object.scale;
	vec3 sampleRay = ray * sampleLength;
	vec3 samplePoint = start + sampleRay * 0.5f;

	vec3 frontColour = vec3(0.0f);

	for (int i = 0; i < int(object.samples); i++)
	{
		float height = length(samplePoint);
		float depth = exp(object.scaleOverScaleDepth * (object.innerRadius - height));
		float lightAngle = dot(lightDir, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = startOffset + depth * (scale(lightAngle) - scale(cameraAngle));
		vec3 attenuation = exp(-scatter * (object.invWavelength * object.Kr4PI + object.Km4PI));
		frontColour += attenuation * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	vec3 rayleighColor = frontColour * (object.invWavelength * object.KrESun);
	vec3 adjustedMieColor = frontColour * object.KmESun;
	vec3 rayDirection = relCameraPos - relPosition;

	float cosAngle = dot(lightDir, rayDirection) / length(rayDirection);
	float cosAngle2 = cosAngle * cosAngle;
	float rayleighPhase = getRayleighPhase(cosAngle2);
	float miePhase = getMiePhase(cosAngle, cosAngle2);
	outColour = rayleighPhase * vec4(rayleighColor, 1.0f) + miePhase * vec4(adjustedMieColor, 1.0f);

//	outColour = 1.0f - exp(-object.hdrExposure * outColour);
}
