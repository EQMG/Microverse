#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerRingLookup;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inPosition;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

#include "Shaders/Noise.glsl"

void main()
{
	float len = length(inPosition);

	if (len < object.innerRadius || len > object.outerRadius)
	{
		discard;
	}

	vec3 unitNormal = normalize(inNormal);

	float u = (len - object.innerRadius) / (object.outerRadius - object.innerRadius);

	vec4 diffuse = texture(samplerRingLookup, vec2(u, 0.0f));

	outPosition = vec4(inWorldPos, 1.0);
	outDiffuse = diffuse;
	outNormal = vec4(unitNormal, 1.0f);
	outMaterial = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
