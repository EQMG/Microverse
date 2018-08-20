#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec4 colour;
	float innerRadius;
	float outerRadius;
	float falloff;
} object;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inPosition;

layout(location = 0) out vec4 outColour;

void main() 
{
	outColour = vec4(1.0f);
}
