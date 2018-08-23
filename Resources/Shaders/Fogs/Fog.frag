#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;

	vec4 colourCeiling;
	vec4 colourFloor;
	vec4 colourNight;

	vec3 planetPos;
	vec3 lightPos;
	float innerRadius;
	float outerRadius;
} object;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inCameraPos;

layout(location = 0) out vec4 outColour;

void main()
{
	outColour = vec4(object.colourCeiling.rgb, 0.4f);
}
