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
layout(location = 1) in vec3 inCameraPos;

layout(location = 0) out vec4 outColour;

void main()
{
	vec3 relCameraPos = inCameraPos - object.planetPos;
    vec3 relPosition = inWorldPos - object.planetPos;
    float cameraHeight = length(relCameraPos);
    vec3 lightDir = normalize(object.lightPos - inWorldPos);

	float a = cameraHeight / object.outerRadius;
	outColour = vec4(0.0f, 0.0f, a, 0.4f);
}
