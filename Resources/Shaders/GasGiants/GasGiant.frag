#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerDiffuse;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

#include "Shaders/Pipeline.glsl"

void main() 
{
	vec3 unitNormal = normalize(fragmentNormal);

	vec3 textureColour = texture(samplerDiffuse, fragmentUv).rgb;

	outColour = vec4(textureColour, 1.0f);
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
