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

	vec4 colourCeiling;
	vec4 colourFloor;
	vec4 colourNight;

	vec3 planetPos;
	vec3 lightPos;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outCameraPos;
    	
out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	vec4 totalLocalPos = vec4(inPosition, 1.0f);

	vec4 worldPosition = object.transform * totalLocalPos;

	gl_Position = scene.projection * scene.view * worldPosition;

    outWorldPos = worldPosition.xyz;
    outCameraPos = scene.cameraPos;
}
