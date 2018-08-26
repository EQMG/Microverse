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

	vec3 planetPos;
	vec3 lightPos;
	vec3 invWavelength;
	float innerRadius;
	float outerRadius;
	float gMie;
	float gMie2;
	float KrESun;
	float KmESun;
	float Kr4PI;
	float Km4PI;
	float scale;
	float scaleDepth;
	float scaleOverScaleDepth;
	float samples;

	float hdrExposure;
} object;

layout(set = 0, location = 0) in vec3 inPosition;
layout(set = 0, location = 1) in vec2 inUv;
layout(set = 0, location = 2) in vec3 inNormal;

layout(location = 0) out vec3 outWorldPos;
    	
out gl_PerVertex
{
	vec4 gl_Position;
};

void main() 
{
	vec4 worldPosition = object.transform * vec4(inPosition, 1.0f);
//	vec4 worldNormal = object.transform * vec4(inNormal, 0.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

    outWorldPos = worldPosition.xyz;
}
