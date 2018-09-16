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
	vec3 position;
	float radius;
} object;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec2 inUv;
layout(set = 0, location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec2 outUv;
layout(location = 2) out vec3 outNormal;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(inPosition, 1.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

	outWorldPos = worldPosition.xyz;
	outUv = inUv;
	outNormal = inNormal;
}
