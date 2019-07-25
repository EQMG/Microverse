#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform UniformObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() 
{
	outPosition = vec4(inPosition, 1.0f);
	outDiffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	outNormal = vec4(normalize(normalize(inNormal)), 1.0f);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
