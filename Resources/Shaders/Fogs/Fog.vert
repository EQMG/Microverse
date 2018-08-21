#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 planetPos;
	vec3 cameraPos;
	vec3 lightPos;
	vec3 invWavelength;
	float cameraHeight;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, location = 0) in vec3 inPosition;

layout(location = 0) out vec3 outWorldPos;

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
}
