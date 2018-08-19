#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
} object;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outDiffuse;
layout(location = 2) out vec4 outNormal;
layout(location = 3) out vec4 outMaterial;

void main() 
{
	vec3 unitNormal = normalize(inNormal);

	outPosition = vec4(inWorldPos, 1.0);
	outDiffuse = vec4(inTangent, 1.0f);
	outNormal = vec4(unitNormal, 1.0f);
	outMaterial = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
