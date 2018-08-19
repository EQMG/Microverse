#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	float radius;
} object;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inPosition;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() 
{
	outPosition = vec4(inWorldPos, 1.0);
	outDiffuse = vec4(1.0f);
	outNormal = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	outMaterial = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
