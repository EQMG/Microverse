#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 planetPos;
	vec3 lightPos;
	float planetRadius;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerRingLookup;

layout(location = 0) in vec3 inWorldPos;

layout(location = 0) out vec4 outColour;

const float smoothingDist = 200.0f;

float sphereIntersectAmount(vec3 raydir, vec3 rayorig, vec3 pos, float rad)
{
	float sphereDistance = dot(raydir, pos) - dot(raydir, rayorig);

	if (sphereDistance > 0.0f)
	{
		return 0.0f;
	}

	vec3 closestPoint = sphereDistance * raydir + rayorig;
	float distanceToSphere = length(closestPoint - pos);
	return distanceToSphere - rad;
}

void main()
{
	float len = length(inWorldPos - object.planetPos);

	if (len < object.innerRadius || len > object.outerRadius)
	{
		discard;
	}

	float u = (len - object.innerRadius) / (object.outerRadius - object.innerRadius);
	vec4 colour = texture(samplerRingLookup, vec2(u, 0.0f));

	vec3 lightDir = normalize(inWorldPos - object.lightPos);
	float shadow = clamp(-sphereIntersectAmount(lightDir, inWorldPos, object.planetPos, object.planetRadius) / smoothingDist, 0.05f, 1.0f);
	colour.rgb *= 1.0f - shadow;

	outColour = colour;
}
