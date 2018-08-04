#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentUv;
layout(location = 2) in vec3 fragmentColour;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

vec3 blend(vec3 left, vec3 right, float blend)
{
	vec3 result = vec3(0.0, 0.0, 0.0);
	result.r = ((1.0f - blend) * left.r) + (blend * right.r);
	result.g = ((1.0f - blend) * left.g) + (blend * right.g);
	result.b = ((1.0f - blend) * left.b) + (blend * right.b);
	return result;
}

#include "Shaders/Pipeline.glsl"

void main() 
{
	vec3 unitNormal = normalize(fragmentNormal);

	outColour = vec4(fragmentColour, 1.0f);
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
